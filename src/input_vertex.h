
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>

#include "matrix.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Returns a conical hull (PANDA format).
         ConicalHull<int> verticesConical(std::istream&);
         /// Returns a convex hull (PANDA format).
         ConvexHull<int> verticesConvex(std::istream&);
         /// Returns a convex hull (raw format).
         ConvexHull<int> vertices(std::istream&);
      }
   }
}

