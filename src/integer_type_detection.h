
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "integer_type.h"

namespace panda
{
   /// Returns the user-selected integer type which is used for calculations.
   IntegerType integerType(int, char**);
}

