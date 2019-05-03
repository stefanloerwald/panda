
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_validity.h"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

#include "algorithm_inequality_operations.h"
#include "algorithm_map_operations.h"
#include "algorithm_matrix_operations.h"
#include "algorithm_row_operations.h"

using namespace panda;

namespace
{
   bool inequalityIsValid(const Matrix<int>&, const Inequality<int>&, const std::size_t);
   void checkValidityOfInequality(const Matrix<int>&, const Inequality<int>&, const std::size_t);
   void checkValidityOfVertex(const Matrix<int>&, const Vertex<int>&, const std::size_t);
   void checkValidityInequalities(const Matrix<int>&, const Map&);
   void checkValidityVertices(const Matrix<int>&, const Map&);
}

/// Input is considered valid if and only if each inequality is facet-defining.
void panda::input::implementation::checkValidityOfInequalities(const Matrix<int>& matrix, const Inequalities<int>& inequalities)
{
   if ( inequalities.empty() )
   {
      return;
   }
   const auto dimension = algorithm::dimension(matrix);
   for ( const auto& inequality : inequalities )
   {
      ::checkValidityOfInequality(matrix, inequality, dimension);
   }
}

/// Input is considered valid if and only if each vertex / ray is extremal.
void panda::input::implementation::checkValidityOfVertices(const Matrix<int>& matrix, const Vertices<int>& vertices)
{
   if ( vertices.empty() )
   {
      return;
   }
   const auto dimension = algorithm::dimension(matrix);
   for ( const auto& vertex : vertices )
   {
      ::checkValidityOfVertex(matrix, vertex, dimension);
   }
}

/// Input is considered valid if and only if each map is a bijection on the set of inequalities.
void panda::input::implementation::checkValidityOfInequalityClasses(const Inequalities<int>& inequalities, const Maps& maps)
{
   for ( const auto& map : maps )
   {
      checkValidityInequalities(inequalities, map);
   }
}

/// Input is considered valid if and only if each map is a bijection on the set of vertices.
void panda::input::implementation::checkValidityOfVertexClasses(const Vertices<int>& vertices, const Maps& maps)
{
   for ( const auto& map : maps )
   {
      checkValidityVertices(vertices, map);
   }
}

/// Input is considered valid if and only if each inequality is facet-defining.
void panda::input::implementation::filterInvalidInequalities(const Matrix<int>& matrix, Inequalities<int>& inequalities)
{
   if ( inequalities.empty() )
   {
      return;
   }
   const auto dimension = algorithm::dimension(matrix);
   for ( auto it = inequalities.begin(); it != inequalities.end(); )
   {
      const auto& inequality = *it;
      if ( ::inequalityIsValid(matrix, inequality, dimension) )
      {
         ++it;
      }
      else
      {
         it = inequalities.erase(it);
      }
   }
}

/// Input is considered valid analogous to the case of inequalities.
void panda::input::implementation::filterInvalidVertices(const Matrix<int>& matrix, Vertices<int>& vertices)
{
   filterInvalidInequalities(matrix, vertices);
}

bool panda::input::checkValidity(int argc, char** argv) noexcept
{
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-c") == 0 || std::strcmp(argv[i], "--check") == 0 )
      {
         return true;
      }
   }
   return false;
}

bool panda::input::filterInvalidInput(int argc, char** argv) noexcept
{
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-f") == 0 || std::strcmp(argv[i], "--filter") == 0 )
      {
         return true;
      }
   }
   return false;
}

namespace
{
   bool inequalityIsValid(const Matrix<int>& matrix, const Inequality<int>& inequality, const std::size_t dimension)
   {
      // identify vertices and rays that satisfy the inequality with equality
      // check if any distance is less than 0
      Matrix<int> active;
      for ( const auto& row : matrix )
      {
         const auto dist = algorithm::distance(inequality, row);
         if ( dist < 0 )
         {
            return false;
         }
         if ( dist == 0 )
         {
            active.push_back(row);
         }
      }
      // The resulting set of active vertices must be non-empty and have the correct dimension.
      return !active.empty() && (algorithm::dimension(active) + 1 == dimension);
   }

   void checkValidityOfInequality(const Matrix<int>& matrix, const Inequality<int>& inequality, const std::size_t dimension)
   {
      // identify vertices and rays that satisfy the inequality with equality
      // check if any distance is less than 0
      Matrix<int> active;
      for ( const auto& row : matrix )
      {
         const auto dist = algorithm::distance(inequality, row);
         if ( dist < 0 )
         {
            std::stringstream stream;
            stream << "The inequality " << inequality << " separates the vertex / ray " << row << " from the polytope.";
            throw std::invalid_argument(stream.str());
         }
         if ( dist == 0 )
         {
            active.push_back(row);
         }
      }
      if ( active.empty() )
      {
         std::stringstream stream;
         stream << "The inequality " << inequality << " is a valid inequality, but does not define a facet, as it is a strict inequality.";
         throw std::invalid_argument(stream.str());
      }
      if ( algorithm::dimension(active) + 1 != dimension )
      {
         std::stringstream stream;
         stream << "The inequality " << inequality << " is not a facet, but only a lower-dimensional face.";
         throw std::invalid_argument(stream.str());
      }
   }

   void checkValidityOfVertex(const Matrix<int>& matrix, const Vertex<int>& vertex, const std::size_t dimension)
   {
      Matrix<int> active;
      for ( const auto& row : matrix )
      {
         const auto dist = algorithm::distance(row, vertex);
         if ( dist < 0 )
         {
            std::stringstream stream;
            stream << "The inequality " << row << " is not satisfied by " << vertex << '.';
            throw std::invalid_argument(stream.str());
         }
         if ( dist == 0 )
         {
            active.push_back(row);
         }
      }
      if ( active.empty() )
      {
         std::stringstream stream;
         stream << "The row " << vertex << " is a valid vector, but is not extremal.";
         throw std::invalid_argument(stream.str());
      }
      if ( algorithm::dimension(active) + 1 != dimension )
      {
         std::stringstream stream;
         stream << "The row " << vertex << " is not extremal.";
         throw std::invalid_argument(stream.str());
      }
   }

   void checkValidityInequalities(const Matrix<int>& matrix, const Map& map)
   {
      std::vector<std::size_t> indices(matrix.size());
      for ( std::size_t i = 0; i < matrix.size(); ++i )
      {
         const auto& row = matrix[i];
         const auto mapped_row = algorithm::apply(map, row, tag::facet{});
         const auto offset = std::find(matrix.cbegin(), matrix.cend(), mapped_row) - matrix.cbegin();
         indices[i] = static_cast<std::size_t>(offset);
         if ( indices[i] >= matrix.size() )
         {
            std::stringstream stream;
            stream << "Map invalid: " << map << " maps " << row << " to " << mapped_row << " which is not present in the set of inequalities.";
            throw std::invalid_argument(stream.str());
         }
      }
      std::sort(indices.begin(), indices.end());
      for ( std::size_t i = 0; i < indices.size(); ++i )
      {
         if ( indices[i] != i )
         {
            std::stringstream stream;
            stream << "Map invalid: " << map << " is not a bijection on the set of inequalities.";
            throw std::invalid_argument(stream.str());
         }
      }
   }

   void checkValidityVertices(const Matrix<int>& matrix, const Map& map)
   {
      std::vector<std::size_t> indices(matrix.size());
      for ( std::size_t i = 0; i < matrix.size(); ++i )
      {
         const auto& row = matrix[i];
         const auto mapped_row = algorithm::apply(map, row, tag::vertex{});
         const auto offset = std::find(matrix.cbegin(), matrix.cend(), mapped_row) - matrix.cbegin();
         indices[i] = static_cast<std::size_t>(offset);
         if ( indices[i] >= matrix.size() )
         {
            std::stringstream stream;
            stream << "Map invalid: " << map << " maps " << row << " to " << mapped_row << " which is not present in the set of vertices / rays.";
            throw std::invalid_argument(stream.str());
         }
      }
      std::sort(indices.begin(), indices.end());
      for ( std::size_t i = 0; i < indices.size(); ++i )
      {
         if ( indices[i] != i )
         {
            std::stringstream stream;
            stream << "Map invalid: " << map << " is not a bijection on the set of vertices / rays.";
            throw std::invalid_argument(stream.str());
         }
      }
   }
}

