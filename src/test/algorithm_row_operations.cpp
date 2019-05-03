
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_row_operations.h"

#include <sstream>
#include <stdexcept>

using namespace panda;

namespace
{
   void output();
   void output_named();
   void addition();
   void multiplication();
   void subtraction();
   void division();
   void scalarProduct();
   void normalization();
   void gcd();
}

int main()
try
{
   output();
   output_named();
   addition();
   multiplication();
   subtraction();
   division();
   scalarProduct();
   normalization();
   gcd();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void output()
   {
      std::stringstream stream;
      Row<int> r{0, 1, 2, 0, -1, -2};
      stream << r;
      int t;
      ASSERT(stream >> t && t == 0, "Mismatch.");
      ASSERT(stream >> t && t == 1, "Mismatch.");
      ASSERT(stream >> t && t == 2, "Mismatch.");
      ASSERT(stream >> t && t == 0, "Mismatch.");
      ASSERT(stream >> t && t == -1, "Mismatch.");
      ASSERT(stream >> t && t == -2, "Mismatch.");
      ASSERT(!(stream >> t), "End of data.");
   }

   void output_named()
   {
      Names names{"x1", "y2", "z3"};
      for ( const auto pair : {std::make_pair(Row<int>{0, 1, -2, -1}, "y2 -2z3 <= 1"),
                               std::make_pair(Row<int>{1, 1, -2, -1}, "x1 +y2 -2z3 <= 1"),
                               std::make_pair(Row<int>{1, 1, -1, -1}, "x1 +y2 -z3 <= 1"),
                               std::make_pair(Row<int>{1, 2, -1, 0}, "x1 +2y2 -z3 <= 0"),
                               std::make_pair(Row<int>{-1, 2, -1, 0}, "-x1 +2y2 -z3 <= 0"),
                               std::make_pair(Row<int>{-2, 2, -1, 0}, "-2x1 +2y2 -z3 <= 0")} )
      {
         std::stringstream stream;
         algorithm::prettyPrint(stream, pair.first, names, "<=");
         ASSERT(stream.str() == pair.second, "Unexpected result.");
      }
   }

   void addition()
   {
      Row<int> r0{0, 1, 2, 0, -1, -2};
      Row<int> r1{0, -1, -2, 0, 1, 2};
      const auto r2 = r0 + r1;
      ASSERT((r2 == Row<int>{0, 0, 0, 0, 0, 0}), "+ failed.");
      r0 += r1;
      ASSERT((r0 == Row<int>{0, 0, 0, 0, 0, 0}), "+= failed.");
   }

   void subtraction()
   {
      Row<int> r0{0, 1, 2, 0, -1, -2};
      Row<int> r1{-1, 0, 1, -1, -2, -3};
      const auto r2 = r0 - r1;
      ASSERT((r2 == Row<int>{1, 1, 1, 1, 1, 1}), "- failed.");
      r0 -= r1;
      ASSERT((r0 == Row<int>{1, 1, 1, 1, 1, 1}), "-= failed.");
   }

   void multiplication()
   {
      Row<int> r{0, 1, 2, 0, -1, -2};
      ASSERT((2 * r == Row<int>{0, 2, 4, 0, -2, -4}), "* failed (integer as first argument.");
      ASSERT((r * 2 == Row<int>{0, 2, 4, 0, -2, -4}), "* failed (integer as second argument.");
      r *= 2;
      ASSERT((r == Row<int>{0, 2, 4, 0, -2, -4}), "*= failed.");
   }

   void division()
   {
      Row<int> r{0, 2, 4, 0, -2, -4};
      ASSERT((r / 2 == Row<int>{0, 1, 2, 0, -1, -2}), "/ failed.");
      r /= 2;
      ASSERT((r == Row<int>{0, 1, 2, 0, -1, -2}), "/= failed.");
      ASSERT_EXCEPTION(r / 0, std::invalid_argument, "Divison by 0 must fail.");
      ASSERT_EXCEPTION(r /= 0, std::invalid_argument, "Divison by 0 must fail.");
   }

   void scalarProduct()
   {
      Row<int> r0{0, 0, 1};
      Row<int> r1{0, 1, 0};
      Row<int> r2{1, 2, 3};
      Row<int> r3{4, 5, 6};
      ASSERT(r0 * r1 == 0, "scalar product failed");
      ASSERT(r0 * r2 == 3, "scalar product failed");
      ASSERT(r1 * r2 == 2, "scalar product failed");
      ASSERT(r2 * r3 == 32, "scalar product failed");
   }

   void normalization()
   {
      Row<int> r{0, 1, 2, 0};
      const auto message = "normalization with empty list of equations may not modify except for gcd.";
      ASSERT((r == panda::algorithm::normalize(r, {})), message);
      Equations<int> eqs{{1, 0, 0, 1}, {0, 1, 1, 0}};
      ASSERT((panda::algorithm::normalize(r, eqs) == Row<int>{0, 0, 1, 0}), "Normalization invalid.");
      ASSERT((r == Row<int>{0, 1, 2, 0}), "Normalization modified row.");
   }

   void gcd()
   {
      Row<int> r{0, 2, 4, 0, -2, -4};
      ASSERT(algorithm::gcd(r) == 2, "range gcd invalid value.");
      ASSERT((r == Row<int>{0, 2, 4, 0, -2, -4}), "gcd modified value.");
   }
}

