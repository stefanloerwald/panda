
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include "big_integer.h"
#include "integer_type_detection.h"
#include "safe_integer.h"

#ifdef NO_FLEXIBILITY

template <template <typename> class Functor>
int panda::IntegerTypeSelector<Functor>::select(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   const auto integer_type = integerType(argc, argv);
   if ( integer_type != IntegerType::Default )
   {
      std::cerr << "You compiled with \"-DNO_FLEXIBILITY\"."
                << " Selection of fixed size integer type or arbitrary precision integer is disabled."
                << " Calculations will be done with the standard integer type \"int\".\n";
   }
   return Functor<int>::call(argc, argv);
}

#else

template <template <typename> class Functor>
int panda::IntegerTypeSelector<Functor>::select(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   const auto integer_type = integerType(argc, argv);
   switch ( integer_type )
   {
      case IntegerType::Fixed16:
      {
         return Functor<int16_t>::call(argc, argv);
      }
      case IntegerType::Fixed32:
      {
         return Functor<int32_t>::call(argc, argv);
      }
      case IntegerType::Fixed64:
      {
         return Functor<int64_t>::call(argc, argv);
      }
      case IntegerType::Safe:
      {
         return Functor<SafeInteger>::call(argc, argv);
      }
      case IntegerType::Variable:
      {
         return Functor<BigInteger>::call(argc, argv);
      }
      case IntegerType::Default:
      {
         return Functor<int>::call(argc, argv);
      }
   }
   return 1; // strictly speaking, this cannot happen, though some compilers do not see that this function will always be exited through one of the above cases.
}

#endif

