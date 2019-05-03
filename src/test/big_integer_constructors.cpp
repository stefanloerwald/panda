
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "big_integer.h"

#include <cstdint>
#include <limits>

using namespace panda;

namespace
{
   void test_constructors();
   void test_operator_int();
}

int main()
try
{
   test_constructors();
   test_operator_int();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using BI = BigInteger;

   void test_constructors()
   {
      ASSERT_NOTHROW(BI(std::numeric_limits<int>::min()), "BigInteger(int)");
      ASSERT_NOTHROW(BI(std::numeric_limits<int>::max()), "BigInteger(int)");
      #ifdef INT16_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::min()), "BigInteger(int16_t)");
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(int16_t)");
      #endif
      #ifdef UINT16_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(uint16_t)");
      #endif
      #ifdef INT32_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::min()), "BigInteger(int32_t)");
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(int32_t)");
      #endif
      #ifdef UINT32_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(uint32_t)");
      #endif
      #ifdef INT64_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::min()), "BigInteger(int64_t)");
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(int64_t)");
      #endif
      #ifdef UINT64_MAX
      ASSERT_NOTHROW(BI(std::numeric_limits<int16_t>::max()), "BigInteger(uint64_t)");
      #endif
   }

   void test_operator_int()
   {
      ASSERT_NOTHROW(static_cast<int>(BI(10)), "operator int()");
      ASSERT_NOTHROW(static_cast<int>(BI(-10)), "operator int()");
      ASSERT_NOTHROW(static_cast<int>(BI(0)), "operator int()");
      ASSERT(static_cast<int>(BI(10)) == 10, "operator int()");
      ASSERT(static_cast<int>(BI(-10)) == -10, "operator int()");
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      // a is now 2^128 which doesn't fit into int.
      ASSERT_ANY_EXCEPTION(static_cast<int>(a), "operator int()");
      const auto max = std::numeric_limits<int>::max();
      const auto min = std::numeric_limits<int>::min();
      ASSERT_NOTHROW(static_cast<int>(BI(max)), "operator int()");
      ASSERT(static_cast<int>(BI(max)) == max, "operator int()");
      ASSERT_NOTHROW(static_cast<int>(BI(min)), "operator int()");
      ASSERT(static_cast<int>(BI(min)) == min, "operator int()");
   }
}

