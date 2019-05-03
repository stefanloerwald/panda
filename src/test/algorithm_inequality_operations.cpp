
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "algorithm_inequality_operations.h"

using namespace panda;

namespace
{
   void right_hand_side();
   void distances();
   void furthest_vertex();
   void nearest_vertex();
}

int main()
try
{
   right_hand_side();
   distances();
   furthest_vertex();
   nearest_vertex();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void right_hand_side()
   {
      ASSERT(algorithm::rhs(Inequality<int>{1, 0, -1}) == 1, "Bad rhs.");
   }
   void distances()
   {
      ASSERT((algorithm::distance(Inequality<int>{1, 0, -1}, Vertex<int>{ 0, 0, 1}) ==  1), "Bad distance.");
      ASSERT((algorithm::distance(Inequality<int>{1, 0, -1}, Vertex<int>{-1, 1, 1}) ==  2), "Bad distance.");
      ASSERT((algorithm::distance(Inequality<int>{1, 0, -1}, Vertex<int>{ 1, 0, 1}) ==  0), "Bad distance.");
      ASSERT((algorithm::distance(Inequality<int>{1, 0, -1}, Vertex<int>{ 2, 1, 1}) == -1), "Bad distance.");
   }
   void furthest_vertex()
   {
      Vertices<int> vs{{0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
      ASSERT((algorithm::furthestVertex(vs, Inequality<int>{1, 0, -1}) == Vertex<int>{0, 0, 1}) ||
             (algorithm::furthestVertex(vs, Inequality<int>{1, 0, -1}) == Vertex<int>{0, 1, 1}), "");
   }
   void nearest_vertex()
   {
      Vertices<int> vs{{0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
      ASSERT((algorithm::nearestVertex(vs, Inequality<int>{1, 0, -1}) == Vertex<int>{1, 0, 1}) ||
             (algorithm::nearestVertex(vs, Inequality<int>{1, 0, -1}) == Vertex<int>{1, 1, 1}), "");
   }
}

