
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "method_facet_enumeration.h"

#include <cassert>

#include "input_method.h"
#include "method_adjacency_decomposition.h"
#include "method_double_description.h"

using namespace panda;

int panda::method::facetEnumeration(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   const auto method = input::method(argc, argv);
   constexpr auto mode = OperationMode::FacetEnumeration;
   switch ( method )
   {
      case Method::AdjacencyDecomposition:
      {
         return method::adjacencyDecomposition<mode>(argc, argv);
      }
      case Method::DoubleDescription:
      {
         return method::doubleDescription<mode>(argc, argv);
      }
   }
   return 1;
}

