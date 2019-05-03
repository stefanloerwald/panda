
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <utility>
#include <vector>

namespace panda
{
   /// Type alias for index.
   using Index = std::size_t;
   /// Type alias for factor.
   using Factor = int;
   /// Type alias for term.
   using Term = std::pair<Index, Factor>;
   /// Type alias for image.
   using Image = std::vector<Term>;
   /// Type alias for map.
   using Map = std::vector<Image>;
   /// Type alias for maps.
   using Maps = std::vector<Map>;
}

