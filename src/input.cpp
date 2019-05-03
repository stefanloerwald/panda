
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

#include "algorithm_classes.h"
#include "algorithm_map_operations.h"
#include "algorithm_matrix_operations.h"
#include "algorithm_row_operations.h"
#include "input_common.h"
#include "input_consistency.h"
#include "input_constraint.h"
#include "input_detection.h"
#include "input_dimension.h"
#include "input_keywords.h"
#include "input_map.h"
#include "input_names.h"
#include "input_order.h"
#include "input_validity.h"
#include "input_vertex.h"
#include "istream_peek_line.h"

using namespace panda;

namespace
{
   void sort(int, char**, Matrix<int>&);
   void expandInequalities(Matrix<int>&, const Maps&);
   void expandVertices(Matrix<int>&, const Maps&);

   std::string getFilenameKnownFacets(int, char**);
   std::string getFilenameKnownVertices(int, char**);
   Inequalities<int> knownFacets(int, char**, const Names&);
   Vertices<int> knownVertices(int, char**);
}

namespace
{
   Vertices<int> readVerticesConvex(int argc, char** argv, std::ifstream& file, const Maps& maps)
   {
      const auto conv = input::implementation::verticesConvex(file);
      if ( input::checkValidity(argc, argv) )
      {
         input::implementation::checkValidityOfVertexClasses(conv, maps);
      }
      return conv;
   }
   Vertices<int> readVerticesReducedConvex(int argc, char** argv, std::ifstream& file, const Maps& maps)
   {
      auto conv = input::implementation::verticesConvex(file);
      if ( input::checkValidity(argc, argv) )
      {
         std::cerr << "Warning: validity of input vertices cannot be verified for reduced input.\n";
      }
      expandVertices(conv, maps);
      return conv;
   }
   Vertices<int> readVerticesConical(int argc, char** argv, std::ifstream& file, const Maps& maps)
   {
      const auto cone = input::implementation::verticesConical(file);
      if ( input::checkValidity(argc, argv) )
      {
         input::implementation::checkValidityOfVertexClasses(cone, maps);
      }
      return cone;
   }
   Vertices<int> readVerticesReducedConical(int argc, char** argv, std::ifstream& file, const Maps& maps)
   {
      auto cone = input::implementation::verticesConical(file);
      if ( input::checkValidity(argc, argv) )
      {
         std::cerr << "Warning: validity of input rays cannot be verified for reduced input.\n";
      }
      expandVertices(cone, maps);
      return cone;
   }
   Inequalities<int> readInequalities(int argc, char** argv, std::ifstream& file, const Names& names, const Maps& maps)
   {
      const auto inequalities = input::implementation::constraints<ConstraintType::Inequality>(file, names);
      if ( input::checkValidity(argc, argv) )
      {
         input::implementation::checkValidityOfInequalityClasses(inequalities, maps);
      }
      return inequalities;
   }
   Inequalities<int> readReducedInequalities(int argc, char** argv, std::ifstream& file, const Names& names, const Maps& maps, const Equations<int>& equations)
   {
      auto inequalities = input::implementation::constraints<ConstraintType::Inequality>(file, names);
      if ( input::checkValidity(argc, argv) )
      {
         std::cerr << "Warning: validity of input inequalities cannot be checked for reduced input.\n";
      }
      if ( equations.empty() )
      {
         std::cerr << "Warning: expanding the reduced input will be invalid, if the polytope is not full dimension (i.e. there are equations).\n";
      }
      const auto tmp_maps = algorithm::normalize(maps, equations);
      expandInequalities(inequalities, tmp_maps);
      return inequalities;
   }
}

template <>
std::tuple<Vertices<int>, Names, Maps, Inequalities<int>> panda::input::vertices<int>(int argc, char** argv)
{
   const auto filename = getFilename(argc, argv);
   std::ifstream file(filename.c_str());
   if ( !file )
   {
      throw std::invalid_argument("Failed to open file \"" + filename + "\".");
   }
   ConvexHull<int> conv;
   ConvexHull<int> cone;
   std::size_t dimension = std::numeric_limits<std::size_t>::max();
   Names names;
   Maps maps;
   if ( !implementation::containsKeywords(file) )
   {
      throw std::invalid_argument("An inner description must be given in PANDA format.");
   }
   for ( std::string token; file && advanceToNextKeyword(file, token); )
   {
      if ( implementation::isKeywordDimension(token) )
      {
         dimension = implementation::dimension(file);
      }
      else if ( implementation::isKeywordNames(token) )
      {
         names = implementation::names(file);
      }
      else if ( implementation::isKeywordMaps(token) )
      {
         maps = implementation::maps(file, names);
      }
      else if ( implementation::isKeywordConvexHull(token) )
      {
         conv = readVerticesConvex(argc, argv, file, maps);
      }
      else if ( implementation::isKeywordReducedConvexHull(token) )
      {
         conv = readVerticesReducedConvex(argc, argv, file, maps);
      }
      else if ( implementation::isKeywordConicalHull(token) )
      {
         cone = readVerticesConical(argc, argv, file, maps);
      }
      else if ( implementation::isKeywordReducedConicalHull(token) )
      {
         cone = readVerticesReducedConical(argc, argv, file, maps);
      }
      else
      {
         std::getline(file, token);
      }
   }
   implementation::checkConsistency(conv, cone, names, maps, dimension);
   sort(argc, argv, cone);
   sort(argc, argv, conv);
   Matrix<int> vertices;
   vertices.reserve(cone.size() + conv.size());
   vertices.insert(vertices.end(), cone.cbegin(), cone.cend());
   vertices.insert(vertices.end(), conv.cbegin(), conv.cend());
   auto known_facets = knownFacets(argc, argv, names);
   if ( input::checkValidity(argc, argv) )
   {
      input::implementation::checkValidityOfInequalities(vertices, known_facets);
   }
   return std::make_tuple(vertices, names, maps, known_facets);
}

template <>
std::tuple<Inequalities<int>, Names, Maps, Vertices<int>> panda::input::inequalities<int>(int argc, char** argv)
{
   const auto filename = getFilename(argc, argv);
   std::ifstream file(filename.c_str());
   if ( !file )
   {
      throw std::invalid_argument("Failed to open file \"" + filename + "\".");
   }
   Equations<int> equations;
   Inequalities<int> inequalities;
   std::size_t dimension = std::numeric_limits<std::size_t>::max();
   Names names;
   Maps maps;
   if ( !implementation::containsKeywords(file) )
   {
      throw std::invalid_argument("An outer description must be given in PANDA format.");
   }
   for ( std::string token; file && advanceToNextKeyword(file, token); )
   {
      if ( implementation::isKeywordDimension(token) )
      {
         dimension = implementation::dimension(file);
      }
      else if ( implementation::isKeywordNames(token) )
      {
         names = implementation::names(file);
      }
      else if ( implementation::isKeywordMaps(token) )
      {
         maps = implementation::maps(file, names);
      }
      else if ( implementation::isKeywordEquations(token) )
      {
         equations = implementation::constraints<ConstraintType::Equation>(file, names);
      }
      else if ( implementation::isKeywordInequalities(token) )
      {
         inequalities = readInequalities(argc, argv, file, names, maps);
      }
      else if ( implementation::isKeywordReducedInequalities(token) )
      {
         inequalities = readReducedInequalities(argc, argv, file, names, maps, equations);
      }
      else
      {
         std::getline(file, token);
      }
   }
   implementation::checkConsistency(inequalities, names, dimension);
   inequalities.reserve(inequalities.size() + 2 * equations.size());
   for ( const auto& equation : equations )
   {
      inequalities.push_back(equation);
      inequalities.push_back(-1 * equation);
   }
   sort(argc, argv, inequalities);
   inequalities.emplace_back(inequalities.back().size(), 0);
   inequalities.back().back() = -1;
   auto known_vertices = knownVertices(argc, argv);
   if ( input::checkValidity(argc, argv) )
   {
      input::implementation::checkValidityOfVertices(inequalities, known_vertices);
   }
   return std::make_tuple(inequalities, names, maps, known_vertices);
}

namespace
{
   std::string getFilenameKnownFacets(int argc, char** argv)
   {
      for ( int i = 1; i < argc; ++i )
      {
         if ( std::strcmp(argv[i], "-k") == 0 )
         {
            if ( i + 1 == argc )
            {
               throw std::invalid_argument("Expected argument to option \"--known-facets\".");
            }
            return argv[i + 1];
         }
         else if ( std::strncmp(argv[i], "-k=", 3) == 0 )
         {
            return argv[i] + 3;
         }
         else if ( std::strncmp(argv[i], "--known-data=", 13) == 0 )
         {
            return argv[i] + 13;
         }
         else if ( std::strncmp(argv[i], "--known-facets=", 15) == 0 )
         {
            return argv[i] + 15;
         }
         else if ( std::strncmp(argv[i], "--known-vertices=", 17) == 0 )
         {
            throw std::invalid_argument("Unexpected argument \"--known-vertices\". Did you mean \"--known-data\"?");
         }
         else if ( std::strncmp(argv[i], "-k", 2) == 0 || std::strncmp(argv[i], "--k", 3) == 0 )
         {
            throw std::invalid_argument("Illegal parameter. Did you mean \"-k <file>\" or \"--known-data=<file>\"?");
         }
      }
      return "";
   }

   std::string getFilenameKnownVertices(int argc, char** argv)
   {
      for ( int i = 1; i < argc; ++i )
      {
         if ( std::strcmp(argv[i], "-k") == 0 )
         {
            if ( i + 1 == argc )
            {
               throw std::invalid_argument("Expected argument to option \"--known-vertices\".");
            }
            return argv[i + 1];
         }
         else if ( std::strncmp(argv[i], "-k=", 3) == 0 )
         {
            return argv[i] + 3;
         }
         else if ( std::strncmp(argv[i], "--known-data=", 13) == 0 )
         {
            return argv[i] + 13;
         }
         else if ( std::strncmp(argv[i], "--known-facets=", 15) == 0 )
         {
            throw std::invalid_argument("Unexpected argument \"--known-facets\". Did you mean \"--known-data\"?");
         }
         else if ( std::strncmp(argv[i], "--known-vertices=", 17) == 0 )
         {
            return argv[i] + 17;
         }
         else if ( std::strncmp(argv[i], "-k", 2) == 0 || std::strncmp(argv[i], "--k", 3) == 0 )
         {
            throw std::invalid_argument("Illegal parameter. Did you mean \"-k <file>\" or \"--known-data=<file>\"?");
         }
      }
      return "";
   }

   Inequalities<int> knownFacets(int argc, char** argv, const Names& names)
   {
      const auto filename = getFilenameKnownFacets(argc, argv);
      if ( filename.empty() )
      {
         return {};
      }
      std::ifstream file(filename.c_str());
      if ( !file )
      {
         throw std::invalid_argument("Failed to open file \"" + filename + "\".");
      }
      std::string token;
      input::advanceToNextKeyword(file, token);
      if ( input::implementation::isKeywordInequalities(token) )
      {
         return readInequalities(argc, argv, file, names, {});
      }
      throw std::invalid_argument("Expected no other keyword than \"Inequalities\" in a file containing known facets.");
   }

   Vertices<int> knownVertices(int argc, char** argv)
   {
      const auto filename = getFilenameKnownVertices(argc, argv);
      if ( filename.empty() )
      {
         return {};
      }
      std::ifstream file(filename.c_str());
      if ( !file )
      {
         throw std::invalid_argument("Failed to open file \"" + filename + "\".");
      }
      Vertices<int> conv;
      Vertices<int> cone;
      for ( std::string token; input::advanceToNextKeyword(file, token); )
      {
         if ( input::implementation::isKeywordConvexHull(token) )
         {
            conv = readVerticesConvex(argc, argv, file, {});
         }
         else if ( input::implementation::isKeywordConicalHull(token) )
         {
            cone = readVerticesConical(argc, argv, file, {});
         }
         else
         {
            std::getline(file, token);
         }
      }
      conv.reserve(conv.size() + cone.size());
      conv.insert(conv.end(), cone.cbegin(), cone.cend());
      return conv;
   }
}

namespace
{
   void sort(int argc, char** argv, Matrix<int>& matrix)
   {
      const auto order = getInputOrder(argc, argv);
      switch ( order )
      {
         case InputOrder::LexicographicAscending:
         {
            std::sort(matrix.begin(), matrix.end());
            return;
         }
         case InputOrder::LexicographicDescending:
         {
            std::sort(matrix.rbegin(), matrix.rend());
            return;
         }
         case InputOrder::NonZeroEntriesAscending:
         {
            std::sort(matrix.begin(), matrix.end(), [](const Row<int>& a, const Row<int>& b)
            {
               const auto zero_entries_a = std::count(a.cbegin(), a.cend(), 0);
               const auto zero_entries_b = std::count(b.cbegin(), b.cend(), 0);
               if ( zero_entries_a != zero_entries_b )
               {
                  return (zero_entries_a > zero_entries_b);
               }
               return (a < b);
            });
            return;
         }
         case InputOrder::NonZeroEntriesDescending:
         {
            std::sort(matrix.begin(), matrix.end(), [](const Row<int>& a, const Row<int>& b)
            {
               const auto zero_entries_a = std::count(a.cbegin(), a.cend(), 0);
               const auto zero_entries_b = std::count(b.cbegin(), b.cend(), 0);
               if ( zero_entries_a != zero_entries_b )
               {
                  return (zero_entries_a < zero_entries_b);
               }
               return (a < b);
            });
            return;
         }
         case InputOrder::Reverse:
         {
            std::reverse(matrix.begin(), matrix.end());
            return;
         }
         case InputOrder::NoSorting:
         {
            return;
         }
      }
   }

   void expandInequalities(Matrix<int>& matrix, const Maps& maps)
   {
      if ( maps.empty() )
      {
         throw std::invalid_argument("The system of inequalities cannot be a reduced system without any maps. Maps must be declared before the section of inequalities.");
      }
      Matrix<int> all;
      for ( const auto& row : matrix )
      {
         const auto row_class = algorithm::getClass(row, maps, tag::facet{});
         all.reserve(all.size() + row_class.size());
         all.insert(all.end(), row_class.cbegin(), row_class.cend());
      }
      matrix = std::move(all);
   }

   void expandVertices(Matrix<int>& matrix, const Maps& maps)
   {
      if ( maps.empty() )
      {
         throw std::invalid_argument("The system of vertices / rays cannot be a reduced system without any maps. Maps must be declared before the section of vertices / rays.");
      }
      Matrix<int> all;
      for ( const auto& row : matrix )
      {
         const auto row_class = algorithm::getClass(row, maps, tag::vertex{});
         all.reserve(all.size() + row_class.size());
         all.insert(all.end(), row_class.cbegin(), row_class.cend());
      }
      matrix = std::move(all);
   }
}

