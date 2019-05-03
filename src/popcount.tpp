
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

/// The fastest popcount is done with a hardware instruction.
/// Unfortunately this is not guaranteed to be available on every machine.
/// GCC and CLANG provide a builtin popcount which either performs the hardware
/// instruction or an efficient implementation if the instruction doesn't exist.
/// For any other compiler, we fall back to an efficient implementation,
/// see http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel

namespace panda
{
   #if defined(__GNUC__) || defined(__clang__)
      #include <type_traits>
      int popcount(uint32_t n) noexcept
      {
         static_assert(std::is_same<uint32_t, unsigned int>::value, "`__builtin_popcount` expects an `unsigned int`, but the provided parameter is of type `uint32_t` which is not the same as `unsigned int` on this machine.");
         return __builtin_popcount(n);
      }
   #else
      int popcount(uint32_t n) noexcept
      {
          n = n - ((n >> 1) & 0x55555555);
          n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
          return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
      }
   #endif
}

