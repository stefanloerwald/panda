
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <array>
#include <cassert>
#include <limits>

#include "bitset_variable_size.h"
#include "popcount.h"

using namespace panda;

panda::BitsetVariableSize::BitsetVariableSize(const std::size_t bits)
:
   data(1 + (bits - 1) / std::numeric_limits<DataType>::digits)
{
   assert( bits > 0 );
}

bool panda::BitsetVariableSize::equals(const BitsetVariableSize& second, const std::size_t max) const noexcept
{
   assert( data.size() == second.data.size() );
   const auto end = data.cbegin() + 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   return std::equal(data.cbegin(), end, second.data.cbegin());
}

bool panda::BitsetVariableSize::contains(const BitsetVariableSize& second, const std::size_t max) const noexcept
{
   assert( data.size() == second.data.size() );
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

std::size_t panda::BitsetVariableSize::count(const std::size_t max) const noexcept
{
   std::size_t total{0};
   const auto end = data.cbegin() + 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   for ( auto it = data.cbegin(); it != end; ++it )
   {
      total += static_cast<std::size_t>(popcount(*it));
   }
   return total;
}

BitsetVariableSize panda::BitsetVariableSize::merge(const BitsetVariableSize& second, const std::size_t max) const noexcept
{
   assert( data.size() == second.data.size() );
   BitsetVariableSize result = *this;
   const std::size_t end = 1 + (max - 1) / std::numeric_limits<DataType>::digits;
   for ( std::size_t i = 0; i < end; ++i )
   {
      result.data[i] |= second.data[i];
   }
   return result;
}

void panda::BitsetVariableSize::set(const std::size_t index) noexcept
{
   const DataType mask = static_cast<DataType>(1u) << (index % std::numeric_limits<DataType>::digits);
   assert(index / std::numeric_limits<DataType>::digits < data.size() );
   data[index / std::numeric_limits<DataType>::digits] |= mask;
}

std::size_t panda::BitsetVariableSize::unionCount(const BitsetVariableSize& a, const BitsetVariableSize& b, const std::size_t max) noexcept
{
   std::size_t total{0};
   const auto end_a = a.data.cbegin() + 1 + (max - 1) / std::numeric_limits<BitsetVariableSize::DataType>::digits;
   for ( auto ita = a.data.cbegin(), itb = b.data.cbegin(); ita != end_a; ++ita, ++itb )
   {
      total += static_cast<std::size_t>(popcount(*ita));
      total += static_cast<std::size_t>(popcount(*itb));
      total -= static_cast<std::size_t>(popcount((*ita) & (*itb)));
   }
   return total;
}

bool panda::BitsetVariableSize::unionContains(const BitsetVariableSize& a, const BitsetVariableSize& b, const BitsetVariableSize& inner, const std::size_t max) noexcept
{
   const auto end = 1u + (max - 1) / std::numeric_limits<BitsetVariableSize::DataType>::digits;
   for ( std::size_t i = 0; i < end; ++i )
   {
      if ( ((a.data[i] | b.data[i]) & inner.data[i]) != inner.data[i] )
      {
         return false;
      }
   }
   return true;
}

