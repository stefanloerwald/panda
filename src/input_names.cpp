
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_names.h"

#include <stdexcept>
#include <string>

#include "input_common.h"
#include "input_keywords.h"

using namespace panda;

Names panda::input::implementation::names(std::istream& stream)
{
   Names names;
   std::string token;
   if ( !std::getline(stream, token) || !isKeywordNames(trimWhitespace(token)) )
   {
      throw std::invalid_argument("Cannot read names: file is at an invalid position.");
   }
   for ( std::string name = peekWord(stream); stream && !isKeyword(name); name = peekWord(stream) )
   {
      stream >> name; // consume name (was only peeked)
      names.push_back(name);
   }
   return names;
}

