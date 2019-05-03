
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <iosfwd>
#include <vector>

#include "matrix.h"
#include "names.h"
#include "row.h"

/// Outputs a matrix of integers.
template <typename Integer>
std::ostream& operator<<(std::ostream&, const panda::Matrix<Integer>&);
/// Matrix-vector product.
template <typename Integer>
panda::Row<Integer> operator*(const panda::Matrix<Integer>&, const panda::Row<Integer>&);

namespace panda
{
   using Indices = std::vector<std::size_t>;

   namespace algorithm
   {
      /// Outputs a matrix. Columns are named, the last column ist delimited by
      /// the others by a sequence of characters (last argument).
      template <typename Integer>
      void prettyPrint(std::ostream&, const Matrix<Integer>&, const Names&, const char*);
      /// returns the transposition of the input matrix.
      template <typename Integer>
      Matrix<Integer> transpose(const Matrix<Integer>&);
      /// Returns the dimension of the matrix.
      template <typename Integer>
      std::size_t dimension(Matrix<Integer>);
      /// Performs gaussian elimination on the input matrix. Returns sets of indices
      /// referring to the rows and columns denoting the pivot elements
      /// (relevant for Fourier-Motzkin elimination).
      template <typename Integer>
      std::pair<Indices, Indices> gaussianElimination(Matrix<Integer>&);
      /// Used for gaussian elimination: First step of FME is to calculate an inverse.
      template <typename Integer>
      void appendNegativeIdentityMatrix(Matrix<Integer>&);
      /// Extracts the equations from a set of vertices.
      template <typename Integer>
      Equations<Integer> extractEquations(Matrix<Integer>);
      /// Extracts equations marked by indices in a matrix.
      template <typename Integer>
      Equations<Integer> extractEquations(Matrix<Integer>&, const Indices&);
   }
}

#include "algorithm_matrix_operations.eti"

