
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_MAP_OPERATIONS
#include "algorithm_map_operations.h"
#undef COMPILE_TEMPLATE_ALGORITHM_MAP_OPERATIONS

#include <algorithm>
#include <cassert>
#include <iostream>

#include "algorithm_row_operations.h"

using namespace panda;

namespace
{
   /// Normalize a single map using equations.
   template <typename Integer>
   Map normalizeMap(Map, const Equations<Integer>&);
   /// Output a single term in an image of a map.
   std::ostream& operator<<(std::ostream&, const Term&);
   /// Output a single image in a map.
   std::ostream& operator<<(std::ostream&, const Image&);
}

template <typename Integer>
Maps panda::algorithm::normalize(Maps maps, const Equations<Integer>& equations)
{
   for ( auto& map : maps )
   {
      map = normalizeMap(map, equations);
   }
   return maps;
}

namespace
{
   template <typename Integer>
   void applyTerm(const Row<Integer>& input, Row<Integer>& result, const std::size_t index, const Term& term, tag::facet)
   {
      assert( index < input.size() );
      assert( term.first < result.size() );
      result[term.first] += static_cast<Integer>(term.second) * input[index];
   }

   template <typename Integer>
   void applyTerm(const Row<Integer>& input, Row<Integer>& result, const std::size_t index, const Term& term, tag::vertex)
   {
      assert( index < input.size() );
      assert( term.first < result.size() );
      result[index] += static_cast<Integer>(term.second) * input[term.first];
   }
}

template <typename Integer, typename TagType>
Row<Integer> algorithm::apply(const Map& map, const Row<Integer>& row, TagType tag)
{
   assert( row.size() == map.size() );
   Row<Integer> result(row.size(), Integer(0));
   for ( std::size_t i = 0; i < map.size(); ++i )
   {
      const auto& entry = map[i];
      for ( const auto& term : entry )
      {
         applyTerm(row, result, i, term, tag);
      }
   }
   const auto gcd_value = gcd(result);
   if ( gcd_value > 1 )
   {
      result /= gcd_value;
   }
   return result;
}

std::ostream& operator<<(std::ostream& stream, const panda::Map& map)
{
   stream << '[';
   for ( const auto& image : map )
   {
      stream << image;
   }
   stream << ']';
   return stream;
}

namespace
{
   using Index = std::size_t;
   using Indices = std::vector<Index>;

   /// Identify the indices that are going to be eliminated by the equations.
   template <typename Integer>
   Indices indices(const Equations<Integer>& equations)
   {
      Indices indices;
      indices.reserve(equations.size());
      for ( const auto& eq : equations )
      {
         const auto non_zero_it = std::find_if(eq.cbegin(), eq.cend(), [](const Integer& a) { return a != 0; });
         const auto index = static_cast<Index>(non_zero_it - eq.cbegin());
         indices.push_back(index);
      }
      return indices;
   }

   /// Applying the map to an inequality has precondition that the input is normalized.
   /// Hence, the coefficients matching one of the indices eliminated by the equations are zero.
   void eliminateZeroCoefficients(Map& map, const Indices& indices)
   {
      for ( const auto index : indices )
      {
         map[index].clear();
      }
   }

   /// Use equation to get eliminated index back to zero.
   template <typename Integer>
   void subtractEquation(Map& map, const Index& index, const Equation<Integer>& equation)
   {
      assert( equation[index] != 0 );
      for ( auto& image : map )
      {
         Image new_image;
         for ( const auto& term : image )
         {
            if ( term.first == index )
            {
               for ( std::size_t j = index + 1; j < equation.size(); ++j )
               {
                  if ( equation[j] != 0 )
                  {
                     new_image.push_back(std::make_pair(j, -term.second * equation[j]));
                  }
               }
            }
            else
            {
               new_image.push_back(term);
            }
            image = new_image;
         }
      }
   }

   /// Use equations to get eliminated indices back to zero.
   template <typename Integer>
   void subtractEquations(Map& map, const Indices& indices, const Equations<Integer>& equations)
   {
      assert( indices.size() == equations.size() );
      for ( std::size_t i = 0; i < indices.size(); ++i )
      {
         const auto& index = indices[i];
         const auto& equation = equations[i];
         subtractEquation(map, index, equation);
      }
   }

   /// Simplification of a map: If an image is something like [1x1 + 2x2 -1x1 +1x2],
   /// it can be simplified to [3x2].
   void simplify(Map& map)
   {
      for ( auto& image : map )
      {
         std::sort(image.begin(), image.end());
         for ( std::size_t i = 0; i + 1 < image.size(); ++i )
         {
            if ( image[i].first == image[i + 1].first ) // same Index
            {
               image[i].second += image[i + 1].second; // merge factors
               image.erase(image.begin() + static_cast<Image::difference_type>(i) + 1);     // erase unused term
               if ( image[i].second == 0 )
               {
                  image.erase(image.begin() + static_cast<Image::difference_type>(i)); // erase unused item
                  --i; // decrement i because the current i needs to be revisited
               }
               --i; // decrement i because we erased the next item
            }
         }
      }
   }

   /// A normalized map can ignore the eliminated indices, as the coefficients are zero
   /// in a normalized inequality.
   template <typename Integer>
   Map normalizeMap(Map map, const Equations<Integer>& equations)
   {
      const auto indices = ::indices(equations);
      eliminateZeroCoefficients(map, indices);
      subtractEquations(map, indices, equations);
      simplify(map);
      return map;
   }

   std::ostream& operator<<(std::ostream& stream, const panda::Term& term)
   {
      stream << term.second << "*VAR_" << term.first;
      return stream;
   }

   std::ostream& operator<<(std::ostream& stream, const panda::Image& image)
   {
      stream << '[';
      bool first = true;
      for ( const auto& term : image )
      {
         stream << term;
         if ( !first )
         {
            stream << ',';
         }
         first = false;
      }
      stream << ']';
      return stream;
   }
}

