
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <tuple>
#include <vector>

#include "matrix.h"
#include "row.h"

namespace panda
{
   namespace algorithm
   {
      /// Full Fourier-Motzkin elimination: For vertices/rays as input (homogenized),
      /// it returns the set of facets, for a set of inequalities, the set of
      /// extremal vertices/rays is returned.
      template <typename Integer>
      Matrix<Integer> fourierMotzkinElimination(Matrix<Integer>);
      /// Heuristic using Fourier-Motzkin elimination to identify some facets.
      /// Output is guaranteed to contain only facets, but it is highly likely
      /// that it is not the complete set of facets.
      template <typename Integer>
      Matrix<Integer> fourierMotzkinEliminationHeuristic(Matrix<Integer>);
   }
}

#include "algorithm_fourier_motzkin_elimination.eti"

