
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_constraint_unnamed_variables.h"

#include <stdexcept>

#include "input_common.h"

using namespace panda;

namespace
{
   Row<int> constraint(std::istream&, ConstraintType);
}

template <>
Row<int> panda::input::implementation::constraint<ConstraintType::Equation>(std::istream& stream)
{
   return ::constraint(stream, ConstraintType::Equation);
}

template <>
Row<int> panda::input::implementation::constraint<ConstraintType::Inequality>(std::istream& stream)
{
   return ::constraint(stream, ConstraintType::Inequality);
}

namespace
{
   /// We have to deal with two possible input types here:
   /// Raw format (no names, no relation operator) and PANDA format without names.
   /// This only differs in the presence of the missing relation operator.
   /// We allow a leading "()"-enclosed identifier in both cases.
   Row<int> constraint(std::istream& stream, ConstraintType type)
   {
      using namespace input;
      Inequality<int> ieq;
      RelationOperator relation = RelationOperator::Undecided;
      bool has_rhs = false;
      skipWhitespace(stream);
      const auto id = isIdentifier(stream) ? identifier(stream) : "";
      skipWhitespace(stream);
      while ( stream && !isEndOfLine(stream) && !isEndOfFile(stream) )
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
         }
         else
         {
            int coefficient;
            if ( !(stream >> coefficient) )
            {
               errorWithLineInformation<std::invalid_argument>("Non-integral token in inequality", id);
            }
            if ( has_rhs )
            {
               errorWithLineInformation<std::invalid_argument>("Only one coefficient allowed on right hand side", id);
            }
            if ( relation != RelationOperator::Undecided )
            {
               has_rhs = true;
            }
            ieq.push_back(coefficient);
         }
         skipWhitespace(stream);
      }
      if ( relation != RelationOperator::Undecided && !has_rhs )
      {
         errorWithLineInformation<std::invalid_argument>("No right hand side provided", id);
      }
      if ( ieq.empty() )
      {
         return Row<int>{};
      }
      if ( relation != RelationOperator::Undecided ) // invert the last entry if input was in PANDA format
      {
         ieq.back() *= -1;
      }
      return normalize(ieq, relation);
   }
}

