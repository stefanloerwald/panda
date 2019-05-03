
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_constraint_named_variables.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "input_common.h"
#include "istream_peek_line.h"

using namespace panda;

namespace
{
   using Index = std::size_t;
   using Coefficient = int;
   using Token = std::string;
   using Identifier = std::string;
   std::pair<Index, Coefficient> term(const Token&, const Names&, const Identifier&);
   Row<int> constraint(std::istream&, const Names&, ConstraintType);
}

template <>
Row<int> panda::input::implementation::constraint<ConstraintType::Equation>(std::istream& stream, const Names& names)
{
   return ::constraint(stream, names, ConstraintType::Equation);
}

template <>
Row<int> panda::input::implementation::constraint<ConstraintType::Inequality>(std::istream& stream, const Names& names)
{
   return ::constraint(stream, names, ConstraintType::Inequality);
}

namespace
{
   using namespace input;
   std::pair<Index, Coefficient> term(const Token& token, const Names& names, const Identifier& id)
   {
      assert( !token.empty() );
      const auto end_of_factor = token.find_first_not_of("+-0123456789");
      const auto factor = toInt(token.substr(0, end_of_factor));
      if ( end_of_factor == std::string::npos ) // this is the RHS (because there is no name).
      {
         // c.f. algorithm_inequality_operations.tpp: the rhs is stored negated.
         return std::make_pair(names.size(), -factor);
      }
      const auto variable = token.substr(end_of_factor);
      const auto iterator = std::find(names.cbegin(), names.cend(), variable);
      if ( iterator == names.cend() )
      {
         errorWithLineInformation<std::invalid_argument>("Token \"" + variable + "\" is not a variable name", id);
      }
      const auto index = iterator - names.cbegin();
      return std::make_pair(index, factor);
   }

   Row<int> constraint(std::istream& stream, const Names& names, ConstraintType type)
   {
      Inequality<int> ieq(names.size() + 1, 0);
      auto relation = RelationOperator::Undecided;
      bool has_rhs = false;
      skipWhitespace(stream);
      const auto id = isIdentifier(stream) ? identifier(stream) : "";
      skipWhitespace(stream);
      auto line = peekLine(stream);
      if ( line.empty() )
      {
         return Row<int>{};
      }
      for ( ; stream && !isEndOfLine(stream) && !isEndOfFile(stream); skipWhitespace(stream) )
      {
         if ( isRelation(stream) )
         {
            if ( relation != RelationOperator::Undecided )
            {
               errorWithLineInformation<std::invalid_argument>("Multiple relation operators", id);
            }
            relation = relationOperator(stream);
            if ( type == ConstraintType::Equation && relation != RelationOperator::Equal )
            {
               errorWithLineInformation<std::invalid_argument>("Invalid relation operator for an equation", id);
            }
            else if ( type == ConstraintType::Inequality && relation == RelationOperator::Equal )
            {
               errorWithLineInformation<std::invalid_argument>("Invalid relation operator for an inequality", id);
            }
            continue;
         }
         std::string token;
         stream >> token;
         Index index;
         Coefficient coefficient;
         if ( token == "+" || token == "-" )
         {
            bool is_plus = token == "+";
            stream >> token;
            std::tie(index, coefficient) = term(token, names, id);
            if ( coefficient < 0 )
            {
               errorWithLineInformation<std::invalid_argument>("Excessive \"+\" or \"-\"", id);
            }
            if ( !is_plus )
            {
               coefficient *= -1;
            }
         }
         else
         {
            std::tie(index, coefficient) = term(token, names, id);
         }
         if ( ieq[index] != 0 && index + 1 < ieq.size() )
         {
            errorWithLineInformation<std::invalid_argument>("Multiple terms for variable \"" + names[index] + "\"", id);
         }
         ieq[index] = coefficient;
         if ( has_rhs )
         {
            const std::string message = ( index + 1 < ieq.size() )
                                        ? "No variables are allowed on the right hand side"
                                        : "Only one integer is allowed on the right hand side";
            errorWithLineInformation<std::invalid_argument>(message, id);
         }
         if ( index + 1 == ieq.size() )
         {
            if ( relation == RelationOperator::Undecided )
            {
               errorWithLineInformation<std::invalid_argument>("Constants are not allowed on the LHS of an inequality", id);
            }
            has_rhs = true;
         }
      }
      if ( !has_rhs )
      {
         errorWithLineInformation<std::invalid_argument>("Inequalities must have a right hand side", id);
      }
      return normalize(ieq, relation);
   }
}

