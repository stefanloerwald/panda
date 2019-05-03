
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "concurrency.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace panda;

namespace
{
   /// Tries to read a number from char*.
   int interpretParameter(char*);
}

int panda::concurrency::numberOfThreads(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-t") == 0 )
      {
         if ( i + 1 == argc )
         {
            throw std::invalid_argument("Command line option \"-t <n>\" needs an integral parameter.");
         }
         return interpretParameter(argv[i + 1]);
      }
      else if ( std::strncmp(argv[i], "-t=", 3) == 0 )
      {
         return interpretParameter(argv[i] + 3);
      }
      else if ( std::strncmp(argv[i], "--threads=", 10) == 0 )
      {
         return interpretParameter(argv[i] + 10);
      }
      else if ( std::strncmp(argv[i], "-t", 2) == 0 || std::strncmp(argv[i], "--t", 3) == 0 )
      {
         throw std::invalid_argument("Illegal parameter. Did you mean \"-t <n>\" or \"--threads=<n>\"?");
      }
   }
   const auto default_value = static_cast<int>(std::thread::hardware_concurrency());
   return (default_value > 0) ? default_value : 1;
}

namespace
{
   int interpretParameter(char* string)
   {
      assert( string != nullptr );
      std::istringstream stream(string);
      int n;
      if ( !(stream >> n) )
      {
         std::string message = "Command line option \"-t <n>\" / \"--threads=<n>\"";
         message += " needs an integral parameter.";
         throw std::invalid_argument(message);
      }
      std::string rest;
      stream >> rest;
      if ( !rest.empty() )
      {
         std::string message = "Command line option \"-t <n>\" / \"--threads=<n>\"";
         message += " needs an integral parameter.";
         throw std::invalid_argument(message);
      }
      if ( n <= 0 )
      {
         std::string message = "Command line option \"-t <n>\" / \"--threads=<n>\"";
         message += " needs an integral parameter greater zero.";
         throw std::invalid_argument(message);
      }
      return n;
   }
}

