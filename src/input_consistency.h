
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "maps.h"
#include "matrix.h"
#include "names.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Checks the consistency of a convex hull description.
         void checkConsistency(const ConvexHull<int>&);
         /// Checks the consistency of a convex/conical hull description.
         void checkConsistency(const ConvexHull<int>&, const ConicalHull<int>&, const Names&, const Maps&, const std::size_t);
         /// Checks the consistency of an inequality description.
         void checkConsistency(const Inequalities<int>&, const Names&, const std::size_t);
         /// Checks the consistency of a polytope.
         void checkConsistency(const ConvexHull<int>&, const ConicalHull<int>&, const Inequalities<int>&, const Names&, const Maps&, const std::size_t);
      }
   }
}

