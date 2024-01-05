/*******************************************************************************
* Copyright 2016-2023 Intel Corporation
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

#define VCHECK_CONV(cond, msg, ...) \
    VCONDCHECK(create, check, convolution, (cond), status::invalid_arguments, \
            msg, ##__VA_ARGS__)

namespace dnnl {
namespace impl {
status_t conv_desc_init(convolution_desc_t *conv_desc, prop_kind_t prop_kind,
        alg_kind_t alg_kind, const memory_desc_t *src_desc,
        const memory_desc_t *weights_desc, const memory_desc_t *bias_desc,
        const memory_desc_t *dst_desc, const dims_t strides,
        const dims_t dilates, const dims_t padding_l, const dims_t padding_r) {
    VCHECK_CONV(!any_null(conv_desc, src_desc, weights_desc, dst_desc, strides,
                        padding_l),
            VERBOSE_NULL_ARG);
    VCHECK_CONV(one_of(alg_kind, convolution_auto, convolution_direct,
                        convolution_winograd),
            VERBOSE_BAD_ALGORITHM);

    if (padding_r == nullptr) padding_r = padding_l;

    auto cd = convolution_desc_t();
    cd.primitive_kind = primitive_kind::convolution;
    cd.prop_kind = prop_kind;
    cd.alg_kind = alg_kind;

    cd.diff_src_desc = cd.src_desc = zero_md();
    cd.diff_dst_desc = cd.dst_desc = zero_md();
    cd.diff_weights_desc = cd.weights_desc = zero_md();
    cd.diff_bias_desc = cd.bias_desc = zero_md();

    const bool is_fwd = one_of(prop_kind, forward_training, forward_inference);
    const bool with_bias
            = bias_desc && bias_desc->format_kind != format_kind::undef;
    const bool with_groups = weights_desc->ndims == src_desc->ndims + 1;

    bool runtime_dims_or_strides
            = memory_desc_wrapper(src_desc).has_runtime_dims_or_strides()
            || memory_desc_wrapper(weights_desc).has_runtime_dims_or_strides()
            || memory_desc_wrapper(dst_desc).has_runtime_dims_or_strides();
    if (with_bias)
        runtime_dims_or_strides = runtime_dims_or_strides
                || memory_desc_wrapper(bias_desc).has_runtime_dims_or_strides();
    VCONDCHECK(create, check, conv, !runtime_dims_or_strides,
            status::unimplemented, VERBOSE_RUNTIMEDIM_UNSUPPORTED);

    (prop_kind == backward_data ? cd.diff_src_desc : cd.src_desc) = *src_desc;
    (is_fwd ? cd.dst_desc : cd.diff_dst_desc) = *dst_desc;
    (prop_kind == backward_weights ? cd.diff_weights_desc : cd.weights_desc)
            = *weights_desc;
    if (with_bias)
        (prop_kind == backward_weights ? cd.diff_bias_desc : cd.bias_desc)
                = *bias_desc;

    cd.accum_data_type = types::default_accum_data_type(src_desc->data_type,
            weights_desc->data_type, dst_desc->data_type, prop_kind);
    VCHECK_CONV(cd.accum_data_type != data_type::undef,
            VERBOSE_INVALID_DATATYPE, "accumulation");

    VCHECK_CONV(memory_desc_wrapper(weights_desc).nelems(),
            VERBOSE_EMPTY_TENSOR, "weights");
    VCHECK_CONV(src_desc->ndims == dst_desc->ndims, VERBOSE_INCONSISTENT_NDIMS,
            "src", "dst");
    VCHECK_CONV(utils::one_of(src_desc->ndims, 3, 4, 5), VERBOSE_BAD_NDIMS,
            "src", src_desc->ndims);
    VCHECK_CONV(utils::one_of(weights_desc->ndims, src_desc->ndims,
                        src_desc->ndims + 1),
            VERBOSE_INCONSISTENT_NDIMS, "src", "weights");

    const int g = with_groups ? weights_desc->dims[0] : 1;
    const int bias_dim = prop_kind == backward_data ? src_desc->dims[1]
                                                    : dst_desc->dims[1];
    VCHECK_CONV(
            IMPLICATION(with_bias,
                    bias_desc->ndims == 1 && bias_desc->dims[0] == bias_dim),
            VERBOSE_BAD_DIM, "bias", 0);
    VCHECK_CONV(src_desc->dims[0] == dst_desc->dims[0],
            VERBOSE_INCONSISTENT_DIM, "src", 0, "dst", 0);
    VCHECK_CONV(src_desc->dims[1] == g * weights_desc->dims[with_groups + 1],
            VERBOSE_INCONSISTENT_DIM, "src", 1, "weights", with_groups + 1);
    VCHECK_CONV(dst_desc->dims[1] == g * weights_desc->dims[with_groups + 0],
            VERBOSE_INCONSISTENT_DIM, "dst", 1, "weights", with_groups + 0);

    int sp_dims = src_desc->ndims - 2;
    utils::array_copy(cd.strides, strides, sp_dims);
    utils::array_copy(cd.padding[0], padding_l, sp_dims);
    utils::array_copy(cd.padding[1], padding_r, sp_dims);
    if (dilates)
        utils::array_copy(cd.dilates, dilates, sp_dims);
    else
        utils::array_set(cd.dilates, 0, sp_dims);

    for (int i = 2; i < src_desc->ndims; ++i) {
        int src = src_desc->dims[i];
        int ker = weights_desc->dims[with_groups + i];
        int dil = cd.dilates[i - 2];
        int pad_l = padding_l[i - 2];
        int pad_r = padding_r[i - 2];
        int str = strides[i - 2];
        int dst = dst_desc->dims[i];
        int ker_range = 1 + (ker - 1) * (dil + 1);
        VCHECK_CONV(str > 0, VERBOSE_BAD_DIM, "strides", i - 2);
        VCHECK_CONV(dil >= 0 && pad_l >= 0 && pad_r + str > 0,
                VERBOSE_INCONSISTENT_PRB);
        VCHECK_CONV((src - ker_range + pad_l + pad_r) / str + 1 == dst,
                VERBOSE_INCONSISTENT_PRB);
    }

    *conv_desc = cd;
    return success;
}
} // namespace impl
} // namespace dnnl

status_t dnnl_convolution_forward_primitive_desc_create(
        primitive_desc_iface_t **primitive_desc_iface, engine_t *engine,
        prop_kind_t prop_kind, alg_kind_t alg_kind,
        const memory_desc_t *src_desc, const memory_desc_t *weights_desc,
        const memory_desc_t *bias_desc, const memory_desc_t *dst_desc,
        const dims_t strides, const dims_t dilates, const dims_t padding_l,
        const dims_t padding_r, const primitive_attr_t *attr) {
    if (!one_of(prop_kind, forward_training, forward_inference))
        return invalid_arguments;

    auto conv_desc = convolution_desc_t();
    CHECK(dnnl::impl::conv_desc_init(&conv_desc, prop_kind, alg_kind, src_desc,
            weights_desc, bias_desc, dst_desc, strides, dilates, padding_l,
            padding_r));
    return primitive_desc_create(primitive_desc_iface, engine,
            (const op_desc_t *)&conv_desc, nullptr, attr);
}

status_t dnnl_convolution_backward_data_primitive_desc_create(
        primitive_desc_iface_t **primitive_desc_iface, engine_t *engine,
        alg_kind_t alg_kind, const memory_desc_t *diff_src_desc,
        const memory_desc_t *weights_desc, const memory_desc_t *diff_dst_desc,
        const dims_t strides, const dims_t dilates, const dims_t padding_l,
        const dims_t padding_r, const primitive_desc_iface_t *hint_fwd_pd,
        const primitive_attr_t *attr) {

    auto conv_desc = convolution_desc_t();
    CHECK(dnnl::impl::conv_desc_init(&conv_desc, backward_data, alg_kind,
            diff_src_desc, weights_desc, nullptr, diff_dst_desc, strides,
            dilates, padding_l, padding_r));
    return primitive_desc_create(primitive_desc_iface, engine,
            (const op_desc_t *)&conv_desc, hint_fwd_pd, attr);
}

status_t dnnl_convolution_backward_weights_primitive_desc_create(
        primitive_desc_iface_t **primitive_desc_iface, engine_t *engine,
        alg_kind_t alg_kind, const memory_desc_t *src_desc,
        const memory_desc_t *diff_weights_desc,
        const memory_desc_t *diff_bias_desc, const memory_desc_t *diff_dst_desc,
        const dims_t strides, const dims_t dilates, const dims_t padding_l,
        const dims_t padding_r, const primitive_desc_iface_t *hint_fwd_pd,
        const primitive_attr_t *attr) {

    auto conv_desc = convolution_desc_t();
    CHECK(dnnl::impl::conv_desc_init(&conv_desc, backward_weights, alg_kind,
            src_desc, diff_weights_desc, diff_bias_desc, diff_dst_desc, strides,
            dilates, padding_l, padding_r));
    return primitive_desc_create(primitive_desc_iface, engine,
            (const op_desc_t *)&conv_desc, hint_fwd_pd, attr);
}

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
