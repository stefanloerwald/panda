
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace panda
{
   /// A class for variable size bitsets that provides checks for equality and containment, as well as utility functions such as merging.
   class BitsetVariableSize
   {
      public:
         static std::size_t unionCount(const BitsetVariableSize&, const BitsetVariableSize&, const std::size_t) noexcept;
         static bool unionContains(const BitsetVariableSize&, const BitsetVariableSize&, const BitsetVariableSize&, const std::size_t) noexcept;
         /// Underlying data type.
         using DataType = uint32_t;
         /// Constructor: argument denotes number of bits.
         BitsetVariableSize(const std::size_t);
         /// Default copy constructor.
         BitsetVariableSize(const BitsetVariableSize&) = default;
         /// Default move constructor.
         BitsetVariableSize(BitsetVariableSize&&) = default;
         /// Default copy assignment operator.
         BitsetVariableSize& operator=(const BitsetVariableSize&) = default;
         /// Default move assignment operator.
         BitsetVariableSize& operator=(BitsetVariableSize&&) = default;
         /// Comparison (equality) with another Bitset with a hint of highest set bit.
         bool equals(const BitsetVariableSize&, const std::size_t) const noexcept;
         /// Checks if the passed Bitset is contained in this (with hint of highest set bit).
         bool contains(const BitsetVariableSize&, const std::size_t) const noexcept;
         /// Returns the union with a second bitset with a hint of highest set bit.
         BitsetVariableSize merge(const BitsetVariableSize&, const std::size_t) const noexcept;
         /// Returns the number of 1s in the bitset with a hint of highest set bit.
         std::size_t count(const std::size_t) const noexcept;
         /// Sets the i^th bit.
         void set(const std::size_t) noexcept;
      private:
         std::vector<DataType> data;
   };
}

