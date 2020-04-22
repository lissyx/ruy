/* Copyright 2019 Google LLC. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// Public header for some advanced usage patterns.

#ifndef RUY_RUY_RUY_ADVANCED_H_
#define RUY_RUY_RUY_ADVANCED_H_

#include <cstddef>
#include <functional>

#include "ruy/context.h"
#include "ruy/mat.h"
#include "ruy/matrix.h"
#include "ruy/path.h"
#include "ruy/prepack.h"
#include "ruy/side_pair.h"

namespace ruy {

// Low-level, explicit pre-packing API.
//
// The cost of packing an input matrix (either the LHS or RHS) is amortized
// across the non-depth dimension of the opposite input matrix. Thus, when the
// LHS has very few rows or the RHS has very few columns, the cost of packing
// the opposite input matrix can become significant. See pack.h for further
// information on packing.
//
// This file provides an API allowing a user to explicitly pack a matrix and
// reuse the pre-packed matrix, avoiding that cost.
//
// See example_prepack.cc for example usage.

template <Path CompiledPaths, typename LhsScalar, typename RhsScalar,
          typename DstScalar, typename MulParamsType>
void PrePackForMul(const Matrix<LhsScalar>& lhs, const Matrix<RhsScalar>& rhs,
                   const MulParamsType& mul_params, Context* context,
                   Matrix<DstScalar>* dst, PrepackedMatrix* prepacked_lhs,
                   PrepackedMatrix* prepacked_rhs,
                   std::function<void*(int)> alloc_fn) {
  Mat<LhsScalar> internal_lhs = ToInternal(lhs);
  Mat<RhsScalar> internal_rhs = ToInternal(rhs);
  Mat<DstScalar> internal_dst = ToInternal(*dst);
  SidePair<PrepackedMatrix*> prepacked(prepacked_lhs, prepacked_rhs);
  PrePackForMulInternal<CompiledPaths>(internal_lhs, internal_rhs, mul_params,
                                       context, &internal_dst, prepacked,
                                       alloc_fn);
}

template <Path CompiledPaths, typename LhsScalar, typename RhsScalar,
          typename DstScalar, typename MulParamsType>
void MulWithPrepacked(const Matrix<LhsScalar>& lhs,
                      const Matrix<RhsScalar>& rhs,
                      const MulParamsType& mul_params, Context* context,
                      Matrix<DstScalar>* dst, PrepackedMatrix* prepacked_lhs,
                      PrepackedMatrix* prepacked_rhs) {
  Mat<LhsScalar> internal_lhs = ToInternal(lhs);
  Mat<RhsScalar> internal_rhs = ToInternal(rhs);
  Mat<DstScalar> internal_dst = ToInternal(*dst);
  SidePair<PrepackedMatrix*> prepacked(prepacked_lhs, prepacked_rhs);

  MulWithPrepackedInternal<CompiledPaths>(internal_lhs, internal_rhs,
                                          mul_params, context, &internal_dst,
                                          prepacked);
}

}  // namespace ruy

#endif  // RUY_RUY_RUY_ADVANCED_H_
