
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <iosfwd>
#include <type_traits>

#include "matrix.h"
#include "names.h"
#include "row.h"

/// Unnamed output of a row.
template <typename Integer>
std::ostream& operator<<(std::ostream&, const panda::Row<Integer>&);
template <>
std::ostream& operator<<(std::ostream&, const panda::Row<int>&);

/// Addition of a row onto another.
template <typename Integer>
panda::Row<Integer>& operator+=(panda::Row<Integer>&, const panda::Row<Integer>&) noexcept;
/// Addition of two rows.
template <typename Integer>
panda::Row<Integer> operator+(panda::Row<Integer>, const panda::Row<Integer>&);
/// Scaling of a row.
template <typename Integer>
panda::Row<Integer>& operator*=(panda::Row<Integer>&, const Integer) noexcept;
/// Returns a scaled row.
template <typename Integer>
panda::Row<Integer> operator*(panda::Row<Integer>, const Integer);
/// Returns a scaled row.
template <typename Integer>
panda::Row<Integer> operator*(const Integer, panda::Row<Integer>);
/// Subtraction of a row from another.
template <typename Integer>
panda::Row<Integer>& operator-=(panda::Row<Integer>&, const panda::Row<Integer>&) noexcept;
/// Subtraction of two rows.
template <typename Integer>
panda::Row<Integer> operator-(panda::Row<Integer>, const panda::Row<Integer>&);
/// Scaling of a row (division).
template <typename Integer>
panda::Row<Integer>& operator/=(panda::Row<Integer>&, const Integer);
/// Returns a scaled row (division).
template <typename Integer>
panda::Row<Integer> operator/(panda::Row<Integer>, const Integer);
/// Returns the scalar product of two rows.
template <typename Integer>
Integer operator*(const panda::Row<Integer>&, const panda::Row<Integer>&) noexcept;

namespace panda
{
   namespace algorithm
   {
      /// Normalizes a row according to a set of equations.
      template <typename Integer>
      Row<Integer> normalize(Row<Integer>, const Equations<Integer>&);
      /// Calculates the greatest common divisor of all entries of a row.
      template <typename Integer>
      Integer gcd(const Row<Integer>&) noexcept;
      /// Calculates the least common multiple of all entries of a row.
      template <typename Integer>
      Integer lcm(const Row<Integer>&) noexcept;
      /// Named output of a row (last entry is delimited by a sequence of characters (last argument)).
      template <typename Integer>
      void prettyPrint(std::ostream&, const Row<Integer>&, const Names&, const char*);
      /// Named output of a row with trailing newline.
      template <typename Integer>
      void prettyPrintln(std::ostream&, const Row<Integer>&, const Names&, const char*);
      /// Unnamed output of a vertex that may have fractional values.
      template <typename Integer>
      void printFractional(std::ostream&, const Vertex<Integer>&);

      // explicit instantiations for type int:
      template <>
      void prettyPrint(std::ostream&, const Row<int>&, const Names&, const char*);
      template <>
      void prettyPrintln(std::ostream&, const Row<int>&, const Names&, const char*);
      template <>
      void printFractional(std::ostream&, const Vertex<int>&);
   }
}

#include "algorithm_row_operations.eti"

