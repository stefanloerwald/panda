
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   template <template <typename> class Functor>
   struct IntegerTypeSelector
   {
      /// Selects the correct integer type for execution of Functor::call(int, char**).
      static int select(int, char**);
   };
}

#include "integer_type_selection.tpp"

