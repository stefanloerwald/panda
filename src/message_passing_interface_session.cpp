
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "message_passing_interface_session.h"

#include <cassert>

#ifdef MPI_SUPPORT
#include "mpi_no_warnings.h"
#endif

using namespace panda;

mpi::Session& panda::mpi::getSession() noexcept
{
   // the keyword static asserts thread safe initialization. Only this one session will be created.
   static mpi::Session session;
   return session;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"

bool panda::mpi::Session::isMaster() const noexcept
{
   #ifdef MPI_SUPPORT
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   return rank == 0;
   #else
   return true;
   #endif
}

int panda::mpi::Session::getNumberOfNodes() const noexcept
{
   #ifdef MPI_SUPPORT
   int size;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   assert( size > 0 );
   return size;
   #else
   return 1;
   #endif
}

#pragma clang diagnostic pop

panda::mpi::Session::Session() noexcept
{
   #ifdef MPI_SUPPORT
   MPI_Init(nullptr, nullptr);
   #endif
}

panda::mpi::Session::~Session()
{
   #ifdef MPI_SUPPORT
   MPI_Finalize();
   #endif
}

