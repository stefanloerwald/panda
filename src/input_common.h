
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <istream>
#include <string>

#include "names.h"
#include "row.h"

namespace panda
{
   namespace input
   {
      enum class RelationOperator
      {
         GreaterEqual,
         Equal,
         LessEqual,
         Undecided
      };

      /// Returns true if the next token in the stream is an identifier.
      bool isIdentifier(std::istream&);
      /// Returns the identifier if the stream is at the position of an identifier. Throws otherwise.
      std::string identifier(std::istream&);
      /// Returns true if the next token in the stream is a relation operator.
      bool isRelation(std::istream&);
      /// Returns the relation operator if the stream is at the position of an
      /// relation operator. Throws otherwise.
      RelationOperator relationOperator(std::istream&);

      // Returns true if the stream is at the end of the file.
      bool isEndOfFile(std::istream&);

      /// Returns true if the next symbol is a special character for end of lines.
      bool isEndOfLine(std::istream&);
      /// Consumes all "end of line" characters at the beginning of the stream.
      void skipEndOfLine(std::istream&);

      /// Returns true if the next character is white space.
      bool isWhitespace(std::istream&);
      /// Consumes all whitespace characters up to the next non-whitespace character.
      void skipWhitespace(std::istream&);
      /// Trims trailing and leading whitespace.
      std::string trimWhitespace(std::string);

      /// Returns if any of the names occurs in the rest of the stream.
      bool containsNames(const Names&, std::istream&);

      /// Throws and exception of type ExceptionType with a message (first argument)
      /// and optionally the identifier (second argument).
      template <typename ExceptionType>
      void errorWithLineInformation(const std::string&, const std::string&);

      /// Brings a row into normal form (c^T x <= b).
      Row<int> normalize(Row<int>, RelationOperator);
      /// Converts a string into an integer. Empty string, '+' and '-' are considered
      /// valid numbers (1, 1, -1 respectively).
      int toInt(const std::string&);

      /// Consume all tokens until the next token is a keyword.
      std::istream& advanceToNextKeyword(std::istream&, std::string&);
      /// Returns the first word of a line.
      std::string firstWord(std::string);
      /// Returns the next word in the stream.
      std::string peekWord(std::istream&);
   }
}

#include "input_common.tpp"

