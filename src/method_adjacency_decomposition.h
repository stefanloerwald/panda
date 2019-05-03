
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "operation_mode.h"

namespace panda
{
   namespace method
   {
      /// Facet or vertex enumeration with adjacency decomposition.
      template <OperationMode>
      int adjacencyDecomposition(int, char**);
      template <>
      int adjacencyDecomposition<OperationMode::FacetEnumeration>(int, char**);
      template <>
      int adjacencyDecomposition<OperationMode::VertexEnumeration>(int, char**);
   }
}

