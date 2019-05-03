
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace panda
{
   /// A class for fixed size bitsets with methods for equality, containment checks and merging.
   /// This is mainly a performance optimization of class BitsetVariableSize.
   template <std::size_t Size>
   class BitsetFixedSize
   {
      public:
         static std::size_t unionCount(const BitsetFixedSize<Size>&, const BitsetFixedSize<Size>&, const std::size_t) noexcept;
         static bool unionContains(const BitsetFixedSize<Size>&, const BitsetFixedSize<Size>&, const BitsetFixedSize<Size>&, const std::size_t) noexcept;
         /// Underlying data type.
         using DataType = uint32_t;
         /// Constructor: argument denotes number of bits.
         BitsetFixedSize(const std::size_t);
         /// Default copy constructor.
         BitsetFixedSize(const BitsetFixedSize&) = default;
         /// Default move constructor.
         BitsetFixedSize(BitsetFixedSize&&) = default;
         /// Default copy assignment operator.
         BitsetFixedSize& operator=(const BitsetFixedSize&) = default;
         /// Default move assignment operator.
         BitsetFixedSize& operator=(BitsetFixedSize&&) = default;
         /// Comparison (equality) with another Bitset with a hint of highest set bit.
         bool equals(const BitsetFixedSize<Size>&, const std::size_t) const noexcept;
         /// Checks if the passed Bitset is contained in this (with hint of highest set bit).
         bool contains(const BitsetFixedSize<Size>&, const std::size_t) const noexcept;
         /// Returns the union with a second bitset with a hint of highest set bit.
         BitsetFixedSize<Size> merge(const BitsetFixedSize<Size>&, const std::size_t) const noexcept;
         /// Returns the number of 1s in the bitset with a hint of highest set bit.
         std::size_t count(const std::size_t) const noexcept;
         /// Sets the i^th bit.
         void set(const std::size_t) noexcept;
      private:
         std::array<DataType, Size> data;
   };
}

#include "bitset_fixed_size.tpp"

