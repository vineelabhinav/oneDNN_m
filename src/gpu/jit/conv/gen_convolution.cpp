/*******************************************************************************
* Copyright 2021-2023 Intel Corporation
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

#include "gpu/jit/conv/gen_convolution.hpp"

#include <iostream>
#include <utility>

#include "common/impl_registration.hpp"
#include "common/utils.hpp"
#include "common/verbose.hpp"
#include "gpu/jit/ir/kernel_info.hpp"
#include "gpu/jit/reorder/reorder_kernel.hpp"
#include "gpu/jit/utils/utils.hpp"
#include "gpu/ocl/ocl_utils.hpp"

#include "gpu/jit/conv/config.hpp"
#include "gpu/jit/conv/conv_kernel.hpp"
#include "gpu/jit/conv/zero_out.hpp"

namespace dnnl {
namespace impl {
namespace gpu {
namespace jit {

struct conv_pd_data_t {
    conv_config_t pd_cfg;
    tensor_config_t tensor_cfg;
    std::vector<kernel_info_t> kernel_infos;
};

class gen_convolution_t {
public:
    static const int max_kernels = 16;

    template <typename T>
    static status_t init_pd(T *pd, engine_t *engine) {
        try {
            using compute::compute_engine_t;
            auto *compute_engine = utils::downcast<compute_engine_t *>(engine);

            if (!compute_engine->mayiuse_ngen_kernels())
                return status::unimplemented;
            if (!pd->set_default_alg_kind(alg_kind::convolution_direct))
                return status::unimplemented;

            conv_problem_t prb;
            CHECK(prb.init(engine, pd));

            pd->data = std::make_shared<conv_pd_data_t>();
            memory_desc_t &src_md
                    = *(const_cast<memory_desc_t *>(pd->invariant_src_md()));
            memory_desc_t &wei_md
                    = *(const_cast<memory_desc_t *>(pd->invariant_wei_md()));
            memory_desc_t &bia_md
                    = *(const_cast<memory_desc_t *>(pd->invariant_bia_md()));
            memory_desc_t &dst_md
                    = *(const_cast<memory_desc_t *>(pd->invariant_dst_md()));
            CHECK(init_pd_time_cfg(prb, pd->data->pd_cfg, engine, pd, src_md,
                    wei_md, bia_md, dst_md, &pd->attr_));

            // XXX: This is to be removed once we'll be able to properly handle
            // out-of-registers cases during primitive creation.
            auto tmp_cfg = pd->data->pd_cfg;
            CHECK(init_cfg(tmp_cfg, pd));

            pd->data->tensor_cfg = get_tensor_config(pd->data->pd_cfg);
            pd->data->kernel_infos.reserve(max_kernels);
            CHECK(init_kernel_infos(pd));

            return status::success;
        } catch (std::runtime_error &err) {
            // If verbose is enabled, print the primitive case and rethrow the
            // exception.
            VERROR(gpu, "%s,%s", pd->info(engine), err.what());
            return status::runtime_error;
        }
    }

    gen_convolution_t() = default;

    template <typename T>
    status_t init(T *primitive, engine_t *engine) {
        try {
            auto &data = *primitive->pd()->data;
            auto &tensor_cfg = data.tensor_cfg;
            auto cfg = data.pd_cfg;
            CHECK(init_cfg(cfg, primitive->pd()));

            ir_info() << "Configuration:" << std::endl;
            ir_info() << cfg;

            init_nd_ranges(primitive, cfg);

            auto &kernel_infos = data.kernel_infos;
            for (int i = 0; i < int(kernel_infos.size()); i++) {
                auto &info = kernel_infos[i];
                switch (info.id()) {
                    case kernel_id_t::convolution:
                        try {
                            kernels_.push_back(make_kernel<conv_kernel_t>(
                                    primitive, engine, cfg, info));
                        } catch (const ngen::out_of_registers_exception &e) {
                            if (cfg.regs() < 256
                                    && cfg.hw_cfg().large_grf_support()) {
                                ir_warning()
                                        << "Failed to generate kernel with "
                                           "default register mode, attempting "
                                           "again with large_grf_mode "
                                           "enabled\n";
                                kernels_.push_back(make_kernel<conv_kernel_t>(
                                        primitive, engine, cfg, info,
                                        grf_mode_t::large));
                            } else {
                                throw e;
                            }
                        }
                        break;
                    case kernel_id_t::pre_reorder: {
                        reorder_config_t reorder_cfg(cfg.exec_cfg(),
                                tensor_cfg.user_layout(info.arg_name(1)),
                                tensor_cfg.compute_layout(info.arg_name(1)));
                        kernels_.push_back(make_kernel<reorder_kernel_t>(
                                primitive, engine, reorder_cfg, "conv_reorder",
                                info, cfg.is_dpas_or_dpasw_fma(),
                                grf_mode_t::matches));
                        break;
                    }
                    case kernel_id_t::post_reorder: {
                        reorder_config_t reorder_cfg(cfg.exec_cfg(),
                                tensor_cfg.compute_layout(info.arg_name(0)),
                                tensor_cfg.user_layout(info.arg_name(0)));
                        kernels_.push_back(make_kernel<reorder_kernel_t>(
                                primitive, engine, reorder_cfg, "conv_reorder",
                                info, cfg.is_dpas_or_dpasw_fma(),
                                grf_mode_t::matches));
                        break;
                    }
                    case kernel_id_t::zero_out:
                        if (can_skip_zero_out(info, cfg)) {
                            kernels_.emplace_back();
                            continue;
                        }
                        kernels_.push_back(make_kernel<zero_out_kernel_t>(
                                primitive, engine, cfg.exec_cfg(), info,
                                cfg.is_dpas_or_dpasw_fma(),
                                grf_mode_t::matches));
                        break;
                    default: ir_error_not_expected();
                }
                if (!kernels_[i]) return status::runtime_error;
            }
        } catch (std::runtime_error &err) {
            // If verbose is enabled, print the primitive case and rethrow the
            // exception.
            VERROR(gpu, "%s,%s", primitive->pd()->info(engine), err.what());
            return status::runtime_error;
        }

        return status::success;
    }

    template <typename T>
    status_t init_res_storage(
            const T *primitive, engine_t *engine, gpu_resource_t *r) const {
        auto &data = *primitive->pd()->data;
        auto &kernel_infos = data.kernel_infos;
        for (int i = 0; i < int(kernel_infos.size()); i++) {
            auto &kernel_info = kernel_infos[i];
            for (int j = 0; j < kernel_info.nargs(); j++) {
                if (!kernel_info.is_resource(j)) continue;
                ir_error_not_expected();
            }
        }
        return status::success;
    }

    template <typename T>
    status_t execute(const T *primitive, const exec_ctx_t &ctx) const {
        auto &data = *primitive->pd()->data;
        auto &kernel_infos = data.kernel_infos;

        int max_stage = 100;
        int nsubmitted = 0;
        int nkernels = int(kernel_infos.size());
        for (int stage = 0; stage < max_stage; stage++) {
            for (int i = 0; i < nkernels; i++) {
                auto &info = kernel_infos[i];
                if (info.stage_id() != stage) continue;

                if (kernels_[i]) {
                    std::vector<memory_storage_wrapper_t> storage_list;
                    info.init_memory_storage_list(storage_list, ctx, primitive);

                    compute::kernel_arg_list_t arg_list;
                    info.set_args(arg_list, storage_list);

                    CHECK(primitive->parallel_for(
                            ctx, nd_ranges_[i], kernels_[i], arg_list));
                }
                nsubmitted++;
                if (nsubmitted == nkernels) break;
            }
        }

        return status::success;
    }

private:
    template <typename T>
    static kernel_info_t &create_kernel_info(T *pd, kernel_id_t kernel_id) {
        auto &infos = pd->data->kernel_infos;
        ir_assert((int)infos.size() + 1 <= max_kernels);
        infos.emplace_back();
        auto &ret = infos.back();
        ret.set_id(kernel_id);
        return ret;
    }

    template <typename T>
    static status_t init_kernel_infos(T *pd) {
        auto &data = *pd->data;
        auto &cfg = data.pd_cfg;

        auto scratchpad = pd->scratchpad_registry().registrar();
        auto &conv_info = create_kernel_info(pd, kernel_id_t::convolution);

        // Initialize kernel arguments.
        uint32_t scratchpad_key = 1;
        for (auto &t : data.tensor_cfg.tensors()) {
            int compute_arg_key = t.arg_key;
            int user_arg_key = t.arg_key;
            size_t compute_size = t.compute_layout.size();
            auto compute_buf = make_buffer(t.name);
            auto user_buf = (t.needs_reorder ? make_buffer(t.name + "_user")
                                             : compute_buf);

            if (user_arg_key == -1) {
                ir_assert(!t.needs_reorder);
                ir_assert(!t.needs_zero_out);
                ir_error_not_expected();
                continue;
            }

            if (t.needs_reorder) {
                compute_arg_key = int(scratchpad_key);
                scratchpad.book(scratchpad_key, compute_size, 1,
                        ocl::OCL_BUFFER_ALIGNMENT);
                conv_info.register_scratchpad_arg(compute_buf, compute_arg_key,
                        /*is_input=*/t.is_input && !t.is_output, compute_size);
                scratchpad_key++;

                if (t.is_input) {
                    auto &reorder_info
                            = create_kernel_info(pd, kernel_id_t::pre_reorder);
                    reorder_info.register_user_arg(user_buf, user_arg_key,
                            /*is_input=*/true);
                    reorder_info.register_scratchpad_arg(compute_buf,
                            compute_arg_key,
                            /*is_input=*/false, compute_size);
                    reorder_info.set_nd_range(reorder_kernel_t<>::nd_range(
                            cfg.exec_cfg(), t.user_layout, t.compute_layout));
                }
                if (t.is_output) {
                    auto &reorder_info
                            = create_kernel_info(pd, kernel_id_t::post_reorder);
                    reorder_info.register_scratchpad_arg(compute_buf,
                            compute_arg_key,
                            /*is_input=*/true, compute_size);
                    reorder_info.register_user_arg(user_buf, user_arg_key,
                            /*is_input=*/false);
                    reorder_info.set_nd_range(reorder_kernel_t<>::nd_range(
                            cfg.exec_cfg(), t.compute_layout, t.user_layout));
                }
            }
            if (t.needs_zero_out) {
                auto &zero_out_info
                        = create_kernel_info(pd, kernel_id_t::zero_out);
                if (t.needs_reorder) {
                    zero_out_info.register_scratchpad_arg(compute_buf,
                            compute_arg_key,
                            /*is_input=*/false, compute_size);
                } else {
                    zero_out_info.register_user_arg(compute_buf,
                            compute_arg_key,
                            /*is_input=*/false);
                }
                auto size_var = var_t::make(type_t::u32(), "size");
                zero_out_info.register_internal_arg(
                        size_var, uint32_t(compute_size));
                zero_out_info.set_nd_range(zero_out_kernel_t<>::nd_range(
                        cfg.simd(), int(compute_size)));
            }
            if (!t.needs_reorder)
                conv_info.register_user_arg(user_buf, user_arg_key,
                        /*is_input=*/t.is_input && !t.is_output);
        }

        return status::success;
    }

    template <typename T>
    void init_nd_ranges(T *primitive, const conv_config_t &cfg) {
        auto *pd = primitive->pd();
        auto &data = *pd->data;
        int nkernels = int(data.kernel_infos.size());
        nd_ranges_.resize(nkernels);
        for (int i = 0; i < nkernels; i++) {
            auto &info = data.kernel_infos[i];
            switch (info.id()) {
                case kernel_id_t::convolution:
                    // Convolution kernel info is initialized at PD creation
                    // time when ND range/grid information are not known yet so
                    // we need to directly query config here.
                    nd_ranges_[i] = cfg.nd_range();
                    break;
                case kernel_id_t::pre_reorder:
                case kernel_id_t::post_reorder:
                case kernel_id_t::zero_out:
                    nd_ranges_[i] = info.nd_range();
                    break;
                default: ir_error_not_expected();
            }
        }
    }

    static bool can_skip_zero_out(
            const kernel_info_t &info, const conv_config_t &cfg) {
        ir_assert(info.id() == kernel_id_t::zero_out);
        auto &buf_name = info.arg_var(0).as<var_t>().name;
        if (buf_name == "wei") return cfg.can_skip_wei_zero_out();
        if (buf_name == "bia") return cfg.can_skip_bia_zero_out();
        ir_error_not_expected();
        return false;
    }

    std::vector<compute::kernel_t> kernels_;
    std::vector<compute::nd_range_t> nd_ranges_;
};

status_t gen_convolution_fwd_t::pd_t::init(engine_t *engine) {
    if (!is_fwd()) return status::unimplemented;
    CHECK(gen_convolution_t::init_pd(this, engine));
    return status::success;
}

status_t gen_convolution_fwd_t::init(engine_t *engine) {
    impl_.reset(new gen_convolution_t());
    return impl_->init(this, engine);
}

status_t gen_convolution_fwd_t::execute(const exec_ctx_t &ctx) const {
    return impl_->execute(this, ctx);
}

status_t gen_convolution_fwd_t::init_res_storage(
        engine_t *engine, gpu_resource_t *r) const {
    return impl_->init_res_storage(this, engine, r);
}

status_t gen_convolution_bwd_data_t::pd_t::init(engine_t *engine) {
    if (!is_bwd_d()) return status::unimplemented;
    CHECK(gen_convolution_t::init_pd(this, engine));
    return status::success;
}

status_t gen_convolution_bwd_data_t::init_res_storage(
        engine_t *engine, gpu_resource_t *r) const {
    return impl_->init_res_storage(this, engine, r);
}

status_t gen_convolution_bwd_weights_t::pd_t::init(engine_t *engine) {
    if (!is_bwd_w()) return status::unimplemented;
    CHECK(gen_convolution_t::init_pd(this, engine));
    return status::success;
}

status_t gen_convolution_bwd_data_t::init(engine_t *engine) {
    impl_.reset(new gen_convolution_t());
    return impl_->init(this, engine);
}

status_t gen_convolution_bwd_data_t::execute(const exec_ctx_t &ctx) const {
    return impl_->execute(this, ctx);
}

status_t gen_convolution_bwd_weights_t::init(engine_t *engine) {
    impl_.reset(new gen_convolution_t());
    return impl_->init(this, engine);
}

status_t gen_convolution_bwd_weights_t::init_res_storage(
        engine_t *engine, gpu_resource_t *r) const {
    return impl_->init_res_storage(this, engine, r);
}

status_t gen_convolution_bwd_weights_t::execute(const exec_ctx_t &ctx) const {
    return impl_->execute(this, ctx);
}

} // namespace jit
} // namespace gpu
} // namespace impl
} // namespace dnnl
