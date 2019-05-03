
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
      /// Facet or vertex enumeration with double description method.
      template <OperationMode>
      int doubleDescription(int, char**);
      template <>
      int doubleDescription<OperationMode::FacetEnumeration>(int, char**);
      template <>
      int doubleDescription<OperationMode::VertexEnumeration>(int, char**);
   }
}

