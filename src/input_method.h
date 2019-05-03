
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "method.h"

namespace panda
{
   namespace input
   {
      /// Determines the method of choice from the user (checks for command line argument --method=<m>).
      Method method(int, char**);
   }
}

