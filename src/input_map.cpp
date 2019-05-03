
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_map.h"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "input_common.h"
#include "input_keywords.h"
#include "istream_peek_line.h"

using namespace panda;

namespace
{
   Image image(std::string, const Names&);
   Map map(std::istream&, const Names&);
}

Maps panda::input::implementation::maps(std::istream& stream, const Names& names)
{
   Maps maps;
   std::string token;
   if ( !std::getline(stream, token) || !isKeywordMaps(trimWhitespace(token)) )
   {
      throw std::invalid_argument("Cannot read maps: file is at an invalid position.");
   }
   while ( stream && !isKeyword(trimWhitespace(peekLine(stream))) )
   {
      if ( trimWhitespace(peekLine(stream)).empty() )
      {
         std::getline(stream, token);
         continue;
      }
      try
      {
         auto map = ::map(stream, names);
         if ( !map.empty() && !std::all_of(map.cbegin(), map.cend(), [](const Image& i){ return i.empty(); }) )
         {
            map.push_back({std::make_pair(names.size(), 1)});
            maps.push_back(map);
         }
         skipWhitespace(stream);
         skipEndOfLine(stream);
      }
      catch ( const std::exception& e )
      {
         throw std::invalid_argument("The error " + std::string(e.what()) + " occured in map #" + std::to_string(maps.size() + 1) + ".");
      }
   }
   if ( stream && isKeywordEnd(firstWord(peekLine(stream))) )
   {
      std::getline(stream, token);
   }
   return maps;
}

namespace
{
   int extractFactor(const std::string& token, std::size_t& position)
   {
      auto var_start = position;
      if ( token.find_first_of("+-0123456789", position) == position )
      {
         const auto starts_with_sign = (token[position] == '+') || (token[position] == '-');
         var_start = token.find_first_not_of("0123456789", position + (starts_with_sign ? 1 : 0));
      }
      const auto count = var_start - position;
      const auto factor = input::toInt(token.substr(position, count));
      position = var_start;
      return factor;
   }

   std::string extractName(const std::string& token, std::size_t& position)
   {
      if ( position == token.size() || position == std::string::npos )
      {
         return "";
      }
      if ( token.find_first_of("+-0123456789", position) == position )
      {
         return "";
      }
      const auto next_term = token.find_first_of("+-", position);
      const auto count = (next_term == std::string::npos) ? token.size() - position : next_term - position;
      const auto name = token.substr(position, count);
      position = next_term;
      return name;
   }

   /// Parses a string of the form x1-2x3+4x4-1 or similar.
   Image image(std::string token, const Names& names)
   {
      if ( token.empty() )
      {
         throw std::invalid_argument("Invalid token (is empty).");
      }
      Image result;
      std::size_t position = 0;
      while ( position != std::string::npos && position < token.size() )
      {
         const auto factor = extractFactor(token, position);
         const auto variable = extractName(token, position);
         if ( variable.empty() )
         {
            // not referring to a name => index == number of names
            result.push_back(std::make_pair(names.size(), factor));
         }
         else
         {
            const auto iterator = std::find(names.cbegin(), names.cend(), variable);
            if ( iterator == names.cend() )
            {
               throw std::invalid_argument("In \"" + token + "\" of a map, the token \"" + variable + "\" is not a variable.");
            }
            const auto index = iterator - names.cbegin();
            result.push_back(std::make_pair(index, factor));
         }
      }
      return result;
   }

   Map map(std::istream& stream, const Names& names)
   {
      using namespace input;
      Map result(names.size());
      skipWhitespace(stream);
      std::size_t index{0};
      // tokenize (read std::strings from stream before the next line break)
      while ( stream && !isEndOfLine(stream) && !isEndOfFile(stream) && index < result.size() )
      {
         std::string token;
         stream >> token;
         result[index] = image(token, names);
         ++index;
      }
      return result;
   }
}

