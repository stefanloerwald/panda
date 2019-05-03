
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <set>

#include "maps.h"
#include "matrix.h"
#include "row.h"
#include "tags.h"

namespace panda
{
   namespace algorithm
   {
      /// returns the unique class representative. The class is generated with the provided maps.
      /// Precondition: if input is a facet, the facet must be normalized.
      template <typename Integer, typename TagType>
      Row<Integer> classRepresentative(const Row<Integer>&, const Maps&, TagType);
      /// Creates a set of rows which is the complete class containing the input row.
      /// Precondition: if input is a facet, the facet must be normalized.
      template <typename Integer, typename TagType>
      std::set<Row<Integer>> getClass(const Row<Integer>&, const Maps&, TagType);
      /// Reduces a list of rows to just the representatives.
      /// Precondition: if input are facets, then these facets must be normalized.
      template <typename Integer, typename TagType>
      Matrix<Integer> classes(Matrix<Integer>, const Maps&, TagType);
      /// Reduces a list of rows to just the representatives.
      /// Precondition: if input are facets, then these facets must be normalized.
      template <typename Integer, typename TagType>
      Matrix<Integer> classes(std::set<Row<Integer>>, const Maps&, TagType);
   }
}

#include "algorithm_classes.eti"

