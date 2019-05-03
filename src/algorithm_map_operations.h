
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <iosfwd>

#include "maps.h"
#include "matrix.h"
#include "tags.h"

namespace panda
{
   namespace algorithm
   {
      /// Returns a Map in normal form, i.e. without any variables that are eliminated.
      template <typename Integer>
      Maps normalize(Maps, const Equations<Integer>&);
      /// Applies a Map onto a row.
      template <typename Integer, typename TagType>
      Row<Integer> apply(const Map&, const Row<Integer>&, TagType);
   }
}

/// Output of a map.
std::ostream& operator<<(std::ostream&, const panda::Map&);

#include "algorithm_map_operations.eti"

