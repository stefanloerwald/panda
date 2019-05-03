
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "filename.h"
#include "input_order.h"
#include "operation_mode.h"

namespace panda
{
   /// Returns the file name that is passed as command line argument.
   Filename getFilename(int, char**);
   /// Returns the user-provided order of input.
   InputOrder getInputOrder(int, char**);
   /// Returns the user-provided operation mode.
   OperationMode detectOperationMode(int, char**);
}

