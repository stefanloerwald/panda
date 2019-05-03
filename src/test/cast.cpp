
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "cast.h"

#include <climits>
#include <cstdint>

using namespace panda;

// Selecting an arbitrary integer type which is not equal to int.
#if INT_MIN != SHRT_MIN
using Integer = short;
#elif INT_MIN != LONG_MIN
using Integer = long;
#elif INT_MIN != LLONG_MIN
using Integer = long long;
#endif

#include "cast.h"

int main()
try
{
   #ifndef NO_FLEXIBILITY
   Row<int> row{0, 1, 2};
   Row<Integer> lrow = cast<Integer>(row);
   ASSERT(lrow.size() == row.size(), "Casting affected size.");
   ASSERT((lrow[0] == 0 && lrow[1] == 1 && lrow[2] == 2), "Casting result does not represent the same numbers.");
   ASSERT(cast<int>(lrow) == row, "Casting back should result in the same values");
   Matrix<int> matrix{row, row, row};
   Matrix<Integer> lmatrix = cast<Integer>(matrix);
   const auto message = "Casting result does not represent the same numbers.";
   ASSERT((lmatrix == Matrix<Integer>{{0, 1, 2}, {0, 1, 2}, {0, 1, 2}}), message);
   ASSERT(cast<int>(lmatrix) == matrix, "Casting back should result in the same values");
   #else
   ASSERT(true, "nothing to test here.");
   #endif
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

