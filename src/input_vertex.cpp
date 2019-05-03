
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_vertex.h"

#include <cassert>
#include <stdexcept>
#include <string>

#include "algorithm_row_operations.h"
#include "input_common.h"
#include "input_keywords.h"
#include "istream_peek_line.h"

using namespace panda;

namespace
{
   Row<int> vertex(std::istream&);
}

ConicalHull<int> panda::input::implementation::verticesConical(std::istream& stream)
{
   ConicalHull<int> cone;
   std::string token;
   if ( !std::getline(stream, token) )
   {
      throw std::invalid_argument("Cannot read conical hull: file is at an invalid position.");
   }
   token = trimWhitespace(token);
   if ( !isKeywordConicalHull(token) && !isKeywordReducedConicalHull(token) )
   {
      throw std::invalid_argument("Cannot read conical hull: file is at an invalid position.");
   }
   while ( stream && !isKeyword(trimWhitespace(peekLine(stream))) )
   {
      if ( trimWhitespace(peekLine(stream)).empty() )
      {
         std::getline(stream, token);
         continue;
      }
      auto ray = ::vertex(stream);
      if ( !ray.empty() )
      {
         ray.back() = 0;
         cone.push_back(ray);
      }
      else
      {
         stream.clear();
      }
   }
   if ( stream && isKeywordEnd(firstWord(peekLine(stream))) )
   {
      std::getline(stream, token);
   }
   return cone;
}

ConvexHull<int> panda::input::implementation::verticesConvex(std::istream& stream)
{
   ConvexHull<int> conv;
   std::string token;
   if ( !std::getline(stream, token) )
   {
      throw std::invalid_argument("Cannot read convex hull: file is at an invalid position.");
   }
   token = trimWhitespace(token);
   if ( !isKeywordConvexHull(token) && !isKeywordReducedConvexHull(token) )
   {
      throw std::invalid_argument("Cannot read convex hull: file is at an invalid position.");
   }
   while ( stream && !isKeyword(trimWhitespace(peekLine(stream))) )
   {
      if ( trimWhitespace(peekLine(stream)).empty() )
      {
         std::getline(stream, token);
         continue;
      }
      auto vertex = ::vertex(stream);
      if ( !vertex.empty() )
      {
         conv.push_back(vertex);
      }
   }
   if ( stream && isKeywordEnd(firstWord(peekLine(stream))) )
   {
      std::getline(stream, token);
   }
   return conv;
}

ConvexHull<int> panda::input::implementation::vertices(std::istream& stream)
{
   ConvexHull<int> conv;
   while ( stream )
   {
      const auto vertex = ::vertex(stream);
      if ( !vertex.empty() )
      {
         conv.push_back(vertex);
      }
   }
   return conv;
}

namespace
{
   using namespace input;
   Row<int> vertex(std::istream& stream)
   {
      skipWhitespace(stream);
      const auto id = isIdentifier(stream) ? identifier(stream) : "";
      skipWhitespace(stream);
      Row<int> numerator;
      Row<int> denominator;
      for ( int tmp; stream >> tmp; )
      {
         numerator.push_back(tmp);
         skipWhitespace(stream);
         if ( stream.peek() == '/' )
         {
            stream.get(); // consume division operator
            stream >> tmp;
            if ( tmp == 0 )
            {
               errorWithLineInformation<std::invalid_argument>("Division by zero", id);
            }
            denominator.push_back(tmp);
         }
         else
         {
            denominator.push_back(1);
         }
         skipWhitespace(stream);
         if ( stream.peek() == '\n' )
         {
            break;
         }
      }
      assert( numerator.size() == denominator.size() );
      if ( numerator.empty() )
      {
         return numerator;
      }
      const auto lcm_value = algorithm::lcm(denominator);
      if ( lcm_value > 0 )
      {
         for ( std::size_t i = 0; i < numerator.size(); ++i )
         {
            numerator[i] *= lcm_value / denominator[i];
         }
      }
      numerator.push_back(lcm_value);
      return numerator;
   }
}

