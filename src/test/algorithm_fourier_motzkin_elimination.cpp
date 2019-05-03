
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_fourier_motzkin_elimination.h"

#include <algorithm>
#include <cassert>
#include <random>

using namespace panda;

namespace
{
   void facetsConvexOnly();
   void vertices();
}

int main()
try
{
   facetsConvexOnly();
   vertices();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void facetsConvexOnly()
   {
      {
         Vertices<int> triangle{{0, 0, 1}, {1, 0, 1}, {0, 1, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(triangle.begin(), triangle.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(triangle);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, 0, 0}, {0, -1, 0}, {1, 1, -1}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
      {
         Vertices<int> triangle{{1, 1, 1}, {1, 0, 1}, {0, 1, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(triangle.begin(), triangle.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(triangle);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, -1, 1}, {0, 1, -1}, {1, 0, -1}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
      {
         Vertices<int> square{{0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(square.begin(), square.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(square);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, 0, 0}, {0, -1, 0}, {0, 1, -1}, {1, 0, -1}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
      {
         Vertices<int> square{{1, 1, 1}, {2, 1, 1}, {1, 2, 1}, {2, 2, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(square.begin(), square.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(square);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, 0, 1}, {0, -1, 1}, {0, 1, -2}, {1, 0, -2}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
      {
         Vertices<int> octagon{{0, 0, 1}, {1, 0, 1}, {-1, 1, 1}, {2, 1, 1}, {-1, 2, 1}, {2, 2, 1}, {0, 3, 1}, {1, 3, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(octagon.begin(), octagon.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(octagon);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, -1, 0},
                                   {-1, 0, -1},
                                   {-1, 1, -3},
                                   {0, -1, 0},
                                   {0, 1, -3},
                                   {1, -1, -1},
                                   {1, 0, -2},
                                   {1, 1, -4}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
      {
         Vertices<int> octagon{{1, 1, 1}, {2, 1, 1}, {0, 2, 1}, {3, 2, 1}, {0, 3, 1}, {3, 3, 1}, {1, 4, 1}, {2, 4, 1}};
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(octagon.begin(), octagon.end(), g);
         auto facets = algorithm::fourierMotzkinElimination(octagon);
         std::sort(facets.begin(), facets.end());
         const Facets<int> correct{{-1, -1, 2},
                                   {-1, 0, 0},
                                   {-1, 1, -3},
                                   {0, -1, 1},
                                   {0, 1, -4},
                                   {1, -1, -1},
                                   {1, 0, -3},
                                   {1, 1, -6}};
         assert(std::is_sorted(correct.cbegin(), correct.cend()));
         ASSERT(facets == correct, "Data mismatch.");
      }
   }

   void vertices()
   {
      {
         Inequalities<int> ieqs{{1, 0, -1}, {-1, 0, 0}, {0, 1, -1}, {0, -1, 0}};
         auto vs = algorithm::fourierMotzkinElimination(ieqs);
         std::sort(vs.begin(), vs.end());
         Vertices<int> correct{{0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
         assert( std::is_sorted(correct.cbegin(), correct.cend()) );
         ASSERT(vs == correct, "Data mismatch.");
      }
      {
         Inequalities<int> ieqs{{1, 1, -2}, {-1, 0, 0}, {0, -1, 0}};
         auto vs = algorithm::fourierMotzkinElimination(ieqs);
         std::sort(vs.begin(), vs.end());
         Vertices<int> correct{{0, 0, 1}, {0, 2, 1}, {2, 0, 1}};
         assert( std::is_sorted(correct.cbegin(), correct.cend()) );
         ASSERT(vs == correct, "Data mismatch.");
      }
      {
         // redundant ieqs
         Inequalities<int> ieqs{{1, 1, -2}, {1, 1, -3}, {-1, 0, 0}, {0, -1, 0}};
         auto vs = algorithm::fourierMotzkinElimination(ieqs);
         std::sort(vs.begin(), vs.end());
         Vertices<int> correct{{0, 0, 1}, {0, 2, 1}, {2, 0, 1}};
         assert( std::is_sorted(correct.cbegin(), correct.cend()) );
         ASSERT(vs == correct, "Data mismatch.");
      }
      {
         // with equation
         Inequalities<int> ieqs{{1, 0, 0}, {0, 1, -3}, {-1, 0, 0}, {0, -1, 0}};
         auto vs = algorithm::fourierMotzkinElimination(ieqs);
         std::sort(vs.begin(), vs.end());
         Vertices<int> correct{{0, 0, 1}, {0, 3, 1}};
         assert( std::is_sorted(correct.cbegin(), correct.cend()) );
         ASSERT(vs == correct, "Data mismatch.");
      }
      {
         // with equation
         Inequalities<int> ieqs{{2, 0, -1}, {0, 2, -1}, {-1, 0, 0}, {0, -1, 0}};
         auto vs = algorithm::fourierMotzkinElimination(ieqs);
         std::sort(vs.begin(), vs.end());
         Vertices<int> correct{{0, 0, 1}, {0, 1, 2}, {1, 0, 2}, {1, 1, 2}};
         assert( std::is_sorted(correct.cbegin(), correct.cend()) );
         ASSERT(vs == correct, "Data mismatch.");
      }
   }
}

