
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "input_detection.h"

#include <cstring>

using namespace panda;

namespace
{
   void testContainsMultipleFilenames();
   void testGetFilename();
}

int main()
try
{
   testContainsMultipleFilenames();
   testGetFilename();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void testContainsMultipleFilenames()
   {
      char** argv = new char*[5];
      argv[0] = nullptr;
      argv[1] = new char[3];
      argv[2] = new char[4];
      argv[3] = new char[6];
      argv[4] = new char[6];
      strcpy(argv[1], "-i");
      strcpy(argv[2], "bad");
      strcpy(argv[3], "file1");
      strcpy(argv[4], "file2");
      ASSERT_ANY_EXCEPTION(getFilename(5, argv), "Two filenames supplied, ambiguity.");
      delete [] argv[4];
      delete [] argv[3];
      delete [] argv[2];
      delete [] argv[1];
      delete [] argv;
   }
   void testGetFilename()
   {
      char** argv = new char*[4];
      argv[0] = nullptr;
      argv[1] = new char[3];
      strcpy(argv[1], "-i");
      argv[2] = new char[4];
      strcpy(argv[2], "bad");
      argv[3] = new char[6];
      strcpy(argv[3], "file1");
      ASSERT(getFilename(3, argv) == "", "No filename supplied.");
      ASSERT(getFilename(4, argv) == "file1", "One filename supplied.");
      delete [] argv[3];
      delete [] argv[2];
      delete [] argv[1];
      delete [] argv;
   }
}

