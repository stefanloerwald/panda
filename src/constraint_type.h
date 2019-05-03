
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   /// The ConstraintType of a constraint is either equation of inequality.
   /// This is used in input methods.
   enum class ConstraintType
   {
      Equation,  /// The handled constraint is an equation.
      Inequality /// The handled constraint is an inequality.
   };
}

