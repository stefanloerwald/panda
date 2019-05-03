
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_INPUT_CONSTRAINT
#include "input_constraint.h"
#undef COMPILE_TEMPLATE_INPUT_CONSTRAINT

#include <stdexcept>
#include <string>

#include "algorithm_row_operations.h"
#include "input_common.h"
#include "input_constraint_named_variables.h"
#include "input_constraint_unnamed_variables.h"
#include "input_keywords.h"
#include "istream_peek_line.h"

using namespace panda;

namespace
{
   template <ConstraintType>
   void keyword_condition(std::istream&);
   template <ConstraintType>
   Row<int> potentiallyNamedConstraint(std::istream&, const Names&);
}

template <ConstraintType type>
Matrix<int> panda::input::implementation::constraints(std::istream& stream, const Names& names)
{
   keyword_condition<type>(stream);
   Matrix<int> rows;
   std::string token;
   while ( stream && !isKeyword(trimWhitespace(peekLine(stream))) )
   {
      if ( trimWhitespace(peekLine(stream)).empty() )
      {
         std::getline(stream, token);
         continue;
      }
      const auto row = potentiallyNamedConstraint<type>(stream, names);
      if ( !row.empty() )
      {
         rows.push_back(row);
      }
      skipWhitespace(stream);
      skipEndOfLine(stream);
   }
   if ( stream && isKeywordEnd(firstWord(peekLine(stream))) )
   {
      std::getline(stream, token);
   }
   return rows;
}

namespace
{
   template <>
   void keyword_condition<ConstraintType::Equation>(std::istream& stream)
   {
      std::string token;
      if ( !std::getline(stream, token) )
      {
         throw std::invalid_argument("Cannot read equations: file is at an invalid position.");
      }
      using namespace input;
      using namespace input::implementation;
      token = trimWhitespace(token);
      if ( !isKeywordEquations(token) )
      {
         throw std::invalid_argument("Cannot read equation: file is at an invalid position.");
      }
   }

   template <>
   void keyword_condition<ConstraintType::Inequality>(std::istream& stream)
   {
      std::string token;
      if ( !std::getline(stream, token) )
      {
         throw std::invalid_argument("Cannot read inequalities: file is at an invalid position.");
      }
      using namespace input;
      using namespace input::implementation;
      token = trimWhitespace(token);
      if ( !isKeywordInequalities(token) && !isKeywordReducedInequalities(token) )
      {
         throw std::invalid_argument("Cannot read inequalities: file is at an invalid position.");
      }
   }

   template <ConstraintType type>
   Row<int> potentiallyNamedConstraint(std::istream& stream, const Names& names)
   {
      if ( names.empty() || !input::containsNames(names, stream) )
      {
         return input::implementation::constraint<type>(stream);
      }
      else
      {
         return input::implementation::constraint<type>(stream, names);
      }
   }
}

