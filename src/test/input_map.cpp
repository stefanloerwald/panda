
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "input_map.h"

#include <iostream>
#include <sstream>

#include "istream_peek_line.h"

using namespace panda;

namespace
{
   void single_valid();
}

int main()
try
{
   single_valid();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void single_valid()
   {
      Names names{"x0", "x1", "x2", "x3", "x4"};
      std::istringstream stream("MAPS\nx0 +x1 -x2+1 1-x3 +7+x4");
      const auto maps = input::implementation::maps(stream, names);
      ASSERT(maps.size() == 1, "");
      ASSERT((maps[0] == Map{Image{std::make_pair(0u, 1)},
                             Image{std::make_pair(1u, 1)},
                             Image{std::make_pair(2u, -1), std::make_pair(5u, 1)},
                             Image{std::make_pair(5u, 1), std::make_pair(3u, -1)},
                             Image{std::make_pair(5u, 7), std::make_pair(4u, 1)},
                             Image{std::make_pair(5u, 1)}}), "");
   }
}

