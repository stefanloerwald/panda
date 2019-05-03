
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <mutex>

#include "matrix.h"
#include "row.h"

namespace panda
{
   class Communication
   {
      public:
         /// Sending a facet to slave.
         template <typename Integer>
         void toSlave(const Row<Integer>&, const int) const;
         /// Receiving a facet from master.
         template <typename Integer>
         Row<Integer> fromMaster() const;
         /// Sending facets back to the master.
         template <typename Integer>
         void toMaster(const Matrix<Integer>&) const;
         /// Receiving facets from slave.
         template <typename Integer>
         Matrix<Integer> fromSlave(const int) const;
         #pragma GCC diagnostic push
         #pragma GCC diagnostic ignored "-Weffc++"
         /// Default constructor.
         Communication() = default;
         #pragma GCC diagnostic pop
      private:
         mutable std::mutex mutex;
   };

   template <>
   void Communication::toMaster<int>(const Matrix<int>&) const;
   template <>
   Matrix<int> Communication::fromSlave<int>(const int) const;

   template <>
   void Communication::toSlave<int>(const Row<int>&, const int) const;
   template <>
   Row<int> Communication::fromMaster<int>() const;
}

#include "communication.eti"

