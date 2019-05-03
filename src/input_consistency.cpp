
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_consistency.h"

#include <limits>
#include <stdexcept>
#include <string>

using namespace panda;

namespace
{
   /// Checks if any element of a range has the correct size.
   template <typename Range>
   void rangeDimensionCheck(const Range&, const std::size_t, const std::string&);
}

/// A convex hull is considered valid if non-empty and each vertex has the same dimension.
void panda::input::implementation::checkConsistency(const ConvexHull<int>& conv)
{
   if ( conv.empty() )
   {
      throw std::invalid_argument("Invalid input data: Convex hull is empty.");
   }
   const auto dim = conv.front().size();
   rangeDimensionCheck(conv, dim, "Invalid input data: Inconsistent dimensions.");
}

/// A mixed description (cone + conv) is considered valid if the dimension matches and |cone| + |conv| > 0.
void panda::input::implementation::checkConsistency(const ConvexHull<int>& conv, const ConicalHull<int>& cone, const Names& names, const Maps& maps, const std::size_t dimension)
{
   checkConsistency(conv, cone, {}, names, maps, dimension);
}

void panda::input::implementation::checkConsistency(const Inequalities<int>& inequalities, const Names& names, const std::size_t dimension)
{
   if ( inequalities.empty() )
   {
      throw std::invalid_argument("Invalid input data: No inequalities.");
   }
   const auto dim = inequalities.front().size();
   rangeDimensionCheck(inequalities, dim, "Invalid input data: Inconsistent dimensions (an inequality is invalid).");
   if ( !names.empty() && names.size() + 1 != dim )
   {
      throw std::invalid_argument("Invalid input data: The number of names does not match the dimension.");
   }
   if ( dimension != std::numeric_limits<std::size_t>::max() && dim != dimension + 1 )
   {
      throw std::invalid_argument("Invalid input data: Inconsistent dimensions (user-specified dimension does not match the length of rows. Note that this field is optional).");
   }
}

void panda::input::implementation::checkConsistency(const ConvexHull<int>& conv, const ConicalHull<int>& cone, const Inequalities<int>& inequalities, const Names& names, const Maps& maps, const std::size_t dimension)
{
   if ( conv.empty() && cone.empty() )
   {
      throw std::invalid_argument("Invalid input data: Convex hull and conical hull are empty.");
   }
   const auto dim = (!conv.empty()) ? conv.front().size() : cone.front().size();
   rangeDimensionCheck(conv, dim, "Invalid input data: Inconsistent dimensions (a vertex in the convex hull is invalid).");
   rangeDimensionCheck(cone, dim, "Invalid input data: Inconsistent dimensions (a ray in the conical hull is invalid).");
   rangeDimensionCheck(inequalities, dim, "Invalid input data: Inconsistent dimensions (an inequality is invalid).");
   rangeDimensionCheck(maps, dim, "Invalid input data: Inconsistent dimensions (a map is invalid).");
   if ( !names.empty() && names.size() + 1 != dim )
   {
      throw std::invalid_argument("Invalid input data: The number of names does not match the dimension.");
   }
   if ( dimension != std::numeric_limits<std::size_t>::max() && dim != dimension + 1 )
   {
      throw std::invalid_argument("Invalid input data: Inconsistent dimensions (user-specified dimension does not match the length of rows. Note that this field is optional).");
   }
}

namespace
{
   template <typename Range>
   void rangeDimensionCheck(const Range& range, const std::size_t dimension, const std::string& message)
   {
      for ( const auto& entry : range )
      {
         if ( entry.size() != dimension )
         {
            throw std::invalid_argument(message);
         }
      }
   }
}

