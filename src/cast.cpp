
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_CAST
#include "cast.h"
#undef COMPILE_TEMPLATE_CAST

#include <cstddef>

using namespace panda;

template <typename Integer, typename OriginalType, is_different_t<Integer, OriginalType>>
Matrix<Integer> panda::cast(const panda::Matrix<OriginalType>& input)
{
   Matrix<Integer> output;
   output.reserve(input.size());
   for ( const auto& row : input )
   {
      output.push_back(cast<Integer>(row));
   }
   return output;
}

template <typename Integer, typename OriginalType, is_different_t<Integer, OriginalType>>
Row<Integer> panda::cast(const Row<OriginalType>& input)
{
   Row<Integer> output;
   output.reserve(input.size());
   for ( const auto& element : input )
   {
      output.push_back(Integer(element));
   }
   return output;
}

