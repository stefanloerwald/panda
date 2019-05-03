
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>

#include "constraint_type.h"
#include "names.h"
#include "row.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Reads a row which use variable names.
         template <ConstraintType>
         Row<int> constraint(std::istream&, const Names&);

         // specializations
         template <>
         Row<int> constraint<ConstraintType::Equation>(std::istream&, const Names&);
         template <>
         Row<int> constraint<ConstraintType::Inequality>(std::istream&, const Names&);
      }
   }
}

