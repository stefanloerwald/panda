
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "concurrency.h"

#include <cstring>
#include <stdexcept>
#include <thread>

using namespace panda;

namespace
{
   void noArguments();
   void shortInvalid();
   void shortValid();
   void longInvalid();
   void longValid();
}

int main()
try
{
   noArguments();
   shortInvalid();
   shortValid();
   longInvalid();
   longValid();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void noArguments()
   {
      char** argv = new char*[1];
      argv[0] = nullptr;
      ASSERT_NOTHROW(concurrency::numberOfThreads(1, argv), "No arguments");
      ASSERT(concurrency::numberOfThreads(1, argv) > 0, "Default value must be positive");
      const auto message = "Default value must be at least the number of threads provided by the system";
      ASSERT(concurrency::numberOfThreads(1, argv) >= static_cast<int>(std::thread::hardware_concurrency()), message);
      delete [] argv;
   }
   void shortInvalid()
   {
      char** argv = new char*[3];
      argv[0] = nullptr;
      argv[1] = new char[3];
      strcpy(argv[1], "-t");
      argv[2] = new char[10];
      strcpy(argv[2], "bad");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(2, argv), std::invalid_argument, "No parameter provided.");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(3, argv), std::invalid_argument, "Parameter isn't a number");
      strcpy(argv[2], "0");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(3, argv), std::invalid_argument, "Parameter isn't greater 0");
      strcpy(argv[2], "-1");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(3, argv), std::invalid_argument, "Parameter is negative");
      delete [] argv[2];
      delete [] argv[1];
      delete [] argv;
   }
   void shortValid()
   {
      char** argv = new char*[3];
      argv[0] = nullptr;
      argv[1] = new char[3];
      strcpy(argv[1], "-t");
      argv[2] = new char[10];
      strcpy(argv[2], "10");
      ASSERT_NOTHROW(concurrency::numberOfThreads(3, argv), "Parameter is valid");
      ASSERT(concurrency::numberOfThreads(3, argv) == 10, "Parameter is 10");
      strcpy(argv[2], "1");
      ASSERT_NOTHROW(concurrency::numberOfThreads(3, argv), "Parameter is valid");
      ASSERT(concurrency::numberOfThreads(3, argv) == 1, "Parameter is 1");
      delete [] argv[2];
      delete [] argv[1];
      delete [] argv;
   }
   void longInvalid()
   {
      char** argv = new char*[2];
      argv[0] = nullptr;
      argv[1] = new char[20];
      strcpy(argv[1], "--threads=");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(2, argv), std::invalid_argument, "No parameter provided.");
      strcpy(argv[1], "--threads=ABC");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(2, argv), std::invalid_argument, "Parameter isn't a number");
      strcpy(argv[1], "--threads=0");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(2, argv), std::invalid_argument, "Parameter isn't greater 0");
      strcpy(argv[1], "--threads=-1");
      ASSERT_EXCEPTION(concurrency::numberOfThreads(2, argv), std::invalid_argument, "Parameter is negative");
      delete [] argv[1];
      delete [] argv;
   }
   void longValid()
   {
      char** argv = new char*[2];
      argv[0] = nullptr;
      argv[1] = new char[20];
      strcpy(argv[1], "--threads=10");
      ASSERT_NOTHROW(concurrency::numberOfThreads(2, argv), "Parameter is valid");
      ASSERT(concurrency::numberOfThreads(2, argv) == 10, "Parameter is 10");
      strcpy(argv[1], "--threads=1");
      ASSERT_NOTHROW(concurrency::numberOfThreads(2, argv), "Parameter is valid");
      ASSERT(concurrency::numberOfThreads(2, argv) == 1, "Parameter is 1");
      delete [] argv[1];
      delete [] argv;
   }
}

