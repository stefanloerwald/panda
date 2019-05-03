
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_classes.h"

#include "algorithm_row_operations.h"

using namespace panda;

namespace
{
   void facet_class();
   void representative();
}

int main()
try
{
   facet_class();
   representative();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void facet_class()
   {
      Map xy{{std::make_pair(1u, 1)}, {std::make_pair(0u, 1)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}};
      Map x{{std::make_pair(0u, -1), std::make_pair(3u, 1)},{std::make_pair(1u, 1)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}};
      Facet<int> facet{1, 0, 0, -1};
      ASSERT_NOTHROW(algorithm::getClass(facet, {xy, x}, tag::facet{}), "");
      auto facets = algorithm::getClass(facet, {xy, x}, tag::facet{});
      ASSERT((facets == std::set<Facet<int>>{{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 1, 0, -1}, {1, 0, 0, -1}}), "");
   }

   void representative()
   {
      Map xy{{std::make_pair(1u, 1)}, {std::make_pair(0u, 1)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}};
      Map x{{std::make_pair(0u, -1), std::make_pair(3u, 1)},{std::make_pair(1u, 1)}, {std::make_pair(2u, 1)}, {std::make_pair(3u, 1)}};
      Facet<int> facet{1, 0, 0, -1};
      ASSERT_NOTHROW(algorithm::classRepresentative(facet, {xy, x}, tag::facet{}), "");
      const auto rep = algorithm::classRepresentative(facet, {xy, x}, tag::facet{});
      ASSERT((rep == Facet<int>{1, 0, 0, -1}), "");
   }
}

