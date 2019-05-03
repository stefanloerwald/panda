
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <set>
#include <vector>

#include "matrix.h"
#include "names.h"
#include "row.h"
#include "tags.h"

namespace panda
{
   template <typename Integer, typename TagType>
   class List
   {
      public:
         /// merges rows with the list of rows held in the list.
         void put(const Matrix<Integer>&) const;
         /// merges a row with the list of rows held in the list.
         void put(const Row<Integer>&) const;
         /// Returns a row that wasn't ever returned here before. Blocks the
         /// caller until data is available.
         Row<Integer> get() const;
         #pragma GCC diagnostic push
         #pragma GCC diagnostic ignored "-Weffc++"
         /// Constructor: special thing here: number of active workers is initialized
         /// to 1 (allowing heuristic to fill in once).
         List(const Names&);
         #pragma GCC diagnostic pop
      private:
         const Names names;
         mutable std::mutex mutex;
         mutable std::size_t workers;
         mutable std::condition_variable condition;
         mutable std::set<Row<Integer>> rows;
         using Iterator = typename std::set<Row<Integer>>::iterator;
         mutable std::vector<Iterator> iterators;
         mutable std::size_t counter;
      private:
         /// checks if all jobs are done.
         bool empty() const;
   };
}

#include "list.eti"

