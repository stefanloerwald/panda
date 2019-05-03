
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "matrix.h"
#include "row.h"

namespace panda
{
   namespace algorithm
   {
      /// returns the value of the right hand side of an inequality.
      template <typename Integer>
      Integer rhs(const Inequality<Integer>&) noexcept;
      /// returns the distance of a vertex to a face defined by an inequality.
      /// Note that within the rotation algorithm, the result of this method may be negative (which is intended).
      template <typename Integer>
      Integer distance(const Inequality<Integer>&, const Vertex<Integer>&) noexcept;
      /// returns a vertex that maximizes the distance function.
      template <typename Integer>
      Vertex<Integer> furthestVertex(const Vertices<Integer>&, const Inequality<Integer>&);
      /// returns a vertex that minimizes the distance function.
      template <typename Integer>
      Vertex<Integer> nearestVertex(const Vertices<Integer>&, const Inequality<Integer>&);
   }
}

#include "algorithm_inequality_operations.eti"

