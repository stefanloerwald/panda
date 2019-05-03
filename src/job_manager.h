
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <list>

#include "communication.h"
#include "joining_thread.h"
#include "list.h"
#include "matrix.h"
#include "names.h"
#include "row.h"
#include "tags.h"

namespace panda
{
   template <typename Integer, typename TagType>
   class JobManager
   {
      public:
         /// merges all rows with the list of rows held in the pool.
         void put(const Matrix<Integer>&) const;
         /// merges a row with the list of rows held in the pool.
         void put(const Row<Integer>&) const;
         /// Returns a job that wasn't ever returned here before. Blocks the
         /// caller until data is available.
         Row<Integer> get() const;
         /// Constructor. The first argument are the names of indices
         /// (only relevant for printing inequalities).
         /// The second argument must be the number of processors,
         /// the third argument must be the number of threads per processor.
         JobManager(const Names&, const int, const int);
      private:
         Communication communication;
         mutable List<Integer, TagType> rows;
         mutable std::list<JoiningThread> request_threads;
      private:
         /// Copy construction is not allowed.
         JobManager(const JobManager<Integer, TagType>&) = delete;
         /// Copy assignment is not allowed.
         JobManager<Integer, TagType>& operator=(const JobManager<Integer, TagType>&) = delete;
   };
}

#include "job_manager.eti"

