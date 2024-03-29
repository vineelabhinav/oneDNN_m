#===============================================================================
# Copyright 2020-2023 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#===============================================================================

set(CMAKE_CXX_CLANG_TIDY "")

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

if(DNNL_GPU_CONV_DEBUG)
    foreach(dir codegen conv ir pass reorder utils)
        file(GLOB_RECURSE conv_sources ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.cpp)
        foreach(f ${conv_sources})
        set_source_files_properties(${f} PROPERTIES COMPILE_FLAGS "-DGEN_CONV_DEBUG")
        endforeach()
    endforeach()
endif()

set(OBJ_LIB ${LIB_PACKAGE_NAME}_gpu_jit)
add_library(${OBJ_LIB} OBJECT ${SOURCES})
set_property(GLOBAL APPEND PROPERTY DNNL_LIB_DEPS
    $<TARGET_OBJECTS:${OBJ_LIB}>)
