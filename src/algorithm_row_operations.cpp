
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_ROW_OPERATIONS
#include "algorithm_row_operations.h"
#undef COMPILE_TEMPLATE_ALGORITHM_ROW_OPERATIONS

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include "algorithm_inequality_operations.h"
#include "algorithm_integer_operations.h"
#include "cast.h"

using namespace panda;

template <typename Integer>
std::ostream& operator<<(std::ostream& output, const Row<Integer>& row)
{
   return (output << cast<int>(row));
}

template <>
std::ostream& operator<<(std::ostream& output, const Row<int>& row)
{
   for ( const auto& e : row )
   {
      if ( e >= 0 )
      {
         output << ' ';
      }
      output << ' ' << e;
   }
   return output;
}

template <typename Integer>
void panda::algorithm::prettyPrint(std::ostream& output, const Row<Integer>& row, const Names& names, const char* relation)
{
   prettyPrint(output, cast<int>(row), names, relation);
}

template <>
void panda::algorithm::prettyPrint(std::ostream& output, const Row<int>& row, const Names& names, const char* relation)
{
   if ( names.empty() )
   {
      output << row;
      return;
   }
   assert( row.size() == names.size() + 1 );
   bool printed_something = false;
   for ( std::size_t i = 0; i + 1 < row.size(); ++i )
   {
      if ( row[i] != 0 )
      {
         if ( printed_something )
         {
            output << ' ';
         }
         if ( row[i] == 1 )
         {
            if ( printed_something )
            {
               output << '+';
            }
         }
         else if ( row[i] == -1 )
         {
            output << '-';
         }
         else
         {
            if ( row[i] > 1 && printed_something )
            {
               output << '+';
            }
            output << row[i];
         }
         output << names[i];
         printed_something = true;
      }
   }
   output << ' ' << relation << ' ' << algorithm::rhs(row);
}

template <typename Integer>
void panda::algorithm::prettyPrintln(std::ostream& output, const Row<Integer>& row, const Names& names, const char* relation)
{
   prettyPrintln(output, cast<int>(row), names, relation);
}

template <>
void panda::algorithm::prettyPrintln(std::ostream& output, const Row<int>& row, const Names& names, const char* relation)
{
   prettyPrint(output, row, names, relation);
   output << '\n';
}

template <typename Integer>
void panda::algorithm::printFractional(std::ostream& stream, const Vertex<Integer>& vertex)
{
   printFractional(stream, cast<int>(vertex));
}

template <>
void panda::algorithm::printFractional(std::ostream& stream, const Vertex<int>& vertex)
{
   assert( !vertex.empty() );
   assert( vertex.back() != 0 );
   const auto denominator_is_negative = (vertex.back() < 0);
   const auto denominator = ((denominator_is_negative) ? -1 : 1) * vertex.back();
   for ( std::size_t i = 0; i + 1 < vertex.size(); ++i )
   {
      const auto gcd_value = gcd(vertex[i], denominator);
      assert( gcd_value != 0 );
      stream << ((denominator_is_negative) ? -1 : 1) * vertex[i] / gcd_value;
      if ( denominator / gcd_value != 1 )
      {
         stream << '/' << denominator / gcd_value;
      }
      stream << ' ';
   }
}

template <typename Integer>
Row<Integer>& operator+=(Row<Integer>& first, const Row<Integer>& second) noexcept
{
   assert( first.size() == second.size() );
   std::transform(first.cbegin(), first.cend(), second.cbegin(), first.begin(), std::plus<Integer>());
   return first;
}

template <typename Integer>
Row<Integer> operator+(Row<Integer> first, const Row<Integer>& second)
{
   assert( first.size() == second.size() );
   first += second;
   return first;
}

template <typename Integer>
Row<Integer>& operator-=(Row<Integer>& first, const Row<Integer>& second) noexcept
{
   assert( first.size() == second.size() );
   std::transform(first.cbegin(), first.cend(), second.cbegin(), first.begin(), std::minus<Integer>());
   return first;
}

template <typename Integer>
Row<Integer> operator-(Row<Integer> first, const Row<Integer>& second)
{
   assert( first.size() == second.size() );
   first -= second;
   return first;
}

template <typename Integer>
Row<Integer>& operator*=(Row<Integer>& row, const Integer factor) noexcept
{
   if ( factor != 1 )
   {
      for ( auto& entry : row )
      {
         entry *= factor;
      }
   }
   return row;
}

template <typename Integer>
Row<Integer> operator*(Row<Integer> row, const Integer factor)
{
   row *= factor;
   return row;
}

template <typename Integer>
Row<Integer> operator*(const Integer factor, Row<Integer> row)
{
   return row * factor;
}

template <typename Integer>
Row<Integer>& operator/=(Row<Integer>& row, const Integer divisor)
{
   if ( divisor == 0 )
   {
      throw std::invalid_argument("Integer division by 0 in 'row /= value'");
   }
   if ( divisor != 1 )
   {
      for ( auto& entry : row )
      {
         entry /= divisor;
      }
   }
   return row;
}

template <typename Integer>
Row<Integer> operator/(Row<Integer> row, const Integer divisor)
{
   row /= divisor;
   return row;
}

template <typename Integer>
Integer operator*(const Row<Integer>& first, const Row<Integer>& second) noexcept
{
   assert( first.size() == second.size() );
   return std::inner_product(first.cbegin(), first.cend(), second.cbegin(), Integer(0));
}

template <typename Integer>
Row<Integer> panda::algorithm::normalize(Row<Integer> row, const Equations<Integer>& equations)
{
   for ( const auto& eq : equations )
   {
      const auto non_zero_it = std::find_if(eq.cbegin(), eq.cend(), [](const Integer& a) { return a != 0; });
      const auto index = static_cast<std::size_t>(non_zero_it - eq.cbegin());
      if ( row[index] != 0 )
      {
         const auto a = row[index];
         const auto b = eq[index];
         row *= b;
         row -= a * eq;
      }
   }
   const auto gcd_value = gcd(row);
   if ( gcd_value > 1 )
   {
      row /= gcd_value;
   }
   return row;
}

template <typename Integer>
Integer panda::algorithm::gcd(const Row<Integer>& row) noexcept
{
   Integer value(0);
   std::size_t i{0};
   for ( ; i < row.size() && row[i] == 0; ++i )
   {
   }
   if ( i < row.size() )
   {
      using std::abs;
      using algorithm::abs;
      value = abs(row[i]);
      ++i;
   }
   for ( ; i < row.size() && value > 1; ++i )
   {
      value = gcd(row[i], value);
   }
   return value;
}

template <typename Integer>
Integer panda::algorithm::lcm(const Row<Integer>& row) noexcept
{
   Integer value(1);
   std::size_t i{0};
   for ( ; i < row.size() && row[i] == 1; ++i )
   {
   }
   if ( i < row.size() )
   {
      using std::abs;
      using algorithm::abs;
      value = abs(row[i]);
      ++i;
   }
   for ( ; i < row.size(); ++i )
   {
      value = lcm(row[i], value);
   }
   return value;
}

