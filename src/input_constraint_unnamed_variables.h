
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>

#include "constraint_type.h"
#include "row.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Reads a row that is represented by its coefficients (without names).
         template <ConstraintType>
         Row<int> constraint(std::istream&);

         // specializations
         template <>
         Row<int> constraint<ConstraintType::Equation>(std::istream&);
         template <>
         Row<int> constraint<ConstraintType::Inequality>(std::istream&);
      }
   }
}

