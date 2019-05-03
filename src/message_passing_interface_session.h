
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>

namespace panda
{
   namespace mpi
   {
      class Session;

      /// Returns a reference to the MPI session.
      Session& getSession() noexcept;

      class Session
      {
         public:
            /// Returns true if the node ID is zero.
            bool isMaster() const noexcept;
            /// Returns the number of nodes in the setup.
            int getNumberOfNodes() const noexcept;
            friend mpi::Session& mpi::getSession() noexcept;
         private:
            /// Constructor.
            Session() noexcept;
            /// Destructor.
            ~Session();
      };
   }
}

