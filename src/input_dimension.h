
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <istream>

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Reads the dimension from file.
         std::size_t dimension(std::istream&);
      }
   }
}

