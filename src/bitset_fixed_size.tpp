
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <algorithm>
#include <cassert>
#include <limits>

#include "popcount.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template <std::size_t Size>
panda::BitsetFixedSize<Size>::BitsetFixedSize(const std::size_t b)
:
   data()
{
   assert( b <= std::numeric_limits<DataType>::digits * Size );
}
#pragma GCC diagnostic pop

template <std::size_t Size>
bool panda::BitsetFixedSize<Size>::equals(const BitsetFixedSize<Size>& second, const std::size_t max) const noexcept
{
   const auto end = data.cbegin() + 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   return std::equal(data.cbegin(), end, second.data.cbegin());
}

template <std::size_t Size>
bool panda::BitsetFixedSize<Size>::contains(const BitsetFixedSize<Size>& second, const std::size_t max) const noexcept
{
   const auto end = 1u + (max - 1) / std::numeric_limits<DataType>::digits;
   for ( std::size_t i = 0; i < end; ++i )
   {
      if ( (data[i] & second.data[i]) != second.data[i] )
      {
         return false;
      }
   }
   return true;
}

template <std::size_t Size>
std::size_t panda::BitsetFixedSize<Size>::count(const std::size_t max) const noexcept
{
   std::size_t total{0};
   const auto end = data.cbegin() + 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   for ( auto it = data.cbegin(); it != end; ++it )
   {
      total += static_cast<std::size_t>(popcount(*it));
   }
   return total;
}

template <std::size_t Size>
panda::BitsetFixedSize<Size> panda::BitsetFixedSize<Size>::merge(const BitsetFixedSize<Size>& second, const std::size_t max) const noexcept
{
   auto result = *this;
   const auto end = 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   for ( std::size_t i = 0; i < end; ++i )
   {
      result.data[i] |= second.data[i];
   }
   return result;
}

template <std::size_t Size>
void panda::BitsetFixedSize<Size>::set(const std::size_t index) noexcept
{
   const auto mask = static_cast<DataType>(1u) << (index % std::numeric_limits<DataType>::digits);
   assert(index / std::numeric_limits<DataType>::digits < Size );
   data[index / std::numeric_limits<DataType>::digits] |= mask;
}

template <std::size_t Size>
std::size_t panda::BitsetFixedSize<Size>::unionCount(const BitsetFixedSize<Size>& a, const BitsetFixedSize<Size>& b, const std::size_t max) noexcept
{
   std::size_t total{0};
   const auto end_a = a.data.cbegin() + 1 + (max - 1) / std::numeric_limits<typename BitsetFixedSize<Size>::DataType>::digits;
   for ( auto ita = a.data.cbegin(), itb = b.data.cbegin(); ita != end_a; ++ita, ++itb )
   {
      total += static_cast<std::size_t>(popcount(*ita));
      total += static_cast<std::size_t>(popcount(*itb));
      total -= static_cast<std::size_t>(popcount((*ita) & (*itb)));
   }
   return total;
}

template <std::size_t Size>
bool panda::BitsetFixedSize<Size>::unionContains(const BitsetFixedSize<Size>& a, const BitsetFixedSize<Size>& b, const BitsetFixedSize<Size>& inner, const std::size_t max) noexcept
{
   const auto end = 1u + (max - 1) / std::numeric_limits<typename BitsetFixedSize<Size>::DataType>::digits;
   for ( std::size_t i = 0; i < end; ++i )
   {
      if ( ((a.data[i] | b.data[i]) & inner.data[i]) != inner.data[i] )
      {
         return false;
      }
   }
   return true;
}

