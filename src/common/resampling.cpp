/*******************************************************************************
* Copyright 2019-2023 Intel Corporation
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

#include <assert.h>
#include "oneapi/dnnl/dnnl.h"
#include "opdesc.hpp"
#include "primitive_desc_iface.hpp"

#include "c_types_map.hpp"
#include "type_helpers.hpp"
#include "utils.hpp"

using namespace dnnl::impl;
using namespace dnnl::impl::utils;
using namespace dnnl::impl::status;
using namespace dnnl::impl::prop_kind;
using namespace dnnl::impl::alg_kind;
using namespace dnnl::impl::types;

#define VCHECK_RS(cond, msg, ...) \
    VCONDCHECK(create, check, resampling, (cond), status::invalid_arguments, \
            msg, ##__VA_ARGS__);

namespace {
status_t resampling_desc_init(resampling_desc_t *resampling_desc,
        prop_kind_t prop_kind, alg_kind_t alg_kind, const float *factors,
        const memory_desc_t *src_desc, const memory_desc_t *dst_desc) {
    VCHECK_RS(one_of(alg_kind, resampling_nearest, resampling_linear),
            VERBOSE_BAD_ALGORITHM);
    VCHECK_RS(src_desc, VERBOSE_NULL_ARG);
    VCHECK_RS(IMPLICATION(dst_desc == nullptr, factors), VERBOSE_NULL_ARG);
    VCHECK_RS(utils::one_of(src_desc->ndims, 3, 4, 5), VERBOSE_BAD_NDIMS, "src",
            src_desc->ndims);

    const bool is_fwd = one_of(prop_kind, forward_training, forward_inference);
    VCHECK_RS(IMPLICATION(is_fwd, src_desc->format_kind != format_kind::any),
            VERBOSE_UNSUPPORTED_TAG_S, "src");

    auto rd = resampling_desc_t();
    rd.primitive_kind = primitive_kind::resampling;
    rd.prop_kind = prop_kind;
    rd.alg_kind = alg_kind;

    VCHECK_RS(!memory_desc_wrapper(src_desc).has_runtime_dims_or_strides(),
            VERBOSE_RUNTIMEDIM_UNSUPPORTED);
    VCHECK_RS(IMPLICATION(dst_desc,
                      !memory_desc_wrapper(dst_desc)
                               .has_runtime_dims_or_strides()),
            VERBOSE_RUNTIMEDIM_UNSUPPORTED);

    auto fill_dst_md = [](const memory_desc_t *i_md, const float *factors,
                               memory_desc_t *o_md) {
        o_md->ndims = i_md->ndims;
        o_md->data_type = i_md->data_type;
        utils::array_copy(o_md->dims, i_md->dims, 2);
        for (int i = 0; i < o_md->ndims - 2; i++)
            o_md->dims[2 + i] = (dim_t)(i_md->dims[2 + i] * factors[i]);
        o_md->format_kind = format_kind::any;
    };

    (prop_kind == backward_data ? rd.diff_src_desc : rd.src_desc) = *src_desc;
    if (dst_desc)
        (is_fwd ? rd.dst_desc : rd.diff_dst_desc) = *dst_desc;
    else {
        dst_desc = (is_fwd ? &rd.dst_desc : &rd.diff_dst_desc);
        fill_dst_md(
                src_desc, factors, (is_fwd ? &rd.dst_desc : &rd.diff_dst_desc));
    }

    /* User provided factors are used only to compute destination dimensions.
     Implementation uses true scaling factors from source to destination */
    for (int i = 0; i < src_desc->ndims - 2; i++)
        rd.factors[i] = (float)((double)dst_desc->dims[2 + i]
                / src_desc->dims[2 + i]);

    VCHECK_RS(src_desc->ndims == dst_desc->ndims, VERBOSE_INCONSISTENT_NDIMS,
            "src", "dst");
    for (int i : {0, 1})
        VCHECK_RS(src_desc->dims[i] == dst_desc->dims[i],
                VERBOSE_INCONSISTENT_DIM, "src", i, "dst", i);

    *resampling_desc = rd;
    return success;
}
} // namespace

status_t dnnl_resampling_forward_primitive_desc_create(
        primitive_desc_iface_t **primitive_desc_iface, engine_t *engine,
        prop_kind_t prop_kind, alg_kind_t alg_kind, const float *factors,
        const memory_desc_t *src_desc, const memory_desc_t *dst_desc,
        const primitive_attr_t *attr) {
    if (!one_of(prop_kind, forward_training, forward_inference))
        return invalid_arguments;

    auto resampling_desc = resampling_desc_t();
    CHECK(resampling_desc_init(&resampling_desc, prop_kind, alg_kind, factors,
            src_desc, dst_desc));
    return primitive_desc_create(primitive_desc_iface, engine,
            (const op_desc_t *)&resampling_desc, nullptr, attr);
}

status_t dnnl_resampling_backward_primitive_desc_create(
        primitive_desc_iface_t **primitive_desc_iface, engine_t *engine,
        alg_kind_t alg_kind, const float *factors,
        const memory_desc_t *diff_src_desc, const memory_desc_t *diff_dst_desc,
        const primitive_desc_iface_t *hint_fwd_pd,
        const primitive_attr_t *attr) {

    auto resampling_desc = resampling_desc_t();
    CHECK(resampling_desc_init(&resampling_desc, backward_data, alg_kind,
            factors, diff_src_desc, diff_dst_desc));
    return primitive_desc_create(primitive_desc_iface, engine,
            (const op_desc_t *)&resampling_desc, hint_fwd_pd, attr);
}
// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
