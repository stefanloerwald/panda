
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>

#include "names.h"
#include "maps.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Reads maps from file.
         Maps maps(std::istream&, const Names&);
      }
   }
}

