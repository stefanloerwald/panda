
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   namespace algorithm
   {
      /// returns the greatest common divisor of two integers.
      template <typename Integer>
      Integer gcd(Integer, Integer) noexcept;
      /// returns the least common multiple of two integers.
      template <typename Integer>
      Integer lcm(Integer, Integer) noexcept;
      /// overload for short, as the standard library lacks this one.
      short abs(const short);
   }
}

#include "algorithm_integer_operations.eti"

