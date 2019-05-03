
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "big_integer.h"

using namespace panda;

namespace
{
   void test_operator_int_equality();
   void test_operator_int_inequality();
   void test_operator_int_less();
   void test_operator_int_greater();
   void test_operator_int_leq();
   void test_operator_int_geq();
   void test_operator_bigint_equality();
   void test_operator_bigint_inequality();
   void test_operator_bigint_less();
   void test_operator_bigint_greater();
   void test_operator_bigint_leq();
   void test_operator_bigint_geq();
}

int main()
try
{
   test_operator_int_equality();
   test_operator_int_inequality();
   test_operator_int_less();
   test_operator_int_greater();
   test_operator_int_leq();
   test_operator_int_geq();
   test_operator_bigint_equality();
   test_operator_bigint_inequality();
   test_operator_bigint_less();
   test_operator_bigint_greater();
   test_operator_bigint_leq();
   test_operator_bigint_geq();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using BI = BigInteger;

   void test_operator_int_equality()
   {
      ASSERT(BI(10) == 10, "operator==(int)");
      ASSERT(BI(-10) == -10, "operator==(int)");
   }

   void test_operator_int_inequality()
   {
      ASSERT(!(BI(10) != 10), "operator!=(int)");
      ASSERT(!(BI(-10) != -10), "operator!=(int)");
   }

   void test_operator_int_less()
   {
      ASSERT(BI(10) < 11, "operator<(int)");
      ASSERT(BI(-10) < -9, "operator<(int)");
   }

   void test_operator_int_greater()
   {
      ASSERT(BI(10) > 9, "operator>(int)");
      ASSERT(BI(-10) > -11, "operator>(int)");
   }

   void test_operator_int_leq()
   {
      ASSERT(BI(10) <= 10, "operator<=(int)");
      ASSERT(BI(10) <= 11, "operator<=(int)");
      ASSERT(BI(-10) <= -10, "operator<=(int)");
      ASSERT(BI(-10) <= -9, "operator<=(int)");
   }

   void test_operator_int_geq()
   {
      ASSERT(BI(10) >= 10, "operator>=(int)");
      ASSERT(BI(-10) >= -10, "operator>=(int)");
      ASSERT(BI(10) >= 9, "operator>=(int)");
      ASSERT(BI(-10) >= -11, "operator>=(int)");
   }

   void test_operator_bigint_equality()
   {
      ASSERT(BI(10) == BI(10), "operator==(BigInteger)");
      ASSERT(BI(-10) == BI(-10), "operator==(BigInteger)");
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      BI b(2);
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      ASSERT(a == b, "operator==(BigInteger)");
   }

   void test_operator_bigint_inequality()
   {
      ASSERT(!(BI(10) != BI(10)), "operator!=(BigInteger)");
      ASSERT(!(BI(-10) != BI(-10)), "operator!=(BigInteger)");
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      BI b(2);
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      b = b * b;
      ASSERT(!(a != b), "operator!=(BigInteger)");
   }

   void test_operator_bigint_less()
   {
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      ASSERT(BI(0) < BI(1), "operator<(BigInteger)");
      ASSERT(BI(1) < BI(2), "operator<(BigInteger)");
      ASSERT(BI(-1) < BI(2), "operator<(BigInteger)");
      ASSERT(BI(-2) < BI(-1), "operator<(BigInteger)");
      ASSERT(BI(-1) < BI(0), "operator<(BigInteger)");
      ASSERT(BI(1) < a, "operator<(BigInteger)");
      ASSERT(BI(0) < a, "operator<(BigInteger)");
      ASSERT(BI(-1) < a, "operator<(BigInteger)");
      a *= BI(-1);
      ASSERT(a < BI(-1), "operator<(BigInteger)");
      ASSERT(a < BI(0), "operator<(BigInteger)");
      ASSERT(a < BI(1), "operator<(BigInteger)");
   }

   void test_operator_bigint_greater()
   {
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      ASSERT(BI(1) > BI(0), "operator<(BigInteger)");
      ASSERT(BI(2) > BI(1), "operator<(BigInteger)");
      ASSERT(BI(2) > BI(-1), "operator<(BigInteger)");
      ASSERT(BI(-1) > BI(-2), "operator<(BigInteger)");
      ASSERT(BI(0) > BI(-1), "operator<(BigInteger)");
      ASSERT(a > BI(-1), "operator<(BigInteger)");
      ASSERT(a > BI(0), "operator<(BigInteger)");
      ASSERT(a > BI(1), "operator<(BigInteger)");
      a *= BI(-1);
      ASSERT(BI(1) > a, "operator<(BigInteger)");
      ASSERT(BI(0) > a, "operator<(BigInteger)");
      ASSERT(BI(-1) > a, "operator<(BigInteger)");
   }

   void test_operator_bigint_leq()
   {
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      ASSERT(BI(0) <= BI(1), "operator<(BigInteger)");
      ASSERT(BI(0) <= BI(0), "operator<(BigInteger)");
      ASSERT(BI(1) <= BI(2), "operator<(BigInteger)");;
      ASSERT(BI(1) <= BI(1), "operator<(BigInteger)");
      ASSERT(BI(-1) <= BI(2), "operator<(BigInteger)");
      ASSERT(BI(-2) <= BI(-1), "operator<(BigInteger)");
      ASSERT(BI(-1) <= BI(0), "operator<(BigInteger)");
      ASSERT(BI(-1) <= BI(-1), "operator<(BigInteger)");
      ASSERT(BI(1) <= a, "operator<(BigInteger)");
      ASSERT(BI(0) <= a, "operator<(BigInteger)");
      ASSERT(BI(-1) <= a, "operator<(BigInteger)");
      ASSERT(a <= a, "operator<(BigInteger)");
      a *= BI(-1);
      ASSERT(a <= BI(-1), "operator<(BigInteger)");
      ASSERT(a <= BI(0), "operator<(BigInteger)");
      ASSERT(a <= BI(1), "operator<(BigInteger)");
      ASSERT(a <= a, "operator<(BigInteger)");
   }

   void test_operator_bigint_geq()
   {
      BI a(2);
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      a = a * a;
      ASSERT(BI(1) >= BI(0), "operator<(BigInteger)");
      ASSERT(BI(0) >= BI(0), "operator<(BigInteger)");
      ASSERT(BI(2) >= BI(1), "operator<(BigInteger)");;
      ASSERT(BI(1) >= BI(1), "operator<(BigInteger)");
      ASSERT(BI(2) >= BI(-1), "operator<(BigInteger)");
      ASSERT(BI(-1) >= BI(-2), "operator<(BigInteger)");
      ASSERT(BI(0) >= BI(-1), "operator<(BigInteger)");
      ASSERT(BI(-1) >= BI(-1), "operator<(BigInteger)");
      ASSERT(a >= BI(-1), "operator<(BigInteger)");
      ASSERT(a >= BI(0), "operator<(BigInteger)");
      ASSERT(a >= BI(1), "operator<(BigInteger)");
      ASSERT(a >= a, "operator<(BigInteger)");
      a *= BI(-1);
      ASSERT(BI(1) >= a, "operator<(BigInteger)");
      ASSERT(BI(0) >= a, "operator<(BigInteger)");
      ASSERT(BI(-1) >= a, "operator<(BigInteger)");
      ASSERT(a >= a, "operator<(BigInteger)");
   }
}

