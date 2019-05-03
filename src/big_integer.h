
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <vector>

namespace panda
{
   class BigInteger;

   /// Absolute value.
   BigInteger abs(BigInteger) noexcept;

   class BigInteger
   {
      public:
         /// Default constructor (value zero).
         BigInteger();
         #ifdef INT16_MAX
         /// Constructor from int16_t.
         explicit BigInteger(const int16_t);
         #endif
         #ifdef INT32_MAX
         /// Constructor from int32_t.
         explicit BigInteger(const int32_t);
         #endif
         #ifdef INT64_MAX
         /// Constructor from int64_t.
         explicit BigInteger(const int64_t);
         #endif
         #ifdef UINT16_MAX
         /// Constructor from uint16_t.
         explicit BigInteger(const uint16_t);
         #endif
         #ifdef UINT32_MAX
         /// Constructor from uint32_t.
         explicit BigInteger(const uint32_t);
         #endif
         #ifdef UINT64_MAX
         /// Constructor from uint64_t.
         explicit BigInteger(const uint64_t);
         #endif
         /// Conversion to int.
         operator int() const;
         /// Comparison "equals" with integer.
         bool operator==(const int) const noexcept;
         /// Comparison "not equals" with integer.
         bool operator!=(const int) const noexcept;
         /// Comparison "less than" with integer.
         bool operator<(const int) const noexcept;
         /// Comparison "greater than" with integer.
         bool operator>(const int) const noexcept;
         /// Comparison "less than or equal to" with integer.
         bool operator<=(const int) const noexcept;
         /// Comparison "greater than or equal to" with integer.
         bool operator>=(const int) const noexcept;
         /// Comparison "equals" with BigInteger.
         bool operator==(const BigInteger&) const noexcept;
         /// Comparison "not equals" with BigInteger.
         bool operator!=(const BigInteger&) const noexcept;
         /// Comparison "less than" with BigInteger.
         bool operator<(const BigInteger&) const noexcept;
         /// Comparison "greater than" with BigInteger.
         bool operator>(const BigInteger&) const noexcept;
         /// Comparison "less than or equal to" with BigInteger.
         bool operator<=(const BigInteger&) const noexcept;
         /// Comparison "greater than or equal to" with BigInteger.
         bool operator>=(const BigInteger&) const noexcept;
         /// Multiplication (a *= b).
         BigInteger& operator*=(const BigInteger&);
         /// Division (a /= b).
         BigInteger& operator/=(const BigInteger&);
         /// Addition (a += b).
         BigInteger& operator+=(const BigInteger&);
         /// Subtraction (a -= b).
         BigInteger& operator-=(const BigInteger&);
         /// Modulo (a %= b).
         BigInteger& operator%=(const BigInteger&);
         /// Multiplication (a * b).
         BigInteger operator*(const BigInteger&) const;
         /// Division (a / b).
         BigInteger operator/(const BigInteger&) const;
         /// Addition (a + b).
         BigInteger operator+(const BigInteger&) const;
         /// Subtraction (a - b).
         BigInteger operator-(const BigInteger&) const;
         /// Modulo (a % b).
         BigInteger operator%(const BigInteger&) const;
         /// Negation (-a).
         BigInteger operator-() const;
         /// Absolute value.
         friend BigInteger abs(BigInteger) noexcept;
      private:
         /// Underlying data type.
         using DataType = uint_fast32_t;
         /// Sign of the number (Positive means >= 0, negative means < 0).
         enum class Sign
         {
            Positive,
            Negative
         };
         Sign sign;
         using Magnitude = std::vector<DataType>;
         Magnitude data;
      private:
         /// Left shift.
         BigInteger& operator<<=(const std::size_t);
         /// Right shift.
         BigInteger& operator>>=(const std::size_t);
         /// Left shift.
         BigInteger operator<<(const std::size_t);
         /// Right shift.
         BigInteger operator>>(const std::size_t);
         /// Division of magnitudes (sign independent).
         BigInteger divideMagnitudesWithRemainder(const BigInteger&);
         /// Addition of magnitudes (sign independent).
         BigInteger& addMagnitude(const Magnitude&);
         /// Subtraction of magnitudes (sign independent).
         BigInteger& subtractMagnitude(const Magnitude&);
         /// Returns true if a < 0.
         bool isNegative() const noexcept;
         /// Returns true if a > 0.
         bool isPositive() const noexcept;
         /// Returns true if a == 0.
         bool isZero() const noexcept;
         /// Comparison "less than" for magnitudes.
         bool isMagnitudeSmallerThan(const BigInteger&) const;
         /// Comparison "greater than" for magnitudes.
         bool isMagnitudeGreaterThan(const BigInteger&) const;
         /// Inverting the number.
         void flipSign();
         /// Adjusting storage size.
         void shrinkToFit();
         /// Sets the value to zero.
         void setZero();
   };
}

