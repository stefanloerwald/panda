
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_keywords.h"

#include "input_common.h"
#include "position_guarded_file.h"

using namespace panda;

bool panda::input::implementation::containsKeywords(const PositionGuardedFile& pg)
{
   // We are not going to read the whole file, because it is potentially huge.
   // Instead, we know that in no sane file, the first 20 lines are free of keywords.
   // This is because the lines would have to be empty.
   const int max_tokens = 20;
   int i = 0;
   for ( std::string token; i < max_tokens && std::getline(pg.file, token); ++i )
   {
      token = trimWhitespace(token);
      if ( isKeyword(token) )
      {
         return true;
      }
   }
   return false;
}


bool panda::input::implementation::isKeyword(const std::string& string) noexcept
{
   return (isKeywordConicalHull(string) ||
           isKeywordReducedSomething(string) ||
           isKeywordReducedConicalHull(string) ||
           isKeywordConvexHull(string) ||
           isKeywordReducedConvexHull(string) ||
           isKeywordDimension(string) ||
           isKeywordEquations(string) ||
           isKeywordInequalities(string) ||
           isKeywordReducedInequalities(string) ||
           isKeywordMaps(string) ||
           isKeywordNames(string) ||
           isKeywordEnd(string) );
}

bool panda::input::implementation::isKeywordEnd(const std::string& string) noexcept
{
   return (string == "END" ||
           string == "End");
}

bool panda::input::implementation::isKeywordConicalHull(const std::string& string) noexcept
{
   return (string == "CONE_SECTION" ||
           string == "CONE_SECTION:" ||
           string == "CONICAL_HULL" ||
           string == "CONICAL_HULL:" ||
           string == "CONICAL_HULL_SECTION" ||
           string == "CONICAL_HULL_SECTION:" ||
           string == "Rays" ||
           string == "Rays:");
}

bool panda::input::implementation::isKeywordReducedSomething(const std::string& string) noexcept
{
   return (string == "Reduced");
}

bool panda::input::implementation::isKeywordReducedConicalHull(const std::string& string) noexcept
{
   return (string == "REDUCED_CONE_SECTION" ||
           string == "REDUCED_CONE_SECTION:" ||
           string == "REDUCED_CONICAL_HULL" ||
           string == "REDUCED_CONICAL_HULL:" ||
           string == "REDUCED_CONICAL_HULL_SECTION" ||
           string == "REDUCED_CONICAL_HULL_SECTION:" ||
           string == "Reduced Rays" ||
           string == "Reduced Rays:");
}

bool panda::input::implementation::isKeywordConvexHull(const std::string& string) noexcept
{
   return (string == "CONVEX_HULL" ||
           string == "CONVEX_HULL:" ||
           string == "CONVEX_HULL_SECTION" ||
           string == "CONVEX_HULL_SECTION:" ||
           string == "CONV_SECTION" ||
           string == "CONV_SECTION:" ||
           string == "Vertices" ||
           string == "Vertices:");
}

bool panda::input::implementation::isKeywordReducedConvexHull(const std::string& string) noexcept
{
   return (string == "REDUCED_CONVEX_HULL" ||
           string == "REDUCED_CONVEX_HULL:" ||
           string == "REDUCED_CONVEX_HULL_SECTION" ||
           string == "REDUCED_CONVEX_HULL_SECTION:" ||
           string == "REDUCED_CONV_SECTION" ||
           string == "REDUCED_CONV_SECTION:" ||
           string == "Reduced Vertices" ||
           string == "Reduced Vertices:");
}

bool panda::input::implementation::isKeywordDimension(const std::string& string) noexcept
{
   return (string == "DIM" ||
           string == "DIM=" ||
           string == "DIMENSION" ||
           string == "DIMENSION=" ||
           string == "Dim" ||
           string == "Dim=" ||
           string == "Dimension" ||
           string == "Dimension=");
}

bool panda::input::implementation::isKeywordEquations(const std::string& string) noexcept
{
   return (string == "EQUATIONS" ||
           string == "EQUATIONS:" ||
           string == "EQUATION_SECTION" ||
           string == "EQUATION_SECTION:" ||
           string == "EQUATIONS_SECTION" ||
           string == "EQUATIONS_SECTION:" ||
           string == "Equations" ||
           string == "Equations:");
}

bool panda::input::implementation::isKeywordInequalities(const std::string& string) noexcept
{
   return (string == "INEQUALITIES" ||
           string == "INEQUALITIES:" ||
           string == "INEQUALITY_SECTION" ||
           string == "INEQUALITY_SECTION:" ||
           string == "INEQUALITIES_SECTION" ||
           string == "INEQUALITIES_SECTION:" ||
           string == "Inequalities" ||
           string == "Inequalities:");
}

bool panda::input::implementation::isKeywordReducedInequalities(const std::string& string) noexcept
{
   return (string == "REDUCED_INEQUALITIES" ||
           string == "REDUCED_INEQUALITIES:" ||
           string == "REDUCED_INEQUALITY_SECTION" ||
           string == "REDUCED_INEQUALITY_SECTION:" ||
           string == "REDUCED_INEQUALITIES_SECTION" ||
           string == "REDUCED_INEQUALITIES_SECTION:" ||
           string == "Reduced Inequalities" ||
           string == "Reduced Inequalities:");
}

bool panda::input::implementation::isKeywordMaps(const std::string& string) noexcept
{
   return (string == "MAPS" ||
           string == "MAPS:" ||
           string == "MAP_SECTION" ||
           string == "MAP_SECTION:" ||
           string == "MAPS_SECTION" ||
           string == "MAPS_SECTION:" ||
           string == "Maps" ||
           string == "Maps:");
}

bool panda::input::implementation::isKeywordNames(const std::string& string) noexcept
{
   return (string == "INDEX" ||
           string == "INDEX:" ||
           string == "INDICES" ||
           string == "INDICES:" ||
           string == "INDEX_SECTION" ||
           string == "INDEX_SECTION:" ||
           string == "INDICES_SECTION" ||
           string == "INDICES_SECTION:" ||
           string == "NAMES" ||
           string == "NAMES:" ||
           string == "NAME_SECTION" ||
           string == "NAME_SECTION:" ||
           string == "NAMES_SECTION" ||
           string == "NAMES_SECTION:" ||
           string == "Indices" ||
           string == "Indices:" ||
           string == "Names" ||
           string == "Names:");
}

