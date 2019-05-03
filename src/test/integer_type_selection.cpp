
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "integer_type_selection.h"

#include <cstring>
#include <stdexcept>

using namespace panda;

namespace
{
   template <typename Integer>
   struct Mock
   {
      static int call(int, char**);
   };

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
   SILENCE_CERR();
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
   template <typename Integer>
   int Mock<Integer>::call(int, char**)
   {
      return 0;
   }

   void noOption()
   {
      char** argv = new char*[1];
      argv[0] = nullptr;
      IntegerTypeSelector<Mock>::select(1, argv);
      delete [] argv;
   }
   void shortOptionBad()
   {
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[3];
         strcpy(argv[1], "-i");
         ASSERT_EXCEPTION((IntegerTypeSelector<Mock>::select(2, argv)), std::invalid_argument, "");
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
         ASSERT_EXCEPTION((IntegerTypeSelector<Mock>::select(3, argv)), std::invalid_argument, "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(3, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(3, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(3, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(3, argv)), "");
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
         ASSERT_EXCEPTION((IntegerTypeSelector<Mock>::select(2, argv)), std::invalid_argument, "");
         delete [] argv[1];
         delete [] argv;
      }
      {
         char** argv = new char*[2];
         argv[0] = nullptr;
         argv[1] = new char[19];
         strcpy(argv[1], "--integer-type=bad");
         ASSERT_EXCEPTION((IntegerTypeSelector<Mock>::select(2, argv)), std::invalid_argument, "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(2, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(2, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(2, argv)), "");
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
         ASSERT_NOTHROW((IntegerTypeSelector<Mock>::select(2, argv)), "");
         delete [] argv[1];
         delete [] argv;
      }
   }
}

