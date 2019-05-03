
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "big_integer.h"

using namespace panda;

namespace
{
   void test_operator_multiply_assign();
   void test_operator_divide_assign();
   void test_operator_add_assign();
   void test_operator_subtract_assign();
   void test_operator_modulo_assign();
   void test_operator_multiply();
   void test_operator_divide();
   void test_operator_add();
   void test_operator_subtract();
   void test_operator_modulo();
}

int main()
try
{
   test_operator_multiply_assign();
   test_operator_divide_assign();
   test_operator_add_assign();
   test_operator_subtract_assign();
   test_operator_modulo_assign();
   test_operator_multiply();
   test_operator_divide();
   test_operator_add();
   test_operator_subtract();
   test_operator_modulo();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using BI = BigInteger;

   void test_operator_multiply_assign()
   {
      ASSERT(((BI(2) *= BI(3)) == BI(6)), "operator*=(BigInteger)");
      ASSERT(((BI(2) *= BI(-3)) == BI(-6)), "operator*=(BigInteger)");
      ASSERT(((BI(-2) *= BI(3)) == BI(-6)), "operator*=(BigInteger)");
      ASSERT(((BI(-2) *= BI(-3)) == BI(6)), "operator*=(BigInteger)");
      ASSERT(((BI(0) *= BI(3)) == BI(0)), "operator*=(BigInteger)");
      ASSERT(((BI(2) *= BI(0)) == BI(0)), "operator*=(BigInteger)");
   }

   void test_operator_divide_assign()
   {
      ASSERT(((BI(6) /= BI(3)) == BI(2)), "operator/=(BigInteger)");
      ASSERT(((BI(6) /= BI(-3)) == BI(-2)), "operator/=(BigInteger)");
      ASSERT(((BI(-6) /= BI(3)) == BI(-2)), "operator/=(BigInteger)");
      ASSERT(((BI(-6) /= BI(-3)) == BI(2)), "operator/=(BigInteger)");
      ASSERT(((BI(0) /= BI(3)) == BI(0)), "operator/=(BigInteger)");
      ASSERT_ANY_EXCEPTION(((BI(6) /= BI(0)) == BI(0)), "operator/=(BigInteger)");
   }

   void test_operator_add_assign()
   {
      ASSERT(((BI(6) += BI(3)) == BI(9)), "operator+=(BigInteger)");
      ASSERT(((BI(6) += BI(-3)) == BI(3)), "operator+=(BigInteger)");
      ASSERT(((BI(-6) += BI(3)) == BI(-3)), "operator+=(BigInteger)");
      ASSERT(((BI(-6) += BI(-3)) == BI(-9)), "operator+=(BigInteger)");
      ASSERT(((BI(0) += BI(3)) == BI(3)), "operator+=(BigInteger)");
      ASSERT(((BI(6) += BI(0)) == BI(6)), "operator+=(BigInteger)");
   }

   void test_operator_subtract_assign()
   {
      ASSERT(((BI(6) -= BI(3)) == BI(3)), "operator-=(BigInteger)");
      ASSERT(((BI(6) -= BI(-3)) == BI(9)), "operator-=(BigInteger)");
      ASSERT(((BI(-6) -= BI(3)) == BI(-9)), "operator-=(BigInteger)");
      ASSERT(((BI(-6) -= BI(-3)) == BI(-3)), "operator-=(BigInteger)");
      ASSERT(((BI(0) -= BI(3)) == BI(-3)), "operator-=(BigInteger)");
      ASSERT(((BI(6) -= BI(0)) == BI(6)), "operator-=(BigInteger)");
   }

   void test_operator_modulo_assign()
   {
      ASSERT(((BI(9) %= BI(7)) == BI(2)), "operator%=(BigInteger)");
      ASSERT(((BI(-9) %= BI(7)) == BI(5)), "operator%=(BigInteger)");
      ASSERT(((BI(0) %= BI(7)) == BI(0)), "operator%=(BigInteger)");
      ASSERT_ANY_EXCEPTION(((BI(9) %= BI(0)) == BI(0)), "operator%=(BigInteger)");
   }

   void test_operator_multiply()
   {
      ASSERT(((BI(2) * BI(3)) == BI(6)), "operator*(BigInteger)");
      ASSERT(((BI(2) * BI(-3)) == BI(-6)), "operator*(BigInteger)");
      ASSERT(((BI(-2) * BI(3)) == BI(-6)), "operator*(BigInteger)");
      ASSERT(((BI(-2) * BI(-3)) == BI(6)), "operator*(BigInteger)");
      ASSERT(((BI(0) * BI(3)) == BI(0)), "operator*(BigInteger)");
      ASSERT(((BI(2) * BI(0)) == BI(0)), "operator*(BigInteger)");
   }

   void test_operator_divide()
   {
      ASSERT(((BI(6) / BI(3)) == BI(2)), "operator/(BigInteger)");
      ASSERT(((BI(6) / BI(-3)) == BI(-2)), "operator/(BigInteger)");
      ASSERT(((BI(-6) / BI(3)) == BI(-2)), "operator/(BigInteger)");
      ASSERT(((BI(-6) / BI(-3)) == BI(2)), "operator/(BigInteger)");
      ASSERT(((BI(0) / BI(3)) == BI(0)), "operator/(BigInteger)");
      ASSERT_ANY_EXCEPTION(((BI(6) / BI(0)) == BI(0)), "operator/(BigInteger)");
   }

   void test_operator_add()
   {
      ASSERT(((BI(6) + BI(3)) == BI(9)), "operator+(BigInteger)");
      ASSERT(((BI(6) + BI(-3)) == BI(3)), "operator+(BigInteger)");
      ASSERT(((BI(-6) + BI(3)) == BI(-3)), "operator+(BigInteger)");
      ASSERT(((BI(-6) + BI(-3)) == BI(-9)), "operator+(BigInteger)");
      ASSERT(((BI(0) + BI(3)) == BI(3)), "operator+(BigInteger)");
      ASSERT(((BI(6) + BI(0)) == BI(6)), "operator+(BigInteger)");
   }

   void test_operator_subtract()
   {
      ASSERT(((BI(6) - BI(3)) == BI(3)), "operator-(BigInteger)");
      ASSERT(((BI(6) - BI(-3)) == BI(9)), "operator-(BigInteger)");
      ASSERT(((BI(-6) - BI(3)) == BI(-9)), "operator-(BigInteger)");
      ASSERT(((BI(-6) - BI(-3)) == BI(-3)), "operator-(BigInteger)");
      ASSERT(((BI(0) - BI(3)) == BI(-3)), "operator-(BigInteger)");
      ASSERT(((BI(6) - BI(0)) == BI(6)), "operator-(BigInteger)");
   }

   void test_operator_modulo()
   {
      ASSERT(((BI(9) % BI(7)) == BI(2)), "operator%(BigInteger)");
      ASSERT(((BI(-9) % BI(7)) == BI(5)), "operator%(BigInteger)");
      ASSERT(((BI(0) % BI(7)) == BI(0)), "operator%(BigInteger)");
      ASSERT_ANY_EXCEPTION(((BI(9) % BI(0)) == BI(0)), "operator%(BigInteger)");
   }
}

