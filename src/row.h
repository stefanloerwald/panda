
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <vector>

namespace panda
{
   /// Type alias for row.
   template <typename Integer>
   using Row = std::vector<Integer>;

   // Aliases
   /// Type alias for row: equation.
   template <typename Integer>
   using Equation = Row<Integer>;
   /// Type alias for row: facet.
   template <typename Integer>
   using Facet = Row<Integer>;
   /// Type alias for row: inequality.
   template <typename Integer>
   using Inequality = Row<Integer>;
   /// Type alias for row: vertex.
   template <typename Integer>
   using Vertex = Row<Integer>;
}

