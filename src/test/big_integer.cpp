
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "big_integer.h"

using namespace panda;

namespace
{
   void test_operator_unary_minus();
   void test_abs();
}

int main()
try
{
   test_operator_unary_minus();
   test_abs();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using BI = BigInteger;

   void test_operator_unary_minus()
   {
      ASSERT(-BI(0) == BI(0), "operator-()");
      ASSERT(-BI(1) == BI(-1), "operator-()");
      ASSERT(-BI(-1) == BI(1), "operator-()");
   }

   void test_abs()
   {
      ASSERT(abs(BI(0)) == BI(0), "abs(BigInteger)");
      ASSERT(abs(BI(1)) == BI(1), "abs(BigInteger)");
      ASSERT(abs(BI(-1)) == BI(1), "abs(BigInteger)");
   }
}

