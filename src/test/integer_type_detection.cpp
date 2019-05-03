
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "integer_type_detection.h"

#include <cstring>
#include <stdexcept>

using namespace panda;

namespace
{
   void noOption();
   void shortOptionBad();
   void shortOption16();
   void shortOption32();
   void shortOption64();
   void shortOptionInf();
   void longOptionBad();
   void longOption16();
   void longOption32();
   void longOption64();
   void longOptionInf();
}

int main()
try
{
   noOption();
   shortOptionBad();
   shortOption16();
   shortOption32();
   shortOption64();
   shortOptionInf();
   longOptionBad();
   longOption16();
   longOption32();
   longOption64();
   longOptionInf();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void noOption()
   {
      char** argv = new char*[1];
      argv[0] = nullptr;
      ASSERT(integerType(1, argv) == IntegerType::Default, "");
      delete [] argv;
   }
   void shortOptionBad()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         ASSERT_EXCEPTION((integerType(2, argv)), std::invalid_argument, "");
         delete [] argv[1];
         delete [] argv;
      }
      {
         char** argv = new char*[3];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         argv[2] = new char[4];
         strcpy(argv[2], "bad");
         ASSERT_EXCEPTION((integerType(3, argv)), std::invalid_argument, "");
         delete [] argv[2];
         delete [] argv[1];
         delete [] argv;
      }
   }
   void shortOption16()
   {
      {
         char** argv = new char*[3];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         argv[2] = new char[4];
         strcpy(argv[2], "16");
         ASSERT_NOTHROW((integerType(3, argv)), "");
         ASSERT((integerType(3, argv) == IntegerType::Fixed16), "");
         delete [] argv[2];
         delete [] argv[1];
         delete [] argv;
      }
   }
   void shortOption32()
   {
      {
         char** argv = new char*[3];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         argv[2] = new char[4];
         strcpy(argv[2], "32");
         ASSERT_NOTHROW((integerType(3, argv)), "");
         ASSERT((integerType(3, argv) == IntegerType::Fixed32), "");
         delete [] argv[2];
         delete [] argv[1];
         delete [] argv;
      }
   }
   void shortOption64()
   {
      {
         char** argv = new char*[3];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         argv[2] = new char[4];
         strcpy(argv[2], "64");
         ASSERT_NOTHROW((integerType(3, argv)), "");
         ASSERT((integerType(3, argv) == IntegerType::Fixed64), "");
         delete [] argv[2];
         delete [] argv[1];
         delete [] argv;
      }
   }
   void shortOptionInf()
   {
      {
         char** argv = new char*[3];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         argv[2] = new char[4];
         strcpy(argv[2], "inf");
         ASSERT_NOTHROW((integerType(3, argv)), "");
         ASSERT((integerType(3, argv) == IntegerType::Variable), "");
         delete [] argv[2];
         delete [] argv[1];
         delete [] argv;
      }
   }
   void longOptionBad()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[16];
         strcpy(argv[1], "--integer-type=");
         ASSERT_EXCEPTION((integerType(2, argv)), std::invalid_argument, "");
         delete [] argv[1];
         delete [] argv;
      }
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=bad");
         ASSERT_EXCEPTION((integerType(2, argv)), std::invalid_argument, "");
         delete [] argv[1];
         delete [] argv;
      }
   }
   void longOption16()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=16");
         ASSERT_NOTHROW((integerType(2, argv)), "");
         ASSERT((integerType(2, argv) == IntegerType::Fixed16), "");
         delete [] argv[1];
         delete [] argv;
      }
   }
   void longOption32()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=32");
         ASSERT_NOTHROW((integerType(2, argv)), "");
         ASSERT((integerType(2, argv) == IntegerType::Fixed32), "");
         delete [] argv[1];
         delete [] argv;
      }
   }
   void longOption64()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=64");
         ASSERT_NOTHROW((integerType(2, argv)), "");
         ASSERT((integerType(2, argv) == IntegerType::Fixed64), "");
         delete [] argv[1];
         delete [] argv;
      }
   }
   void longOptionInf()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=inf");
         ASSERT_NOTHROW((integerType(2, argv)), "");
         ASSERT((integerType(2, argv) == IntegerType::Variable), "");
         delete [] argv[1];
         delete [] argv;
      }
   }
}

