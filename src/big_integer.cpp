
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "big_integer.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <stdexcept>

using namespace panda;

BigInteger& panda::BigInteger::operator<<=(const std::size_t distance)
{
   if ( isZero() )
   {
      return *this;
   }
   const auto block_distance = distance / std::numeric_limits<DataType>::digits;
   const auto residual_distance = distance % std::numeric_limits<DataType>::digits;
   if ( residual_distance > 0 )
   {
      const auto mask = static_cast<DataType>(1) << (std::numeric_limits<DataType>::digits - residual_distance);
      const auto old_size = data.size();
      if ( (data.back() & mask) != 0 )
      {
         data.push_back(0);
      }
      for ( std::size_t i = old_size; i > 0; )
      {
         --i;
         if ( (data[i] & mask) != 0 )
         {
            data[i + 1] += (data[i] >> (std::numeric_limits<DataType>::digits - residual_distance));
         }
         data[i] <<= residual_distance;
      }
   }
   for ( std::size_t i = 0; i < block_distance; ++i )
   {
      data.insert(data.begin(), 0);
   }
   return *this;
}

BigInteger& panda::BigInteger::operator>>=(const std::size_t distance)
{
   const auto block_distance = distance / std::numeric_limits<DataType>::digits;
   const auto residual_distance = distance % std::numeric_limits<DataType>::digits;
   data.erase(data.begin(), data.begin() + static_cast<Magnitude::difference_type>(block_distance));
   if ( residual_distance > 0 )
   {
      const auto bound = static_cast<DataType>(1) << (residual_distance);
      data[0] >>= residual_distance;
      for ( std::size_t i = 1; i < data.size(); ++i )
      {
         data[i - 1] += (data[i] % bound) << (std::numeric_limits<DataType>::digits - residual_distance);
         data[i] >>= residual_distance;
      }
   }
   shrinkToFit();
   return *this;
}

BigInteger panda::BigInteger::operator<<(const std::size_t distance)
{
   BigInteger result = *this;
   result <<= distance;
   return result;
}

BigInteger panda::BigInteger::operator-() const
{
   BigInteger result = *this;
   result.flipSign();
   return result;
}

BigInteger panda::abs(BigInteger input) noexcept
{
   input.sign = BigInteger::Sign::Positive;
   return input;
}

BigInteger panda::BigInteger::divideMagnitudesWithRemainder(const BigInteger& second)
{
   if ( isMagnitudeSmallerThan(second) )
   {
      BigInteger remainder = *this;
      *this = BigInteger(0);
      return remainder;
   }
   if ( *this == second )
   {
      *this = BigInteger(1);
      return BigInteger(0);
   }
   const auto is_negative = isNegative();
   if ( is_negative )
   {
      flipSign();
   }
   BigInteger result(0);
   BigInteger remainder(0);
   for ( std::size_t it = std::numeric_limits<DataType>::digits * data.size(); it > 0; --it )
   {
      const auto bit = (it - 1) % std::numeric_limits<DataType>::digits;
      const auto& block = data[(it - 1) / std::numeric_limits<DataType>::digits];
      remainder <<= 1;
      result <<= 1;
      remainder.data.front() |= ((block >> bit) & 1);
      if ( !remainder.isMagnitudeSmallerThan(second) )
      {
         remainder.subtractMagnitude(second.data);
         result.data.front() |= 1;
      }
   }
   if ( is_negative )
   {
      result.flipSign();
      remainder.flipSign();
      remainder += second;
   }
   *this = result;
   return remainder;
}

BigInteger& panda::BigInteger::addMagnitude(const Magnitude& second)
{
   bool carry = false; // carry can only ever be 0 or 1.
   if ( second.size() > data.size() )
   {
      data.resize(second.size());
   }
   for ( std::size_t i = 0; i < second.size(); ++i )
   {
      if ( carry && second[i] + 1 == 0 )
      {
         carry = true; // data[i] stays the same, but we get a carry
      }
      else
      {
         const auto old_value = data[i];
         data[i] += second[i] + ((carry) ? 1 : 0);
         carry = (data[i] < old_value); // we get a carry iff data[i] > data[i] + second[i] + carry.
      }
   }
   for ( std::size_t i = second.size(); i < data.size() && carry; ++i )
   {
      ++data[i]; // while there is still a carry and data is big enough, data[i] is incremented (carry only)
      carry = ( data[i] == 0 ); // and there is another carry if data[i] is now 0, i.e. wrapped around.
   }
   if ( carry ) // if there is still a carry, data wasn't big enough.
   {
      data.push_back(1);
   }
   return *this;
}

BigInteger& panda::BigInteger::subtractMagnitude(const Magnitude& second)
{
   assert(data.size() >= second.size());
   bool carry = false; // carry can only ever be 0 or 1.
   for ( std::size_t i = 0; i < second.size(); ++i )
   {
      if ( carry && second[i] + 1 == 0 )
      {
         carry = true; // data[i] stays the same, but we get a carry
      }
      else
      {
         const auto old_value = data[i];
         data[i] -= second[i] + ((carry) ? 1 : 0);
         carry = (data[i] > old_value); // we get a carry iff data[i] < data[i] + second[i] + carry.
      }
   }
   for ( std::size_t i = second.size(); i < data.size() && carry; ++i )
   {
      carry = ( data[i] == 0 ); // while there is still a carry and data is big enough, data[i] is incremented
      --data[i]; // (carry only) and there is another carry if data[i] is now 0, i.e. wrapped around.
   }
   assert( !carry );
   shrinkToFit();
   return *this;
}

bool panda::BigInteger::isNegative() const noexcept
{
   return sign == Sign::Negative;
}

bool panda::BigInteger::isPositive() const noexcept
{
   return (sign == Sign::Positive) && !isZero();
}

bool panda::BigInteger::isZero() const noexcept
{
   return (data.size() == 1) && (data[0] == 0);
}

bool panda::BigInteger::isMagnitudeSmallerThan(const BigInteger& second) const
{
   if ( data.size() != second.data.size() )
   {
      return (data.size() < second.data.size());
   }
   const auto pivot = std::mismatch(data.crbegin(), data.crend(), second.data.crbegin());
   return pivot.first != data.crend() && *pivot.first < *pivot.second;
}

bool panda::BigInteger::isMagnitudeGreaterThan(const BigInteger& second) const
{
   if ( data.size() != second.data.size() )
   {
      return (data.size() > second.data.size());
   }
   const auto pivot = std::mismatch(data.crbegin(), data.crend(), second.data.crbegin());
   return pivot.first != data.crend() && *pivot.first > *pivot.second;
}

void panda::BigInteger::flipSign()
{
   if ( !isZero() )
   {
      sign = (sign == Sign::Positive) ? Sign::Negative : Sign::Positive;
   }
}

void panda::BigInteger::shrinkToFit()
{
   while ( data.size() > 1 && data.back() == 0 )
   {
      data.pop_back();
   }
   if ( data.size() == 1 && data[0] == 0 )
   {
      sign = Sign::Positive;
   }
}

void panda::BigInteger::setZero()
{
   data = {0};
   sign = Sign::Positive;
}

