
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "input_constraint_unnamed_variables.h"

#include <sstream>
#include <stdexcept>

using namespace panda;
using input::implementation::constraint;

namespace
{
   void valid();
   void invalid();
}

int main()
try
{
   valid();
   invalid();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void valid()
   {
      for ( const auto string : {"   1 \t2 -3  -10\t 1",
                                 "   1 \t2 -3  -10\t <= -1",
                                 "   1 \t2 -3  -10\t <= -1",
                                 "   -1 \t-2 3  10\t >= 1",
                                 "   -1 \t-2 3  10\t >= 1",
                                 " ( 1 )  1 \t2 -3  -10\t 1",
                                 " ( a b)  1 \t2 -3  -10\t <= -1",
                                 " ()  1 \t2 -3  -10\t <= -1",
                                 " (5)  -1 \t-2 3  10\t >= 1",
                                 " (6)  -1 \t-2 3  10\t >= 1"} )
      {
         {
            std::istringstream stream(string);
            ASSERT_NOTHROW(constraint<ConstraintType::Inequality>(stream), "On valid input, constraint read method may not throw.");
         }
         {
            std::istringstream stream(string);
            ASSERT((constraint<ConstraintType::Inequality>(stream) == Row<int>{1, 2, -3, -10, 1}), "Data mismatch.");
         }
      }
      for ( const auto string : {"   1 \t2 -3  -10\t 1",
                                 "   1 \t2 -3  -10\t = -1",
                                 "   1 \t2 -3  -10\t == -1",
                                 " (3 )  1 \t2 -3  -10\t = -1",
                                 " ( 44)  1 \t2 -3  -10\t == -1"} )
      {
         {
            std::istringstream stream(string);
            ASSERT_NOTHROW(constraint<ConstraintType::Equation>(stream), "On valid input, constraint read method may not throw.");
         }
         {
            std::istringstream stream(string);
            ASSERT((constraint<ConstraintType::Equation>(stream) == Row<int>{1, 2, -3, -10, 1}), "Data mismatch.");
         }
      }
      {
         std::stringstream stream("");
         constraint<ConstraintType::Inequality>(stream);
         ASSERT_NOTHROW(constraint<ConstraintType::Inequality>(stream), "Empty line may not throw exception.");
      }
      {
         std::stringstream stream("");
         constraint<ConstraintType::Inequality>(stream);
         ASSERT(constraint<ConstraintType::Inequality>(stream).empty(), "Empty line must result in empty row.");
      }
   }

   void invalid()
   {
      for ( const auto string : {" 1 \t2 -3 -10 \t <=",
                                 " 1 \t2 -3 -10 \t <= ",
                                 " 1 \t2 -3 -10 \t < ",
                                 " 1 \t2 -3 -10 \t <",
                                 " 1 \t2 -3 -10 \t =",
                                 " 1 \t2 -3 -10 \t = ",
                                 " 1 \t2 -3 -10 \t ==",
                                 " 1 \t2 -3 -10 \t == ",
                                 " 1 \t2 -3 -10 \t >",
                                 " 1 \t2 -3 -10 \t > ",
                                 " 1 \t2 -3 -10 \t >=",
                                 " 1 \t2 -3 -10 \t >= ",
                                 " 1 \t2 -3 -10 \t >= \n 2"} )
      {
         std::stringstream stream(string);
         const auto message = "Invalid input must result in std::invalid_argument (missing right hand side).";
         ASSERT_EXCEPTION(constraint<ConstraintType::Inequality>(stream), std::invalid_argument, message);
      }
      for ( const auto string : {"   1 \t2 -3  -10\t < 0",
                                 "   1 \t2 -3  -10\t < -1",
                                 "   1 \t2 -3  -10\t ? -1",
                                 "   -1 \t-2 3  10\t > 1",
                                 "   -1 \t-2 3  10\t > 0",
                                 " ( a b)  1 \t2 -3  -10\t < 0",
                                 " ()  1 \t2 -3  -10\t < -1",
                                 " (5)  -1 \t-2 3  10\t > 1",
                                 " (6)  -1 \t-2 3  10\t > 0"} )
      {
         {
            std::istringstream stream(string);
            ASSERT_ANY_EXCEPTION(constraint<ConstraintType::Inequality>(stream), "If an invalid relation operator is used, an exception must be thrown.");
         }
      }
      for ( const auto string : {" 1 \t a 2",
                                 " (",
                                 "( 1 2 3"} )
      {
         std::stringstream stream(string);
         std::string message = "Invalid input must result in std::invalid_argument";
         message += " (invalid character or identifier).";
         ASSERT_EXCEPTION(constraint<ConstraintType::Inequality>(stream), std::invalid_argument, message);
      }
   }
}

