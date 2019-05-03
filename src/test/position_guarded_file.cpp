
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "position_guarded_file.h"

#include <cstddef>
#include <string>
#include <vector>

using namespace panda;

int main()
try
{
   std::ifstream file("src/test/position_guarded_file.cpp");
   std::vector<std::string> content;
   {
      PositionGuardedFile pg(file);
      for ( std::string line; std::getline(pg.file, line); )
      {
         content.push_back(line);
      }
   }
   std::size_t i = 0;
   for ( std::string line; i < content.size() && std::getline(file, line); ++i )
   {
      ASSERT(line == content[i], "PositionGuardedFile did not reset to the correct position.");
   }
   ASSERT(i == content.size(), "PositionGuardedFile did not reset to the correct position.");
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

