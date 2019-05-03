
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_ROTATION
#include "algorithm_rotation.h"
#undef COMPILE_TEMPLATE_ALGORITHM_ROTATION

#include <algorithm>
#include <cassert>

#include "algorithm_classes.h"
#include "algorithm_fourier_motzkin_elimination.h"
#include "algorithm_inequality_operations.h"
#include "algorithm_integer_operations.h"
#include "algorithm_row_operations.h"

using namespace panda;

namespace
{
   /// Rotates a facet around a ridge. It's the exact same algorithm as for vertices.
   template <typename Integer>
   Facet<Integer> rotate(const Vertices<Integer>&, Vertex<Integer>, const Facet<Integer>&, Facet<Integer>);
   /// Returns all ridges on a facet (equivalent to all facets of the facet).
   template <typename Integer>
   Inequalities<Integer> getRidges(const Vertices<Integer>&, const Facet<Integer>&);
   /// Returns all vertices that lie on the facet (satisfy the inequality with equality).
   template <typename Integer>
   Vertices<Integer> verticesWithZeroDistance(const Vertices<Integer>&, const Facet<Integer>&);
}

template <typename Integer, typename TagType>
Matrix<Integer> panda::algorithm::rotation(const Matrix<Integer>& matrix,
                                    const Row<Integer>& input,
                                    const Maps& maps,
                                    TagType tag)
{
   // as the first step of the rotation, the furthest Vertex w.r.t. the input facet is calculated.
   // this will be the same vertex for all neighbouring ridges, hence, only needs to be computed once.
   const auto furthest_vertex = furthestVertex(matrix, input);
   const auto ridges = getRidges(matrix, input);
   std::set<Row<Integer>> output;
   for ( const auto& ridge : ridges )
   {
      const auto new_row = rotate(matrix, furthest_vertex, input, ridge);
      output.insert(new_row);
   }
   return classes(output, maps, tag);
}

namespace
{
   template <typename Integer>
   Facet<Integer> rotate(const Vertices<Integer>& vertices, Vertex<Integer> vertex, const Facet<Integer>& facet, Facet<Integer> ridge)
   {
      // the calculation of the initial vertex, which has to be the furthest vertex w.r.t. "facet", is calculated outside of this function as it is the same for all rotations.
      auto d_f = algorithm::distance(facet, vertex);
      auto d_r = algorithm::distance(ridge, vertex);
      do
      {
         const auto gcd_ds = algorithm::gcd(d_f, d_r);
         if ( gcd_ds > 1 )
         {
            d_f /= gcd_ds;
            d_r /= gcd_ds;
         }
         ridge = d_f * ridge - d_r * facet;
         const auto gcd_value = algorithm::gcd(ridge);
         assert( gcd_value != 0 );
         if ( gcd_value > 1 )
         {
            ridge /= gcd_value;
         }
         vertex = algorithm::nearestVertex(vertices, ridge);
         d_f = algorithm::distance(facet, vertex);
         d_r = algorithm::distance(ridge, vertex);
      }
      while ( d_r != 0 );
      return ridge;
   }

   template <typename Integer>
   Inequalities<Integer> getRidges(const Vertices<Integer>& vertices, const Facet<Integer>& facet)
   {
      const auto vertices_on_facet = verticesWithZeroDistance(vertices, facet);
      assert( !vertices_on_facet.empty() );
      return algorithm::fourierMotzkinElimination(vertices_on_facet);
   }

   template <typename Integer>
   Vertices<Integer> verticesWithZeroDistance(const Vertices<Integer>& vertices, const Facet<Integer>& facet)
   {
      Vertices<Integer> selection;
      std::copy_if(vertices.cbegin(), vertices.cend(), std::back_inserter(selection), [&facet](const Vertex<Integer>& vertex)
      {
         return (algorithm::distance(facet, vertex) == 0);
      });
      return selection;
   }
}

