
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "input_constraint_named_variables.h"

#include <sstream>
#include <stdexcept>

using namespace panda;
using input::implementation::constraint;

#include "algorithm_row_operations.h"

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
      const Names names = {"x", "y", "z"};
      for ( const auto string : {"x -y +2z <= 1",
                                 "+x -1y 2z <= 1",
                                 "-x +y -2z >= -1",
                                 "-x +y -2z >= -1"} )
      {
         std::istringstream stream(string);
         ASSERT_NOTHROW(constraint<ConstraintType::Inequality>(stream, names), "Reading valid data may not throw.");
      }
      for ( const auto string : {"-y +x 2z = 1",
                                 "-y +x 2z == 1"} )
      {
         std::istringstream stream(string);
         ASSERT_NOTHROW(constraint<ConstraintType::Equation>(stream, names), "Reading valid data may not throw.");
      }
      for ( const auto string : {"x -y +2z <= 1",
                                 "+x -1y 2z <= 1",
                                 "-x +y -2z >= -1",
                                 "-x +y -2z >= -1"} )
      {
         std::istringstream stream(string);
         ASSERT((constraint<ConstraintType::Inequality>(stream, names) == Row<int>{1, -1, 2, -1}), "Data mismatch.");
      }
      for ( const auto string : {"-y +x 2z = 1",
                                 "-y +x 2z == 1"} )
      {
         std::istringstream stream(string);
         ASSERT((constraint<ConstraintType::Equation>(stream, names) == Row<int>{1, -1, 2, -1}), "Data mismatch.");
      }
   }

   void invalid()
   {
      const Names names = {"x", "y", "z"};
      for ( const auto pair : {std::make_pair("x y a <= 1", "Bad variable."),
                               std::make_pair("x -1 y z <= 2", "Summand on left hand side."),
                               std::make_pair("x -x <= 1", "Two entries for same variable."),
                               std::make_pair("x y z <= 1 +1", "Two entries on right hand side."),
                               std::make_pair("x y z <? 1", "Bad relation operator."),
                               std::make_pair("x y z < 1", "Bad relation operator."),
                               std::make_pair("x y z > 1", "Bad relation operator."),
                               std::make_pair("x y z > = 1", "Bad relation operator."),
                               std::make_pair("x y z < = 1", "Bad relation operator."),
                               std::make_pair("x y z <= >= 1", "Two relation operators.")} )
      {
         std::istringstream stream(pair.first);
         ASSERT_EXCEPTION(constraint<ConstraintType::Inequality>(stream, names), std::invalid_argument, pair.second);
      }
   }
}

