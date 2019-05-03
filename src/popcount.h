
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstdint>

namespace panda
{
   /// returns the number of active bits in an unsigned integer.
   inline int popcount(uint32_t) noexcept;
}

#include "popcount.tpp"

