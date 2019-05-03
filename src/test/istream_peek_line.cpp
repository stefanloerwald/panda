
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "istream_peek_line.h"

#include <fstream>

using namespace panda;

int main()
try
{
   std::ifstream file("./src/test/ifstream_peek_line.cpp");
   while ( file )
   {
      const auto pos = file.tellg();
      const auto line = peekLine(file);
      ASSERT(pos == file.tellg(), "peekLine modified file position.");
      std::string actual_line;
      std::getline(file, actual_line);
      ASSERT(line == actual_line, "peeked line differed from the actual line.");
   }
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

