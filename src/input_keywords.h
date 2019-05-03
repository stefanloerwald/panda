
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <string>

#include "position_guarded_file.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Returns true if the file contains any of the PANDA keywords.
         bool containsKeywords(const PositionGuardedFile&);
         /// Returns true if the string is a keyword.
         bool isKeyword(const std::string&) noexcept;
         /// Returns true if the string is a keyword for the end of a section.
         bool isKeywordEnd(const std::string&) noexcept;
         /// Returns true if the string is a keyword for a conical hull.
         bool isKeywordConicalHull(const std::string&) noexcept;
         /// Returns true if the string is a keyword beginning with "reduced".
         bool isKeywordReducedSomething(const std::string&) noexcept;
         /// Returns true if the string is a keyword for a reduced conical hull.
         bool isKeywordReducedConicalHull(const std::string&) noexcept;
         /// Returns true if the string is a keyword for a convex hull.
         bool isKeywordConvexHull(const std::string&) noexcept;
         /// Returns true if the string is a keyword for a reduced convex hull.
         bool isKeywordReducedConvexHull(const std::string&) noexcept;
         /// Returns true if the string is a keyword for dimension.
         bool isKeywordDimension(const std::string&) noexcept;
         /// Returns true if the string is a keyword for equations.
         bool isKeywordEquations(const std::string&) noexcept;
         /// Returns true if the string is a keyword for inequalities.
         bool isKeywordInequalities(const std::string&) noexcept;
         /// Returns true if the string is a keyword for reduced inequalities.
         bool isKeywordReducedInequalities(const std::string&) noexcept;
         /// Returns true if the string is a keyword for maps.
         bool isKeywordMaps(const std::string&) noexcept;
         /// Returns true if the string is a keyword for names.
         bool isKeywordNames(const std::string&) noexcept;
      }
   }
}

