
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "method_adjacency_decomposition.h"

#include <exception>
#include <iostream>

#include "application_name.h"
#include "delayed_action.h"
#include "input.h"
#include "integer_type_selection.h"
#include "job_manager.h"
#include "job_manager_proxy.h"
#include "message_passing_interface_session.h"
#include "method_adjacency_decomposition_implementation.h"

using namespace panda;

namespace
{
   /// Implementation of facet enumeration with adjacency decomposition.
   template <typename Integer>
   struct FacetEnumerationAdjacencyDecomposition
   {
      static int call(int, char**);
   };

   /// Implementation of vertex enumeration with adjacency decomposition.
   template <typename Integer>
   struct VertexEnumerationAdjacencyDecomposition
   {
      static int call(int, char**);
   };
}

template <>
int panda::method::adjacencyDecomposition<OperationMode::FacetEnumeration>(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   const auto& mpi_session = mpi::getSession();
   if ( mpi_session.isMaster() )
   {
      std::cerr << project::application_acronym << " -- facet enumeration with adjacency decomposition\n";
   }
   return IntegerTypeSelector<FacetEnumerationAdjacencyDecomposition>::select(argc, argv);
}

template <>
int panda::method::adjacencyDecomposition<OperationMode::VertexEnumeration>(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   const auto& mpi_session = mpi::getSession();
   if ( mpi_session.isMaster() )
   {
      std::cerr << project::application_acronym << " -- vertex enumeration with adjacency decomposition\n";
   }
   return IntegerTypeSelector<VertexEnumerationAdjacencyDecomposition>::select(argc, argv);
}

namespace
{
   template <typename Integer>
   int FacetEnumerationAdjacencyDecomposition<Integer>::call(int argc, char** argv)
   try
   {
      assert( argc > 0 && argv != nullptr );
      auto data = input::vertices<Integer>(argc, argv);
      const auto& mpi_session = mpi::getSession();
      if ( mpi_session.isMaster() )
      {
         implementation::adjacencyDecomposition<JobManager>(argc, argv, data, tag::facet{});
      }
      else
      {
         implementation::adjacencyDecomposition<JobManagerProxy>(argc, argv, data, tag::facet{});
      }
      return 0;
   }
   catch ( const std::exception& e )
   {
      std::cerr << "Exception caught: " << e.what() << '\n';
      return 1;
   }
   catch ( ... )
   {
      std::cerr << "Unknown exception caught in file " << __FILE__ << '\n';
      return 1;
   }

   template <typename Integer>
   int VertexEnumerationAdjacencyDecomposition<Integer>::call(int argc, char** argv)
   try
   {
      assert( argc > 0 && argv != nullptr );
      auto data = input::inequalities<Integer>(argc, argv);
      const auto& mpi_session = mpi::getSession();
      if ( mpi_session.isMaster() )
      {
         implementation::adjacencyDecomposition<JobManager>(argc, argv, data, tag::vertex{});
      }
      else
      {
         implementation::adjacencyDecomposition<JobManagerProxy>(argc, argv, data, tag::vertex{});
      }
      return 0;
   }
   catch ( const std::exception& e )
   {
      std::cerr << "Exception caught: " << e.what() << '\n';
      return 1;
   }
   catch ( ... )
   {
      std::cerr << "Unknown exception caught in file " << __FILE__ << '\n';
      return 1;
   }
}

