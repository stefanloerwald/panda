
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_method.h"

#include <cstring>
#include <stdexcept>

using namespace panda;

namespace
{
   Method detectMethod(char*);
}

Method panda::input::method(int argc, char** argv)
{
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-m") == 0 )
      {
         if ( i + 1 == argc )
         {
            throw std::invalid_argument("Expected argument to option \"--method\".");
         }
         return detectMethod(argv[i + 1]);
      }
      else if ( std::strncmp(argv[i], "-m=", 3) == 0 )
      {
         return detectMethod(argv[i] + 3);
      }
      else if ( std::strncmp(argv[i], "--method=", 9) == 0 )
      {
         return detectMethod(argv[i] + 9);
      }
      else if ( std::strncmp(argv[i], "-m", 2) == 0 || std::strncmp(argv[i], "--m", 3) == 0 )
      {
         throw std::invalid_argument("Illegal parameter. Did you mean \"-m <method>\" or \"--method=<method>\"?");
      }
   }
   return Method::AdjacencyDecomposition; // default value
}

namespace
{
   Method detectMethod(char* argument)
   {
      if ( std::strcmp(argument, "dd") == 0 ||
           std::strcmp(argument, "double-description") == 0 ||
           std::strcmp(argument, "double_description") == 0 )
      {
         return Method::DoubleDescription;
      }
      if ( std::strcmp(argument, "ad") == 0 ||
           std::strcmp(argument, "adjacency-decomposition") == 0 ||
           std::strcmp(argument, "adjacency_decomposition") == 0 )
      {
         return Method::AdjacencyDecomposition;
      }
      throw std::invalid_argument("Expected argument to option \"--method\".");
   }
}

