
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_integer_operations.h"

#include <limits>
#include <random>

using namespace panda;

int main()
try
{
   ASSERT(algorithm::gcd(0, 15) == 15, "");
   ASSERT(algorithm::gcd(15, 0) == 15, "");
   ASSERT(algorithm::gcd(1, 15) == 1, "");
   ASSERT(algorithm::gcd(15, 1) == 1, "");
   ASSERT(algorithm::gcd(-36, 150) == 6, "");
   ASSERT(algorithm::gcd(150, -36) == 6, "");
   std::random_device rd{};
   std::mt19937 mt{rd()};
   std::uniform_int_distribution<int> d_int(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
   for ( int i = 0; i < 100; ++i )
   {
      const auto r0 = d_int(mt);
      const auto r1 = d_int(mt);
      ASSERT(algorithm::gcd(r0, r1) == algorithm::gcd(r1, r0), "gcd is symmetric");
      ASSERT((r0 == 0 && r1 == 0) || algorithm::gcd(r0, r1) != 0, "gcd is not zero, if at least one number is not zero.");
      ASSERT(algorithm::gcd(r0, r1) == 0 || (r0 % algorithm::gcd(r0, r1)) == 0, "gcd != 0 must divide the numbers.");
      ASSERT(algorithm::gcd(r0, r1) == 0 || (r1 % algorithm::gcd(r0, r1)) == 0, "gcd != 0 must divide the numbers.");
   }
   #ifndef NO_FLEXIBILITY
   const auto max = std::numeric_limits<short>::max();
   const auto min = std::numeric_limits<short>::min();
   std::uniform_int_distribution<short> d_short(min, max);
   for ( int i = 0; i < 100; ++i )
   {
      const auto r0 = d_short(mt);
      const auto r1 = d_short(mt);
      ASSERT(algorithm::gcd(r0, r1) == algorithm::gcd(r1, r0), "gcd is symmetric");
      ASSERT((r0 == 0 && r1 == 0) || algorithm::gcd(r0, r1) != 0, "gcd is not zero, if at least one number is not zero.");
      ASSERT(algorithm::gcd(r0, r1) == 0 || (r0 % algorithm::gcd(r0, r1)) == 0, "gcd != 0 must divide the numbers.");
      ASSERT(algorithm::gcd(r0, r1) == 0 || (r1 % algorithm::gcd(r0, r1)) == 0, "gcd != 0 must divide the numbers.");
   }
   #endif
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

