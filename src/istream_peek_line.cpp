
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "istream_peek_line.h"

#include "scope_guard.h"

std::string panda::peekLine(std::istream& file)
{
   if ( !file )
   {
      return "";
   }
   const auto current_position = file.tellg();
   const auto guard = makeScopeGuard([current_position, &file]()
   {
      file.clear();
      file.seekg(current_position);
   });
   std::string line;
   std::getline(file, line);
   return line;
}

