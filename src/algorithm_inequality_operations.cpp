
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_INEQUALITY_OPERATIONS
#include "algorithm_inequality_operations.h"
#undef COMPILE_TEMPLATE_ALGORITHM_INEQUALITY_OPERATIONS

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>

using namespace panda;

namespace
{
   /// Calculates the scalar product of the left hand side coefficients of an inequality with a vertex.
   template <typename Integer>
   Integer scalarProduct(const Inequality<Integer>&, const Vertex<Integer>&) noexcept;
}

template <typename Integer>
Integer panda::algorithm::rhs(const Inequality<Integer>& inequality) noexcept
{
   assert( !inequality.empty() );
   return -inequality.back();
}

template <typename Integer>
Integer panda::algorithm::distance(const Inequality<Integer>& inequality, const Vertex<Integer>& vertex) noexcept
{
   return -scalarProduct(inequality, vertex);
}

namespace
{
   template <typename Integer, typename Comparator>
   Vertex<Integer> extremalVertex(const Vertices<Integer>& vertices, const Inequality<Integer>& inequality, Comparator&& comparator)
   {
      assert( !vertices.empty() );
      auto extremum = algorithm::distance(inequality, *vertices.cbegin());
      auto best_vertex = vertices.cbegin();
      for ( auto it = vertices.cbegin() + 1; it != vertices.cend(); ++it )
      {
         const auto& vertex = *it;
         const auto d = algorithm::distance(inequality, vertex);
         if ( comparator(d, extremum) )
         {
            best_vertex = it;
            extremum = d;
         }
      }
      return *best_vertex;
   }
}

template <typename Integer>
Vertex<Integer> algorithm::furthestVertex(const Vertices<Integer>& vertices, const Inequality<Integer>& inequality)
{
   return extremalVertex(vertices, inequality, std::greater<Integer>{});
}

template <typename Integer>
Vertex<Integer> algorithm::nearestVertex(const Vertices<Integer>& vertices, const Inequality<Integer>& inequality)
{
   return extremalVertex(vertices, inequality, std::less<Integer>{});
}

namespace
{
   template <typename Integer>
   Integer scalarProduct(const Inequality<Integer>& inequality, const Vertex<Integer>& vertex) noexcept
   {
      assert( inequality.size() == vertex.size() );
      return std::inner_product(vertex.cbegin(), vertex.cend(), inequality.cbegin(), Integer{0});
   }
}

