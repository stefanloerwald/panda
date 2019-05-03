
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_matrix_operations.h"

#include <sstream>

using namespace panda;

namespace
{
   void output();
   void mapping();
   void dimension();
   void gaussian_elimination();
   void transposition();
}

int main()
try
{
   output();
   mapping();
   dimension();
   gaussian_elimination();
   transposition();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void output()
   {
      Matrix<int> m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
      std::stringstream stream;
      stream << m;
      int t;
      for ( int i = 1; i < 10; ++i )
      {
         ASSERT((stream >> t) && t == i, "Data mismatch.");
      }
      ASSERT(!(stream >> t), "Too much data available.");
   }

   void mapping()
   {
      Matrix<int> m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
      ASSERT((m * Row<int>{1, 0, 0} == Row<int>{1, 4, 7}), "Data mismatch.");
      ASSERT((m * Row<int>{0, 1, 0} == Row<int>{2, 5, 8}), "Data mismatch.");
      ASSERT((m * Row<int>{0, 0, 1} == Row<int>{3, 6, 9}), "Data mismatch.");
      ASSERT((m == Matrix<int>{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}), "Modification of matrix disallowed.");
      ASSERT((m * Row<int>{1, 1, 1} == Row<int>{6, 15, 24}), "Data mismatch.");
   }

   void dimension()
   {
      ASSERT(algorithm::dimension(Matrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}) == 3, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 1, 1}}) == 3, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 1, 0}}) == 2, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {2, -1, 0}}) == 2, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 2, 3}, {0, 1, 0}, {1, 0, 0}}) == 3, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 2, 3}, {0, 1, 0}, {0, 0, 1}, {0, 0, 1}}) == 3, "");
      ASSERT(algorithm::dimension(Matrix<int>{{1, 2, 3}, {0, 1, 0}}) == 2, "");
      ASSERT(algorithm::dimension(Matrix<int>{{0, 0, 0}, {0, 1, 0}}) == 1, "");
   }

   void gaussian_elimination()
   {
      Matrix<int> m{{1, 2, 4}, {1, 3, 9}, {1, 5, 25}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
      ASSERT_NOTHROW(algorithm::gaussianElimination(m), "Gaussian elimination may not throw.");
      m.erase(m.begin(), m.begin() + 3);
      Matrix<int> o{{1, 2, 4}, {1, 3, 9}, {1, 5, 25}};
      Matrix<int> r(3, Row<int>(3, 0));
      for ( std::size_t i = 0; i < 3; ++i )
      {
         for ( std::size_t j = 0; j < 3; ++j )
         {
            for ( std::size_t k = 0; k < 3; ++k )
            {
               r[i][j] += o[i][k] * m[k][j];
            }
         }
      }
      ASSERT(r[0][0] != 0 && r[1][1] != 0 && r[2][2] != 0, "Data mismatch.");
      r[0][0] = 0;
      r[1][1] = 0;
      r[2][2] = 0;
      ASSERT((r == Matrix<int>{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}), "Data mismatch");
   }

   void transposition()
   {
      Matrix<int> m{{1, 5}, {2, 6}, {3, 7}, {4, 8}};
      auto mt = algorithm::transpose(m);
      ASSERT((m == Matrix<int>{{1, 5}, {2, 6}, {3, 7}, {4, 8}}), "transpose is not allowed to modify inplace");
      ASSERT((mt == Matrix<int>{{1, 2, 3, 4}, {5, 6, 7, 8}}), "Data mismatch.");
   }
}

