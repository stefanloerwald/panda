
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <vector>

#include "row.h"

namespace panda
{
   /// Type alias for matrix.
   template <typename Integer>
   using Matrix = std::vector<Row<Integer>>;

   // Aliases
   /// Synonym for matrix: ConicalHull.
   template <typename Integer>
   using ConicalHull = Matrix<Integer>;
   /// Synonym for matrix: ConvexHull.
   template <typename Integer>
   using ConvexHull = Matrix<Integer>;
   /// Synonym for matrix: Equations.
   template <typename Integer>
   using Equations = Matrix<Integer>;
   /// Synonym for matrix: Facets.
   template <typename Integer>
   using Facets = Matrix<Integer>;
   /// Synonym for matrix: Inequalities.
   template <typename Integer>
   using Inequalities = Matrix<Integer>;
   /// Synonym for matrix: Vertices.
   template <typename Integer>
   using Vertices = Matrix<Integer>;
}

