
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_INTEGER_OPERATIONS
#include "algorithm_integer_operations.h"
#undef COMPILE_TEMPLATE_ALGORITHM_INTEGER_OPERATIONS

#include <cassert>
#include <cstdint>
#include <cstdlib>

using namespace panda;

namespace
{
   /// optimized gcd for unsigned integer values.
   template <typename Integer>
   Integer unsigned_gcd(Integer, Integer) noexcept;
}

template <typename Integer>
Integer panda::algorithm::gcd(Integer a, Integer b) noexcept
{
   using std::abs;
   return unsigned_gcd(static_cast<Integer>(abs(a)), static_cast<Integer>(abs(b)));
}

template <typename Integer>
Integer panda::algorithm::lcm(Integer a, Integer b) noexcept
{
   const auto gcd_value = gcd(a, b);
   using std::abs;
   return static_cast<Integer>(abs(a) * static_cast<Integer>(abs(b) / gcd_value));
}

short panda::algorithm::abs(const short n)
{
   return (n < 0) ? -n : n;
}

namespace
{
   template <typename Integer>
   Integer unsigned_gcd(Integer a, Integer b) noexcept
   {
      assert( a >= 0 && b >= 0 );
      while ( a != 0 && b != 0 )
      {
         if ( a > b )
         {
            a %= b;
         }
         else
         {
            b %= a;
         }
      }
      return (a == 0) ? b : a;
   }
}

