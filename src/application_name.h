
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   namespace project
   {
      constexpr static auto application_acronym = "PANDA"; /// The short name of the application (acronym).
      static_assert(sizeof(application_acronym) > 0, "Dummy assertion.");
      constexpr static auto binary_name = "panda"; /// The name of the binary.
      static_assert(sizeof(binary_name) > 0, "Dummy assertion.");
      constexpr static auto application_name = "Parallel AdjaceNcy Decomposition Algorithm"; /// What the acronym stands for.
      static_assert(sizeof(application_name) > 0, "Dummy assertion.");
   }
}

