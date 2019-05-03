
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>

#include "communication.h"
#include "matrix.h"
#include "names.h"
#include "row.h"
#include "tags.h"

namespace panda
{
   /// For a slave node, there is no JobManager in shared memory.
   /// Instead, this proxy uses a communicator to forward the request to the master.
   /// The class exhibits the same interface to have templated code on master and slaves.
   template <typename Integer, typename TagType>
   class JobManagerProxy
   {
      public:
         /// Merges all rows with the list of rows held in the pool.
         void put(const Matrix<Integer>&) const;
         /// Returns facet that wasn't ever returned here before. Blocks the caller until data is available.
         Row<Integer> get() const;
         /// Constructor. The arguments are deliberately ignored in JobManagerProxy.
         JobManagerProxy(const Names&, const int, const int);
      private:
         Communication communication;
   };
}

#include "job_manager_proxy.eti"

