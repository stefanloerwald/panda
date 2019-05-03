
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <type_traits>

#include "matrix.h"
#include "row.h"

namespace
{
   /// Helper type trait to enable methods if the types are not the same.
   template <typename A, typename B>
   using is_different_t = typename std::enable_if<!std::is_same<A, B>::value, int>::type;
}

namespace panda
{
   /// casting a Matrix of Integers of type OriginalType to a Matrix of Integers of type Integer.
   template <typename Integer, typename OriginalType, is_different_t<Integer, OriginalType> = 0>
   Matrix<Integer> cast(const Matrix<OriginalType>&);
   /// casting a Row of Integers of type OriginalType to a Row of Integers of type Integer.
   template <typename Integer, typename OriginalType, is_different_t<Integer, OriginalType> = 0>
   Row<Integer> cast(const Row<OriginalType>&);
}

#include "cast.eti"

