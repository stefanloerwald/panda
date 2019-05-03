
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "method_double_description.h"

#include <algorithm>
#include <exception>
#include <iostream>

#include "algorithm_classes.h"
#include "algorithm_fourier_motzkin_elimination.h"
#include "algorithm_map_operations.h"
#include "algorithm_matrix_operations.h"
#include "algorithm_row_operations.h"
#include "application_name.h"
#include "input.h"
#include "integer_type_selection.h"

using namespace panda;

namespace
{
   /// Implementation of facet enumeration with double description.
   template <typename Integer>
   struct FacetEnumerationDoubleDescription
   {
      static int call(int, char**);
   };

   /// Implementation of vertex enumeration with double description.
   template <typename Integer>
   struct VertexEnumerationDoubleDescription
   {
      static int call(int, char**);
   };
}

template <>
int panda::method::doubleDescription<OperationMode::FacetEnumeration>(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   std::cerr << project::application_acronym << " -- facet enumeration with double description\n";
   return IntegerTypeSelector<FacetEnumerationDoubleDescription>::select(argc, argv);
}

template <>
int panda::method::doubleDescription<OperationMode::VertexEnumeration>(int argc, char** argv)
{
   assert( argc > 0 && argv != nullptr );
   std::cerr << project::application_acronym << " -- vertex enumeration with double description\n";
   return IntegerTypeSelector<VertexEnumerationDoubleDescription>::select(argc, argv);
}

namespace
{
   template <typename Integer>
   std::pair<std::vector<Vertex<Integer>*>, std::vector<Vertex<Integer>*>> split(Matrix<Integer>& matrix)
   {
      const auto n_rays = static_cast<std::size_t>(std::count_if(matrix.cbegin(), matrix.cend(), [](const Row<Integer>& r) { return r.back() == 0; }));
      std::vector<Vertex<Integer>*> conv;
      conv.reserve(matrix.size() - n_rays);
      std::vector<Vertex<Integer>*> cone;
      cone.reserve(n_rays);
      for ( auto& row : matrix )
      {
         if ( row.back() != 0 )
         {
            conv.push_back(&row);
         }
         else
         {
            cone.push_back(&row);
         }
      }
      return std::make_pair(conv, cone);
   }

   template <typename Integer>
   void print(Matrix<Integer> vertices, const bool is_reduced)
   {
      std::vector<Vertex<Integer>*> conv;
      std::vector<Vertex<Integer>*> cone;
      std::tie(conv, cone) = split(vertices);
      if ( !conv.empty() )
      {
         if ( is_reduced )
         {
            std::cout << "Reduced ";
         }
         std::cout << "Vertices:\n";
         for ( auto vertex : conv )
         {
            algorithm::printFractional(std::cout, *vertex);
            std::cout << '\n';
         }
      }
      if ( !cone.empty() )
      {
         if ( is_reduced )
         {
            std::cout << "Reduced ";
         }
         std::cout << "Rays:\n";
         for ( auto ray : cone )
         {
            ray->pop_back();
            std::cout << *ray << '\n';
         }
      }
   }


   template <typename Integer>
   void print(Matrix<Integer> inequalities, const Names& names, const bool is_reduced)
   {
      if ( is_reduced )
      {
         std::cout << "Reduced ";
      }
      std::cout << "Inequalities:\n";
      algorithm::prettyPrint(std::cout, inequalities, names, "<=");
   }

   template <typename Integer>
   int FacetEnumerationDoubleDescription<Integer>::call(int argc, char** argv)
   try
   {
      assert( argc > 0 && argv != nullptr );
      // input
      auto data = input::vertices<Integer>(argc, argv);
      const auto& vertices = std::get<0>(data);
      const auto& names = std::get<1>(data);
      const auto& maps = std::get<2>(data);
      // computation part 1: identifying equations
      const auto equations = algorithm::extractEquations(vertices);
      const auto reduced_maps = algorithm::normalize(maps, equations);
      if ( !equations.empty() )
      {
         std::cout << "Equations:\n";
         algorithm::prettyPrint(std::cout, equations, names, "=");
         std::cout << '\n';
      }
      // computation part 2: identifying inequalities
      auto inequalities = algorithm::fourierMotzkinElimination(vertices);
      inequalities = algorithm::classes(inequalities, reduced_maps, tag::facet{});
      // output
      const auto is_reduced = !maps.empty();
      print(std::move(inequalities), std::move(names), is_reduced);
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
   int VertexEnumerationDoubleDescription<Integer>::call(int argc, char** argv)
   try
   {
      assert( argc > 0 && argv != nullptr );
      // input
      auto data = input::inequalities<Integer>(argc, argv);
      const auto& inequalities = std::get<0>(data);
      const auto& maps = std::get<2>(data);
      // computation: identifying extremal vertices and rays
      auto matrix = algorithm::fourierMotzkinElimination(inequalities);
      matrix = algorithm::classes(matrix, maps, tag::vertex{});
      // output
      const auto is_reduced = !maps.empty();
      print(std::move(matrix), is_reduced);
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

