
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   enum class OperationMode
   {
      FacetEnumeration,  /// Facet enumeration: input consists of vertices and rays.
      VertexEnumeration, /// Vertex enumeration: input consists of equations and inequalities.
      HelpCommand,       /// Show specific help information for an option.
      Help,              /// Show help information.
      Version,           /// Show version information.
      Invalid,           /// If there are keywords, but not any keyword which can be used to deduce mode.
      Undecided          /// Unknown state.
   };
}

