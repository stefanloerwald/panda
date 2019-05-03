
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "big_integer.h"

#include <cassert>
#include <limits>
#include <stdexcept>

using namespace panda;

namespace
{
   using DataLimits = std::numeric_limits<int>;
   /// Tries to do multiplication with integers instead of BigIntegers.
   BigInteger& fallbackMultiplyAssign(BigInteger&, const BigInteger&);
   /// Tries to do division with integers instead of BigIntegers.
   BigInteger& fallbackDivideAssign(BigInteger&, const BigInteger&);
   /// Tries to do addition with integers instead of BigIntegers.
   BigInteger& fallbackAddAssign(BigInteger&, const BigInteger&);
   /// Tries to do subtraction with integers instead of BigIntegers.
   BigInteger& fallbackSubtractAssign(BigInteger&, const BigInteger&);
   /// Tries to do modulo operation with integers instead of BigIntegers.
   BigInteger& fallbackModuloAssign(BigInteger&, const BigInteger&);
}

BigInteger& panda::BigInteger::operator*=(const BigInteger& second)
{
   if ( isZero() || second.isZero() )
   {
      this->setZero();
      return *this;
   }
   if ( second == 1 || second == -1 )
   {
      if ( second.isNegative() )
      {
         flipSign();
      }
      return *this;
   }
   try
   {
      return fallbackMultiplyAssign(*this, second);
   }
   catch ( ... ) // catch failed attempt of falling back to integer operation
   {
      auto copy = *this;
      data = {0};
      for ( const auto& entry : second.data )
      {
         DataType mask = 1;
         for ( int j = 0; j < std::numeric_limits<DataType>::digits; ++j, copy <<= 1, mask <<= 1 )
         {
            if ( (entry & mask) != 0 )
            {
               *this += copy;
            }
         }
      }
      if ( second.sign == Sign::Negative )
      {
         flipSign();
      }
      return *this;
   }
}

BigInteger& panda::BigInteger::operator/=(const BigInteger& second)
{
   if ( second.isZero() )
   {
      throw std::invalid_argument("Integer division by 0 in \"BigInteger::operator/\".");
   }
   if ( isZero() || second == 1 || second == -1 )
   {
      if ( second.isNegative() )
      {
         flipSign();
      }
      return *this;
   }
   try
   {
      return fallbackDivideAssign(*this, second);
   }
   catch ( ... ) // catch failed attempt of falling back to integer operation
   {
      const auto original_sign = sign;
      auto r = divideMagnitudesWithRemainder(second);
      if ( second.sign != original_sign )
      {
         flipSign();
      }
      return *this;
   }
}

BigInteger& panda::BigInteger::operator+=(const BigInteger& second)
{
   if ( second.isZero() )
   {
      return *this;
   }
   try
   {
      return fallbackAddAssign(*this, second);
   }
   catch ( ... ) // catch failed attempt of falling back to integer operation
   {
      if ( sign == second.sign )
      {
         return addMagnitude(second.data);
      }
      const auto old_sign = sign;
      sign = second.sign; // swapping sign (tmp only)
      if ( *this == second )
      {
         this->setZero();
         return *this;
      }
      if ( (second.sign == Sign::Positive) == (*this > second) )
      {
         sign = old_sign; // restoring sign
         return subtractMagnitude(second.data);
      }
      const auto copy = data;
      data = second.data;
      return subtractMagnitude(copy);
   }
}

BigInteger& panda::BigInteger::operator-=(const BigInteger& second)
{
   if ( second.isZero() )
   {
      return *this;
   }
   try
   {
      return fallbackSubtractAssign(*this, second);
   }
   catch ( ... ) // catch failed attempt of falling back to integer operation
   {
      if ( sign != second.sign )
      {
         return addMagnitude(second.data);
      }
      if ( (sign == Sign::Negative) == (*this > second) )
      {
         const auto copy = data;
         data = second.data;
         flipSign();
         return subtractMagnitude(copy);
      }
      return subtractMagnitude(second.data);
   }
}

BigInteger& panda::BigInteger::operator%=(const BigInteger& second)
{
   if ( !second.isPositive() )
   {
      throw std::invalid_argument("Modulo in BigInteger is only defined for positive numbers.");
   }
   try
   {
      return fallbackModuloAssign(*this, second);
   }
   catch ( ... ) // catch failed attempt of falling back to integer operation
   {
      *this = divideMagnitudesWithRemainder(second);
      return *this;
   }
}

BigInteger panda::BigInteger::operator*(const BigInteger& second) const
{
   BigInteger result = *this;
   result *= second;
   return result;
}

BigInteger panda::BigInteger::operator/(const BigInteger& second) const
{
   if ( second.isZero() )
   {
      throw std::invalid_argument("Integer division by 0 in \"BigInteger::operator/\".");
   }
   BigInteger result = *this;
   result /= second;
   return result;
}

BigInteger panda::BigInteger::operator+(const BigInteger& second) const
{
   BigInteger result = *this;
   result += second;
   return result;
}

BigInteger panda::BigInteger::operator-(const BigInteger& second) const
{
   BigInteger result = *this;
   result -= second;
   return result;
}

BigInteger panda::BigInteger::operator%(const BigInteger& second) const
{
   BigInteger result = *this;
   result %= second;
   return result;
}

namespace
{
   BigInteger& fallbackMultiplyAssign(BigInteger& first, const BigInteger& second)
   {
      const int a = first;
      const int b = second;
      const auto limit = ((a > 0) == (b > 0)) ? DataLimits::max() : DataLimits::min();
      if ( (a > 0 && limit / b < a) || (a < 0 && limit / b > a) )
      {
         throw std::exception();
      }
      first = BigInteger(a * b);
      return first;
   }

   BigInteger& fallbackDivideAssign(BigInteger& first, const BigInteger& second)
   {
      const int a = first;
      const int b = second;
      assert( b != 0 );
      if ( (b == -1) && ((a < 0 && DataLimits::max() + a < 0) || (a > 0 && a + DataLimits::min() > 0)) )
      {
         throw std::exception();
      }
      first = BigInteger(a / b);
      return first;
   }

   BigInteger& fallbackAddAssign(BigInteger& first, const BigInteger& second)
   {
      const int a = first;
      const int b = second;
      if ( (a > 0 && b > 0 && b > DataLimits::max() - a) || (a < 0 && b < 0 && b < DataLimits::min() - a) )
      {
         throw std::exception();
      }
      first = BigInteger(a + b);
      return first;
   }

   BigInteger& fallbackSubtractAssign(BigInteger& first, const BigInteger& second)
   {
      const int a = first;
      const int b = second;
      if ( (b < 0 && a > DataLimits::max() + b) || (b > 0 && a < DataLimits::min() + b) )
      {
         throw std::exception();
      }
      first = BigInteger(a - b);
      return first;
   }

   BigInteger& fallbackModuloAssign(BigInteger& first, const BigInteger& second)
   {
      const int a = first;
      const int b = second;
      assert( b > 0 );
      first = BigInteger(((a < 0) ? b : 0) + (a % b));
      return first;
   }
}

