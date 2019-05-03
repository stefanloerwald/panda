
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <cstdint>

namespace panda
{
   class SafeInteger;

   /// Absolute value.
   inline SafeInteger abs(SafeInteger);

   class SafeInteger
   {
      public:
         #pragma GCC diagnostic push
         #pragma GCC diagnostic ignored "-Weffc++"
         /// Default constructor (value zero).
         SafeInteger() = default;
         #pragma GCC diagnostic pop
         /// Default copy constructor.
         SafeInteger(const SafeInteger&) = default;
         /// Default move constructor.
         SafeInteger(SafeInteger&&) = default;
         /// Default copy assignment operator.
         SafeInteger& operator=(const SafeInteger&) = default;
         /// Default move assignment operator.
         SafeInteger& operator=(SafeInteger&&) = default;
         #ifdef INT16_MAX
         /// Constructor from int16_t.
         explicit inline SafeInteger(const int16_t);
         #endif
         #ifdef INT32_MAX
         /// Constructor from int32_t.
         explicit inline SafeInteger(const int32_t);
         #endif
         #ifdef INT64_MAX
         /// Constructor from int64_t.
         explicit inline SafeInteger(const int64_t);
         #endif
         #ifdef UINT16_MAX
         /// Constructor from uint16_t.
         explicit inline SafeInteger(const uint16_t);
         #endif
         #ifdef UINT32_MAX
         /// Constructor from uint32_t.
         explicit inline SafeInteger(const uint32_t);
         #endif
         #ifdef UINT64_MAX
         /// Constructor from uint64_t.
         explicit inline SafeInteger(const uint64_t);
         #endif
         /// Conversion to int.
         inline operator int() const;
         /// Comparison "equals" with integer.
         inline bool operator==(const int) const noexcept;
         /// Comparison "not equals" with integer.
         inline bool operator!=(const int) const noexcept;
         /// Comparison "less than" with integer.
         inline bool operator<(const int) const noexcept;
         /// Comparison "greater than" with integer.
         inline bool operator>(const int) const noexcept;
         /// Comparison "less than or equal to" with integer.
         inline bool operator<=(const int) const noexcept;
         /// Comparison "greater than or equal to" with integer.
         inline bool operator>=(const int) const noexcept;
         /// Comparison "equals" with BigInteger.
         inline bool operator==(const SafeInteger&) const noexcept;
         /// Comparison "not equals" with BigInteger.
         inline bool operator!=(const SafeInteger&) const noexcept;
         /// Comparison "less than" with BigInteger.
         inline bool operator<(const SafeInteger&) const noexcept;
         /// Comparison "greater than" with BigInteger.
         inline bool operator>(const SafeInteger&) const noexcept;
         /// Comparison "less than or equal to" with BigInteger.
         inline bool operator<=(const SafeInteger&) const noexcept;
         /// Comparison "greater than or equal to" with BigInteger.
         inline bool operator>=(const SafeInteger&) const noexcept;
         /// Multiplication (a *= b).
         inline SafeInteger& operator*=(const SafeInteger&);
         /// Division (a /= b).
         inline SafeInteger& operator/=(const SafeInteger&);
         /// Addition (a += b).
         inline SafeInteger& operator+=(const SafeInteger&);
         /// Subtraction (a -= b).
         inline SafeInteger& operator-=(const SafeInteger&);
         /// Modulo (a %= b).
         inline SafeInteger& operator%=(const SafeInteger&);
         /// Multiplication (a * b).
         inline SafeInteger operator*(const SafeInteger&) const;
         /// Division (a / b).
         inline SafeInteger operator/(const SafeInteger&) const;
         /// Addition (a + b).
         inline SafeInteger operator+(const SafeInteger&) const;
         /// Subtraction (a - b).
         inline SafeInteger operator-(const SafeInteger&) const;
         /// Modulo (a % b).
         inline SafeInteger operator%(const SafeInteger&) const;
         /// Negation (-a).
         inline SafeInteger operator-() const;
         /// Absolute value.
         friend SafeInteger abs(SafeInteger);
      public:
         /// Underlying data type.
         using DataType = int64_t;
      private:
         DataType data;
   };
}

#include "safe_integer.tpp"

