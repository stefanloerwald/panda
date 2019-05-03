
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_map_operations.h"

using namespace panda;

int main()
try
{
   // MAP: x1 1-x0 x2 1
   // Equation: x0 + x1 +x2 = 3
   Map map{{std::make_pair(1u, 1)}, {std::make_pair(3u, 1), std::make_pair(0u, -1)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}};
   Maps maps{map};
   Equations<int> equations{{1, 1, 1, -3}};
   const auto nmaps = algorithm::normalize(maps, equations);
   ASSERT((nmaps[0] == Map{{}, {std::make_pair(1u, 1), std::make_pair(2u, 1), std::make_pair(3u, -2)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}}), "Data mismatch.");
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

