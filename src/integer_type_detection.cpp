
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "integer_type_detection.h"

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>

using namespace panda;

namespace
{
   /// Returns the integer type interpreted from char*.
   IntegerType integerTypeFromString(char*);
}

IntegerType panda::integerType(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-i") == 0 )
      {
         if ( i + 1 == argc )
         {
            std::string message = "Command line option -i needs a parameter:";
            message += " Choose either \"16\", \"32\", \"64\", \"safe\" or \"inf\"-";
            throw std::invalid_argument(message);
         }
         return integerTypeFromString(argv[i + 1]);
      }
      else if ( std::strncmp(argv[i], "-i=", 3) == 0 )
      {
         return integerTypeFromString(argv[i] + 3);
      }
      else if ( std::strncmp(argv[i], "--integer-type=", 15) == 0 )
      {
         return integerTypeFromString(argv[i] + 15);
      }
      else if ( std::strncmp(argv[i], "-i", 2) == 0 || std::strncmp(argv[i], "--i", 3) == 0 )
      {
         throw std::invalid_argument("Illegal parameter. Did you mean \"-i <type>\" or \"--integer-type=<type>\"?");
      }
   }
   return IntegerType::Default;
}

namespace
{
   IntegerType integerTypeFromString(char* string)
   {
      if ( std::strcmp(string, "16") == 0 )
      {
         #ifdef INT16_MIN
         return IntegerType::Fixed16;
         #else
         throw std::invalid_argument("Your system does not support an integer type with exactly 16 bit width.");
         #endif
      }
      else if ( std::strcmp(string, "32") == 0 )
      {
         #ifdef INT32_MIN
         return IntegerType::Fixed32;
         #else
         throw std::invalid_argument("Your system does not support an integer type with exactly 32 bit width.");
         #endif
      }
      else if ( std::strcmp(string, "64") == 0 )
      {
         #ifdef INT64_MIN
         return IntegerType::Fixed64;
         #else
         throw std::invalid_argument("Your system does not support an integer type with exactly 64 bit width.");
         #endif
      }
      else if ( std::strcmp(string, "safe") == 0 )
      {
         return IntegerType::Safe;
      }
      else if ( std::strcmp(string, "inf") == 0 )
      {
         return IntegerType::Variable;
      }
      std::string message = "Invalid parameter to command line option -i";
      message += " / --integer-type: Choose either \"16\", \"32\", \"64\", \"safe\" or \"inf\"-";
      throw std::invalid_argument(message);
   }
}

