
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "range.h"

#include <vector>

using namespace panda;

int main()
try
{
   std::vector<int> v{1, 2, 3, 4, 5};
   int c = 5;
   for ( const auto& e : makeReverseRange(v) ) // observing
   {
      ASSERT(c == e, "reverse range entry mismatch.");
      --c;
   }
   for ( const auto& e : v ) // observing
   {
      ++c;
      ASSERT(c == e, "reverse range mutated, but shouldn't be.");
   }
   for ( auto& e : makeReverseRange(v) ) // mutating
   {
      ++e;
   }
   c = 1;
   for ( const auto e : v ) // observing
   {
      ++c;
      ASSERT(c == e, "reverse range not mutated, but should be.");
   }
   for ( auto e : makeReverseRange(v) ) // mutating copies
   {
      ++e;
   }
   c = 1;
   for ( const auto& e : v ) // observing
   {
      ++c;
      ASSERT(c == e, "reverse range mutated, but shouldn't be.");
   }
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

