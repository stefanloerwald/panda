
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_common.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "algorithm_row_operations.h"
#include "input_keywords.h"
#include "istream_peek_line.h"
#include "scope_guard.h"

using namespace panda;

bool panda::input::isIdentifier(std::istream& stream)
{
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
   });
   return stream && stream.peek() == '(';
}

std::string panda::input::identifier(std::istream& stream)
{
   std::string string;
   if ( !isIdentifier(stream) )
   {
      throw std::invalid_argument("Identifier must start with '('.");
   }
   stream.get(); // consume the `(`.
   skipWhitespace(stream);
   while ( stream &&
           !isEndOfLine(stream) &&
           !isEndOfFile(stream) &&
           stream.peek() != ')' )
   {
      string.push_back(static_cast<char>(stream.get()));
   }
   if ( stream.peek() != ')' )
   {
      std::string message = "Identifier must end with a ')'.";
      message += " The character `)` and the newline character `\\n`";
      message += " may not be contained in the identifier.";
      throw std::invalid_argument(message);
   }
   stream.get(); // consume the `)`.
   return trimWhitespace(string);
}

bool panda::input::isRelation(std::istream& stream)
{
   const auto stream_position = stream.tellg();
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
      stream.seekg(stream_position);
   });
   if ( !stream )
   {
      return false;
   }
   const auto c = stream.peek();
   if ( c == '<' || c == '=' || c == '>' )
   {
      stream.get();
      if ( (stream && stream.peek() == '=') || !stream || isWhitespace(stream) )
      {
         return true;
      }
   }
   return false;
}

input::RelationOperator panda::input::relationOperator(std::istream& stream)
{
   if ( !stream )
   {
      throw std::invalid_argument("Not a relation operator.");
   }
   const auto first = stream.get();
   if ( first == '<' )
   {
      if ( stream && stream.peek() == '=' )
      {
         stream.get();
         return RelationOperator::LessEqual;
      }
      throw std::invalid_argument("Relation operator \"<\" is invalid.");
   }
   else if ( first == '=' )
   {
      if ( stream && stream.peek() == '=' )
      {
         stream.get();
         return RelationOperator::Equal;
      }
      else if ( !stream || isWhitespace(stream) )
      {
         return RelationOperator::Equal;
      }
      else
      {
         throw std::invalid_argument("Relation operator \"=\" must be followed by whitespace.");
      }
   }
   else if ( first == '>' )
   {
      if ( stream && stream.peek() == '=' )
      {
         stream.get();
         return RelationOperator::GreaterEqual;
      }
      throw std::invalid_argument("Relation operator \">\" is invalid.");
   }
   throw std::invalid_argument("Not a relation operator.");
}

bool panda::input::isEndOfFile(std::istream& stream)
{
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
   });
   if ( !stream )
   {
      return false;
   }
   return stream.peek() == std::istream::traits_type::eof();
}

bool panda::input::isEndOfLine(std::istream& stream)
{
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
   });
   if ( !stream )
   {
      return false;
   }
   const auto c1 = stream.peek();
   return ( c1 == '\r' || c1 == '\n' );
}

bool panda::input::isWhitespace(std::istream& stream)
{
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
   });
   if ( !stream )
   {
      return false;
   }
   const auto c = stream.peek();
   return c == ' ' || c == '\t';
}

void panda::input::skipEndOfLine(std::istream& stream)
{
   while ( stream )
   {
      const auto guard = makeScopeGuard([&stream]()
      {
         stream.clear();
      });
      const auto c = stream.peek();
      if ( c != '\r' && c != '\n' )
      {
         break;
      }
      stream.get();
   }
}

void panda::input::skipWhitespace(std::istream& stream)
{
   while ( stream && isWhitespace(stream) )
   {
      stream.get();
   }
}

std::string panda::input::trimWhitespace(std::string string)
{
   string.erase(0, string.find_first_not_of(" \t")); // erases leading whitespace
   string.erase(string.find_last_not_of(" \t") + 1); // erases trailing whitespace
   return string;
}

bool panda::input::containsNames(const Names& names, std::istream& stream)
{
   const auto stream_position = stream.tellg();
   const auto guard = makeScopeGuard([&]()
   {
      stream.clear();
      stream.seekg(stream_position);
   });
   std::size_t counter = 0;
   constexpr std::size_t max_counter = 2000; // this constant is actually arbitrary. We simply assume that if the first 2000 tokens do not contain a name, it will hold for the rest as well.
   while ( stream && counter < max_counter )
   {
      auto position = names.cbegin();
      std::string word = "";
      while ( stream && position != names.cend() )
      {
         const auto character = static_cast<char>(stream.get());
         word.append(1, character);
         position = std::find_if(names.cbegin(), names.cend(), [&] (const Name& name) { return word.size() <= name.size() && name.substr(0, word.size()) == word; });
         if ( position != names.cend() && *position == word ) // match!
         {
            return true;
         }
         ++counter;
      }
   }
   // assuming that if a name occurs it will do so in the first few characters
   return false;
}

Row<int> panda::input::normalize(Row<int> row, RelationOperator relation)
{
   if ( relation == RelationOperator::GreaterEqual )
   {
      row *= -1; // invert, now row satisfies LessEqual.
   }
   return row;
}

int panda::input::toInt(const std::string& string)
{
   if ( string.empty() || string == "+" )
   {
      return 1;
   }
   if ( string == "-" )
   {
      return -1;
   }
   std::istringstream stream(string);
   int result;
   if ( !(stream >> result) )
   {
      throw std::invalid_argument("toInt: Argument is not an integer.");
   }
   return result;
}

std::istream& panda::input::advanceToNextKeyword(std::istream& stream, std::string& string)
{
   while ( stream )
   {
      string = peekLine(stream);
      string = trimWhitespace(string);
      if ( implementation::isKeyword(string) )
      {
         break;
      }
      std::getline(stream, string);
   }
   return stream;
}

std::string panda::input::firstWord(std::string line)
{
   line = trimWhitespace(line);
   const auto end_word = line.find_first_of(" \t");
   if ( end_word != std::string::npos )
   {
      line.erase(end_word);
   }
   return line;
}

std::string panda::input::peekWord(std::istream& stream)
{
   const auto stream_position = stream.tellg();
   auto guard = makeScopeGuard([&]()
   {
      stream.clear();
      stream.seekg(stream_position);
   });
   std::string token;
   if ( !(stream >> token) )
   {
      guard.dismiss();
   }
   return token;
}

