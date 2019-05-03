
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <iostream>
#include <string>

namespace panda
{
   /// Returns the contents of the next line without consuming it.
   std::string peekLine(std::istream&);
}

