/*******************************************************************************
 * Copyright 2020-2023 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <fstream>
#include <stdlib.h>
#include "llvm_jit.hpp"
#include "llvm_jit_resolver.hpp"
#include <compiler/codegen/codegen_c.hpp>
#include <compiler/codegen/codegen_llvm.hpp>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <runtime/config.hpp>
#include <runtime/runtime.hpp>
#include <util/file.hpp>
#include <util/scoped_timer.hpp>
#include <util/utils.hpp>

SC_MODULE(jit.llvm)

namespace dnnl {
namespace impl {
namespace graph {
namespace gc {

static std::string dump_module_to_string(llvm::Module *m) {
    std::string ret;
    llvm::raw_string_ostream os(ret);
    os << *m;
    return ret;
}

static void optimize_llvm_module(llvm::TargetMachine *tm, llvm::Module *module,
        llvm::CodeGenOpt::Level llvm_opt) {
    llvm::PassManagerBuilder passbuilder;
    passbuilder.OptLevel = llvm_opt;
    passbuilder.SizeLevel = 0;
    passbuilder.LoopVectorize = true;
    passbuilder.SLPVectorize = true;
    llvm::legacy::PassManager MPM;
    llvm::legacy::FunctionPassManager FPM(module);
#if SC_LLVM_BACKEND < 16
    tm->adjustPassManager(passbuilder);
#endif
    passbuilder.Inliner = llvm::createFunctionInliningPass(
            static_cast<unsigned int>(llvm_opt), 0, false);

    MPM.add(new llvm::TargetLibraryInfoWrapperPass(tm->getTargetTriple()));
    MPM.add(llvm::createTargetTransformInfoWrapperPass(
            tm->getTargetIRAnalysis()));
    FPM.add(llvm::createTargetTransformInfoWrapperPass(
            tm->getTargetIRAnalysis()));

    passbuilder.populateFunctionPassManager(FPM);
    passbuilder.populateModulePassManager(MPM);

    FPM.doInitialization();
    for (llvm::Function &F : *module)
        FPM.run(F);
    FPM.doFinalization();
    MPM.run(*module);
    SC_MODULE_INFO << dump_module_to_string(module);
}
std::unique_ptr<llvm::TargetMachine> get_llvm_target_machine(
        llvm::CodeGenOpt::Level optlevel);

static void *resolve_llvm_symbol(
        llvm::ExecutionEngine *engine, const std::string &name);

struct llvm_jit_listeners {
    std::unique_ptr<llvm::JITEventListener> intel_jit_;
    std::unique_ptr<llvm::JITEventListener> perf_;
    llvm_jit_listeners()
        : intel_jit_(std::unique_ptr<llvm::JITEventListener>(
                llvm::JITEventListener::createIntelJITEventListener()))
        , perf_(std::unique_ptr<llvm::JITEventListener>(
                  llvm::JITEventListener::createPerfJITEventListener())) {}
};

std::shared_ptr<jit_module> llvm_jit::make_jit_module(
        const_ir_module_ptr module, bool generate_wrapper) {
    auto llvm_ctx = utils::make_unique<llvm::LLVMContext>();
    std::unique_ptr<llvm::Module> llvmmod;
    llvm_generator_pass gen {*llvm_ctx, llvmmod, generate_wrapper};
    auto new_mod = gen(module);
    ir_module_ptr copied_ir_module;
    auto &compiler_config = utils::compiler_configs_t::get();
    if (!compiler_config.dump_gen_code_.empty()) {
        copied_ir_module = module->deep_copy();
    }
    auto timer = SC_SCOPED_TIMER_INFO("pass.time.llvm_jit", "");
    auto &attr_table = *new_mod->attr_.get<std::shared_ptr<statics_table_t>>(
            ir_module_t::attr_key_t::MODULE_DATA_BUFFERS);
    bool use_managed_tp = new_mod->attr_.get<bool>(
            ir_module_t::attr_key_t::MANAGED_THREAD_POOL);
    std::string source_path = std::move(gen.out_source_path_);
    std::string err;
    llvm::raw_string_ostream ss(err);
    if (llvm::verifyModule(*llvmmod, &ss)) {
        ss << "Module:\n" << *llvmmod;
        throw std::runtime_error("LLVM module verify error: " + err);
    }

    auto opt = std::min(opt_level_, 3U);
    auto llvm_opt = static_cast<llvm::CodeGenOpt::Level>(opt);

    llvm::Module *mod_ptr = llvmmod.get();
    auto tm = get_llvm_target_machine(llvm_opt).release();
    optimize_llvm_module(tm, mod_ptr, llvm_opt);
    auto engine = llvm::EngineBuilder(std::move(llvmmod))
                          .setErrorStr(&err)
                          .setOptLevel(llvm_opt)
                          .setEngineKind(llvm::EngineKind::JIT)
                          .setMCJITMemoryManager(
                                  utils::make_unique<sc_llvm_jit_resolver>())
                          .create(tm);
    std::shared_ptr<llvm_jit_listeners> outlisteners;
    if (utils::compiler_configs_t::get().jit_profile_) {
        // one listener for all JIT modules
        static auto listeners = std::make_shared<llvm_jit_listeners>();
        engine->RegisterJITEventListener(listeners->intel_jit_.get());
        engine->RegisterJITEventListener(listeners->perf_.get());
        outlisteners = listeners;
    }
    if (!engine) {
        throw std::runtime_error("LLVM EngineBuilder error: " + err);
    }
    engine->finalizeObject();
    typedef void (*init_func_t)(void *ctx, void *mod);
    auto init_func = reinterpret_cast<init_func_t>(
            resolve_llvm_symbol(engine, "__sc_init__"));
    if (init_func) { init_func(nullptr, attr_table.data_.data_); }
    auto ret = std::make_shared<llvm_jit_module>(
            std::unique_ptr<llvm::ExecutionEngine>(engine), std::move(llvm_ctx),
            std::move(attr_table), std::move(outlisteners), use_managed_tp,
            source_path);
    ret->postprocess(new_mod);

    if (copied_ir_module) {
        std::stringstream of;
        std::ofstream dump_main_f;
        std::ofstream dump_header_f;
        std::ofstream dump_data_f;
        c_generator_optional_out_t optional_dump {
                &dump_main_f, &dump_header_f, &dump_data_f};
        std::string dump_path_base = compiler_config.dump_gen_code_ + '/';
        dump_path_base += module->attr_.get_or_else(
                ir_module_t::attr_key_t::NAME,
                "/cfake_jit_module-" + utils::get_unique_name_for_file());

        std::string dump_path = dump_path_base + ".cpp";
        std::string dump_header_path = dump_path_base + ".hpp";
        std::string dump_data_path = dump_path_base + "_data.cpp";
        utils::open_file_for_write(dump_main_f, dump_path);
        utils::open_file_for_write(dump_header_f, dump_header_path);
        utils::open_file_for_write(dump_data_f, dump_data_path);

        auto gen = create_c_generator(
                of, context_, generate_wrapper, &optional_dump);
        auto new_mod = gen(copied_ir_module);
    }
    return ret;
}

llvm_jit_module::llvm_jit_module(std::unique_ptr<llvm::ExecutionEngine> engine,
        std::unique_ptr<llvm::LLVMContext> llvm_ctx, statics_table_t &&globals,
        std::shared_ptr<llvm_jit_listeners> &&listeners,
        bool managed_thread_pool, const std::string &source_path)
    : jit_module(std::move(globals), managed_thread_pool)
    , listeners_(std::move(listeners))
    , llvm_ctx_(std::move(llvm_ctx))
    , engine_(std::move(engine))
    , source_path_(source_path) {}

std::vector<std::string> llvm_jit_module::get_temp_filenames() const {
    if (source_path_.empty()) { return {}; }
    return {source_path_};
}

llvm_jit_module::~llvm_jit_module() {
    if (!source_path_.empty()) { remove(source_path_.c_str()); }
}

static void *resolve_llvm_symbol(
        llvm::ExecutionEngine *engine, const std::string &name) {
#ifdef __APPLE__
    return engine->getPointerToNamedFunction("_" + name, false);
#else
    return engine->getPointerToNamedFunction(name, false);
#endif
}

void *llvm_jit_module::get_address_of_symbol(const std::string &name) {
    void *global_var = globals_.get_or_null(name);
    if (global_var) { return global_var; }

    return resolve_llvm_symbol(engine_.get(), name);
}
std::shared_ptr<jit_function_t> llvm_jit_module::get_function(
        const std::string &name) {
    void *fun = resolve_llvm_symbol(engine_.get(), name);
    void *wrapper = resolve_llvm_symbol(engine_.get(), name + "_0wrapper");
    if (fun || wrapper) {
        if (runtime_config_t::get().execution_verbose_) {
            return general_jit_function_t::make(shared_from_this(), fun,
                    wrapper, name, managed_thread_pool_);
        } else {
            return general_jit_function_t::make(shared_from_this(), fun,
                    wrapper, std::string(), managed_thread_pool_);
        }
    } else {
        return nullptr;
    }
}

} // namespace gc
} // namespace graph
} // namespace impl
} // namespace dnnl
