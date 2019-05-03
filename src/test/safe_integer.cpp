
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "safe_integer.h"

#include <utility>
#include <vector>

using namespace panda;

namespace
{
   void test_constructor_int16_t();
   void test_constructor_int32_t();
   void test_constructor_int64_t();
   void test_constructor_uint16_t();
   void test_constructor_uint32_t();
   void test_constructor_uint64_t();
   void test_operator_multiply_assign();
   void test_operator_divide_assign();
   void test_operator_add_assign();
   void test_operator_subtract_assign();
   void test_operator_modulo_assign();
   void test_operator_int();
   void test_operator_unary_minus();
}

int main()
try
{
   test_constructor_int16_t();
   test_constructor_int32_t();
   test_constructor_int64_t();
   test_constructor_uint16_t();
   test_constructor_uint32_t();
   test_constructor_uint64_t();
   test_operator_multiply_assign();
   test_operator_divide_assign();
   test_operator_add_assign();
   test_operator_subtract_assign();
   test_operator_modulo_assign();
   test_operator_int();
   test_operator_unary_minus();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using SI = SafeInteger;
   using SID = SafeInteger::DataType;
   constexpr static SID SID_MIN = std::numeric_limits<SID>::min();
   constexpr static SID SID_MAX = std::numeric_limits<SID>::max();

   constexpr static auto SID_RANGE_IS_ASYMMETRIC = (SID_MIN + SID_MAX != 0);
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wtype-limits"
   template <typename Integer>
   void test_constructor_signed()
   {
      if ( SID_MIN > std::numeric_limits<Integer>::min() )
      {
         ASSERT_ANY_EXCEPTION(SI(std::numeric_limits<Integer>::min()), "Range too small.");
      }
      else
      {
         ASSERT_NOTHROW(SI(std::numeric_limits<Integer>::min()), "Range big enough.");
      }
      if ( SID_MAX < std::numeric_limits<Integer>::max() )
      {
         ASSERT_ANY_EXCEPTION(SI(std::numeric_limits<Integer>::max()), "Range too small.");
      }
      else
      {
         ASSERT_NOTHROW(SI(std::numeric_limits<Integer>::max()), "Range big enough.");
      }
   }

   void test_constructor_int16_t()
   {
      test_constructor_signed<int16_t>();
   }

   void test_constructor_int32_t()
   {
      test_constructor_signed<int32_t>();
   }

   void test_constructor_int64_t()
   {
      test_constructor_signed<int64_t>();
   }

   template <typename Integer>
   void test_constructor_unsigned()
   {
      if ( static_cast<Integer>(SID_MAX) < std::numeric_limits<Integer>::max() )
      {
         ASSERT_ANY_EXCEPTION(SI(std::numeric_limits<Integer>::max()), "Range too small.");
      }
      else
      {
         ASSERT_NOTHROW(SI(std::numeric_limits<Integer>::max()), "Range big enough.");
      }
   }

   void test_constructor_uint16_t()
   {
      test_constructor_unsigned<uint16_t>();
   }

   void test_constructor_uint32_t()
   {
      test_constructor_unsigned<uint32_t>();
   }

   void test_constructor_uint64_t()
   {
      test_constructor_unsigned<uint64_t>();
   }
   #pragma GCC diagnostic pop

   void test_operator_multiply_assign()
   {
      std::vector<std::pair<SID, SID>> good{std::make_pair(SID_MIN, 0),
                                            std::make_pair(SID_MIN, 1),
                                            std::make_pair(-2, -2),
                                            std::make_pair(-2, -1),
                                            std::make_pair(-2, 0),
                                            std::make_pair(-2, 1),
                                            std::make_pair(-2, 2),
                                            std::make_pair(-1, -2),
                                            std::make_pair(-1, -1),
                                            std::make_pair(-1, 0),
                                            std::make_pair(-1, 1),
                                            std::make_pair(-1, 2),
                                            std::make_pair(-1, SID_MAX),
                                            std::make_pair(0, SID_MIN),
                                            std::make_pair(0, -2),
                                            std::make_pair(0, -1),
                                            std::make_pair(0, 0),
                                            std::make_pair(0, 1),
                                            std::make_pair(0, 2),
                                            std::make_pair(0, SID_MAX),
                                            std::make_pair(1, SID_MIN),
                                            std::make_pair(1, -2),
                                            std::make_pair(1, -1),
                                            std::make_pair(1, 0),
                                            std::make_pair(1, 1),
                                            std::make_pair(1, 2),
                                            std::make_pair(1, SID_MAX),
                                            std::make_pair(2, -2),
                                            std::make_pair(2, -1),
                                            std::make_pair(2, 0),
                                            std::make_pair(2, 1),
                                            std::make_pair(2, 2),
                                            std::make_pair(SID_MAX, -1),
                                            std::make_pair(SID_MAX, 0),
                                            std::make_pair(SID_MAX, 1)};
      for ( const auto p : good )
      {
         SI a(p.first);
         SI b(p.second);
         ASSERT_NOTHROW(a *= b, "Valid multiplication: " + std::to_string(p.first) + " *= " + std::to_string(p.second) + ".");
      }
      std::vector<std::pair<SID, SID>> bad{std::make_pair(SID_MIN, SID_MIN),
                                           std::make_pair(SID_MIN, -2),
                                           std::make_pair(SID_MIN, 2),
                                           std::make_pair(SID_MIN, SID_MAX),
                                           std::make_pair(-2, SID_MIN),
                                           std::make_pair(-2, SID_MAX),
                                           std::make_pair(2, SID_MIN),
                                           std::make_pair(2, SID_MAX),
                                           std::make_pair(SID_MAX, SID_MIN),
                                           std::make_pair(SID_MAX, -2),
                                           std::make_pair(SID_MAX, 2),
                                           std::make_pair(SID_MAX, SID_MAX)};
      for ( const auto p : bad )
      {
         SI a(p.first);
         SI b(p.second);
         ASSERT_ANY_EXCEPTION(a *= b, "Invalid multiplication: " + std::to_string(p.first) + " *= " + std::to_string(p.second) + ".");
      }
      if ( SID_RANGE_IS_ASYMMETRIC )
      {
         ASSERT_ANY_EXCEPTION(SI(SID_MIN) *= SI(-1), "Invalid multiplication: " + std::to_string(SID_MIN) + " *= -1.");
         ASSERT_ANY_EXCEPTION(SI(-1) *= SI(SID_MIN), "Invalid multiplication: -1 *= " + std::to_string(SID_MIN) + ".");
      }
   }

   void test_operator_divide_assign()
   {
      std::vector<SID> values{SID_MIN, -2, -1, 0, 1, 2, SID_MAX};
      for ( auto a : values )
      {
         for ( auto b : values )
         {
            SI sa(a);
            SI sb(b);
            if ( b == 0 )
            {
               ASSERT_ANY_EXCEPTION(sa /= sb, "Division of " + std::to_string(a) + " by zero.");
            }
            else if ( SID_RANGE_IS_ASYMMETRIC && (a == SID_MIN) && (b == -1) )
            {
               ASSERT(sa == SI(SID_MIN), "");
               ASSERT_ANY_EXCEPTION(-sa, "Division of " + std::to_string(a) + " by " + std::to_string(b) + " is invalid.");
               ASSERT_ANY_EXCEPTION(sa /= sb, "Division of " + std::to_string(a) + " by " + std::to_string(b) + " is invalid.");
            }
            else
            {
               ASSERT_NOTHROW(sa /= sb, "Division of " + std::to_string(a) + " by " + std::to_string(b) + " is valid.");
            }
         }
      }
   }

   void test_operator_add_assign()
   {
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) += SI(SID_MIN), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) += SI(-2), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(SID_MIN) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) += SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) += SI(SID_MAX), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(-2) += SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(-2) += SI(-2), "Addition invalid.");
      ASSERT_NOTHROW(SI(-2) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(-2) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) += SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) += SI(SID_MAX), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(-1) += SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(-1) += SI(-2), "Addition invalid.");
      ASSERT_NOTHROW(SI(-1) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(-1) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) += SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) += SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(0) += SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(0) += SI(-2), "Addition invalid.");
      ASSERT_NOTHROW(SI(0) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(0) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(0) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(0) += SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(0) += SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(1) += SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(1) += SI(-2), "Addition invalid.");
      ASSERT_NOTHROW(SI(1) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(1) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(1) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(1) += SI(2), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(2) += SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(2) += SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(2) += SI(-2), "Addition invalid.");
      ASSERT_NOTHROW(SI(2) += SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(2) += SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(2) += SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(2) += SI(2), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(2) += SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) += SI(SID_MIN), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) += SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) += SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) += SI(0), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) += SI(1), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) += SI(2), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) += SI(SID_MAX), "Addition invalid.");
   }

   void test_operator_subtract_assign()
   {
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) -= SI(SID_MIN), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) -= SI(-2), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MAX) -= SI(-1), "Addition invalid.");
      ASSERT_NOTHROW(SI(SID_MAX) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) -= SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MAX) -= SI(SID_MAX), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(2) -= SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(2) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(2) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(2) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(2) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(2) -= SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(2) -= SI(SID_MAX), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(1) -= SI(SID_MIN), "Addition invalid.");
      ASSERT_NOTHROW(SI(1) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(1) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(1) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(1) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(1) -= SI(2), "Addition valid.");
      ASSERT_NOTHROW(SI(1) -= SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(SID_MAX), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(0) -= SI(2), "Addition valid.");
      if ( SID_RANGE_IS_ASYMMETRIC )
      {
         ASSERT_ANY_EXCEPTION(SI(0) -= SI(SID_MIN), "Addition invalid.");
      }
      else
      {
         ASSERT_NOTHROW(SI(0) -= SI(SID_MIN), "Addition valid.");
      }
      ASSERT_NOTHROW(SI(-1) -= SI(SID_MIN), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(-1) -= SI(2), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(-2) -= SI(SID_MAX), "Addition invalid.");
      ASSERT_NOTHROW(SI(-2) -= SI(SID_MIN), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) -= SI(0), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) -= SI(1), "Addition valid.");
      ASSERT_NOTHROW(SI(-2) -= SI(2), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(-2) -= SI(SID_MAX), "Addition invalid.");
      ASSERT_NOTHROW(SI(SID_MIN) -= SI(SID_MIN), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) -= SI(-2), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) -= SI(-1), "Addition valid.");
      ASSERT_NOTHROW(SI(SID_MIN) -= SI(0), "Addition valid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) -= SI(1), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) -= SI(2), "Addition invalid.");
      ASSERT_ANY_EXCEPTION(SI(SID_MIN) -= SI(SID_MAX), "Addition invalid.");
   }

   void test_operator_modulo_assign()
   {
      std::vector<SID> bad{SID_MIN, -2, -1, 0};
      std::vector<SID> good{1, 2, SID_MAX};
      std::vector<SID> all{SID_MIN, -2, -1, 0, 1, 2, SID_MAX};
      for ( auto a : all )
      {
         for ( auto b : bad )
         {
            SI sa(a);
            SI sb(b);
            ASSERT_ANY_EXCEPTION(sa %= sb, "Right hand side of modulo operation invalid.");
         }
         for ( auto b : good )
         {
            SI sa(a);
            SI sb(b);
            ASSERT_NOTHROW(sa %= sb, "Modulo operation valid.");
         }
      }
   }

   void test_operator_int()
   {
      constexpr static auto int_min = std::numeric_limits<int>::min();
      constexpr static auto int_max = std::numeric_limits<int>::max();
      std::vector<SID> values{SID_MIN, int_min, -2, -1, 0, 1, 2, int_max, SID_MAX};
      for ( auto a : values )
      {
         if ( a < int_min || int_max < a )
         {
            ASSERT_ANY_EXCEPTION(static_cast<int>(SI(a)), "Conversion of out-of-range-value is invalid.");
         }
         else
         {
            ASSERT_NOTHROW(static_cast<int>(SI(a)), "Conversion of in-of-range-value is valid.");
         }
      }
   }

   void test_operator_unary_minus()
   {
      ASSERT(-SI(0) == SI(0), "operator-()");
      ASSERT(-SI(1) == SI(-1), "operator-()");
      ASSERT(-SI(-1) == SI(1), "operator-()");
      if ( SID_RANGE_IS_ASYMMETRIC )
      {
         ASSERT_ANY_EXCEPTION(-SI(std::numeric_limits<SID>::min()), "operator-() has to throw on invalid argument.");
      }
   }
}

