
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>

#include "constraint_type.h"
#include "matrix.h"
#include "names.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Reads constraints in PANDA format.
         template <ConstraintType>
         Matrix<int> constraints(std::istream&, const Names&);
      }
   }
}

#include "input_constraint.eti"

