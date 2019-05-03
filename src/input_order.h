
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   enum class InputOrder
   {
      NoSorting,                /// Take input as it is present in file.
      LexicographicAscending,   /// Sort as in a dictionary.
      LexicographicDescending,  /// Sort as in a backwards dictionary.
      NonZeroEntriesAscending,  /// Sort by amount of non-zero entries.
      NonZeroEntriesDescending, /// Sort by amount of zero-entries.
      Reverse                   /// Simply reverse the original order.
   };
}

