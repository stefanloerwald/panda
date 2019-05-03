
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "big_integer.h"

#include <algorithm>
#include <cstdlib>

using namespace panda;

bool panda::BigInteger::operator==(const int value) const noexcept
{
   if ( data.size() > 1 )
   {
      return false;
   }
   if ( (value >= 0) != (sign == Sign::Positive) )
   {
      return false;
   }
   return data[0] == static_cast<decltype(data[0])>(std::abs(value));
}

bool panda::BigInteger::operator!=(const int value) const noexcept
{
   return !(*this == value);
}

bool panda::BigInteger::operator<(const int value) const noexcept
{
   if ( data.size() > 1 ) // magnitude is bigger, so it all comes down to the sign of this.
   {
      return (sign == Sign::Negative);
   }
   if ( (sign == Sign::Positive) != (value >= 0) ) // different signs, this is smaller if sign is negative.
   {
      return (sign == Sign::Negative);
   }
   if ( sign == Sign::Negative ) // now both have same sign
   {
      return (data[0] > static_cast<DataType>(std::abs(value)));
   }
   return (data[0] < static_cast<DataType>(value));
}

bool panda::BigInteger::operator>(const int value) const noexcept
{
   if ( data.size() > 1 ) // magnitude is bigger, so it all comes down to the sign of this.
   {
      return (sign == Sign::Positive);
   }
   if ( (sign == Sign::Positive) != (value >= 0) ) // different signs, this is greater if sign is positive.
   {
      return (sign == Sign::Positive);
   }
   if ( sign == Sign::Negative ) // now both have same sign
   {
      return (data[0] < static_cast<DataType>(std::abs(value)));
   }
   return (data[0] > static_cast<DataType>(value));
}

bool panda::BigInteger::operator<=(const int value) const noexcept
{
   return !(*this > value);
}

bool panda::BigInteger::operator>=(const int value) const noexcept
{
   return !(*this < value);
}

bool panda::BigInteger::operator==(const BigInteger& second) const noexcept
{
   if ( sign != second.sign || data.size() != second.data.size() )
   {
      return false;
   }
   return std::equal(data.cbegin(), data.cend(), second.data.cbegin());
}

bool panda::BigInteger::operator!=(const BigInteger& second) const noexcept
{
   return !(*this == second);
}

bool panda::BigInteger::operator<(const BigInteger& second) const noexcept
{
   if ( sign != second.sign )
   {
      return (sign == Sign::Negative);
   }
   if ( sign == Sign::Positive )
   {
      return this->isMagnitudeSmallerThan(second);
   }
   else
   {
      return this->isMagnitudeGreaterThan(second);
   }
}

bool panda::BigInteger::operator>(const BigInteger& second) const noexcept
{
   return second < *this;
}

bool panda::BigInteger::operator<=(const BigInteger& second) const noexcept
{
   return !(*this > second);
}

bool panda::BigInteger::operator>=(const BigInteger& second) const noexcept
{
   return !(*this < second);
}

