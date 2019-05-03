
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include "maps.h"
#include "matrix.h"
#include "names.h"

namespace panda
{
   namespace input
   {
      namespace implementation
      {
         /// Checks the validity of a set of maps on an inequality description.
         void checkValidityOfInequalityClasses(const Inequalities<int>&, const Maps&);
         /// Checks the validity of a set of maps on an inner description.
         void checkValidityOfVertexClasses(const Vertices<int>&, const Maps&);
         /// Checks the validity of a set of inequalities.
         void checkValidityOfInequalities(const Matrix<int>&, const Inequalities<int>&);
         /// Checks the validity of a set of vertices.
         void checkValidityOfVertices(const Matrix<int>&, const Vertices<int>&);
         /// Filters inequalities that are invalid.
         void filterInvalidInequalities(const Matrix<int>&, Inequalities<int>&);
         /// Filters vertices that are invalid.
         void filterInvalidVertices(const Matrix<int>&, Vertices<int>&);
      }

      /// Returns true if the user provided the --check parameter.
      bool checkValidity(int, char**) noexcept;
      /// Returns true if the user provided the --filter parameter.
      bool filterInvalidInput(int, char**) noexcept;
   }
}

