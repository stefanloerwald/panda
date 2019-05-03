
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_MATRIX_OPERATIONS
#include "algorithm_matrix_operations.h"
#undef COMPILE_TEMPLATE_ALGORITHM_MATRIX_OPERATIONS

#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>

#include "algorithm_integer_operations.h"
#include "algorithm_row_operations.h"

using namespace panda;

template <typename Integer>
std::ostream& operator<<(std::ostream& output, const panda::Matrix<Integer>& matrix)
{
   for ( const auto& row : matrix )
   {
      output << row << '\n';
   }
   return output;
}

template <typename Integer>
void panda::algorithm::prettyPrint(std::ostream& output, const Matrix<Integer>& matrix, const Names& names, const char* relation)
{
   for ( const auto& row : matrix )
   {
      prettyPrintln(output, row, names, relation);
   }
}

template <typename Integer>
Row<Integer> operator*(const Matrix<Integer>& matrix, const Row<Integer>& vector)
{
   assert( !matrix.empty() && matrix.back().size() == vector.size() );
   Row<Integer> result(matrix.size());
   for ( std::size_t i = 0; i < matrix.size(); ++i )
   {
      result[i] = matrix[i] * vector;
   }
   return result;
}

template <typename Integer>
Matrix<Integer> algorithm::transpose(const Matrix<Integer>& matrix)
{
   assert( !matrix.empty() && !matrix.back().empty() );
   Matrix<Integer> result(matrix.back().size(), Row<Integer>(matrix.size()));
   for ( std::size_t i = 0; i < matrix.back().size(); ++i )
   {
      for ( std::size_t j = 0; j < matrix.size(); ++j )
      {
         result[i][j] = matrix[j][i];
      }
   }
   return result;
}

// implementation details for template <typename Integer> std::pair<Indices, Indices> gaussianElimination(Matrix<Integer>&);
namespace
{
   using RowIndex = std::size_t;
   using ColumnIndex = std::size_t;
   template <typename Integer>
   void eliminateColumns(Matrix<Integer>&, const RowIndex, const ColumnIndex);
   template <typename Integer>
   std::pair<RowIndex, ColumnIndex> pivot(Matrix<Integer>&, std::size_t, std::vector<ColumnIndex>&);
}

template <typename Integer>
std::size_t algorithm::dimension(Matrix<Integer> matrix)
{
   assert( !matrix.empty() && !matrix.back().empty() );
   std::vector<std::size_t> pivot_columns;
   for ( std::size_t i = 0; i < matrix.size(); ++i )
   {
      auto& row = matrix[i];
      assert( i == pivot_columns.size() );
      for ( std::size_t j = 0; j < i; ++j ) // use row j to eliminate coefficients in row i.
      {
         const auto piv_col = pivot_columns[j];
         if ( row[piv_col] != 0 )
         {
            const auto factor = row[piv_col];
            row *= Integer(-matrix[j][piv_col]); // Integer type name necessary because of integral promotion of short.
            row += factor * matrix[j];
            const auto gcd_value = gcd(row);
            if ( gcd_value > 1 )
            {
               row /= gcd_value;
            }
         }
      }
      const auto nz_entry = std::find_if(row.cbegin(), row.cend(), [](const Integer& a) { return a != 0; });
      if ( nz_entry == row.cend() )
      {
         matrix.erase(matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(i));
         --i;
      }
      else
      {
         pivot_columns.push_back(static_cast<std::size_t>(nz_entry - row.cbegin()));
      }
   }
   return pivot_columns.size();
}

template <typename Integer>
std::pair<Indices, Indices> algorithm::gaussianElimination(Matrix<Integer>& matrix)
{
   assert( !matrix.empty() && !matrix.back().empty() );
   Indices L;
   Indices T;
   const auto row_size = matrix.size();
   const auto col_size = matrix.back().size();
   const auto t = matrix.size() - matrix.back().size();
   std::vector<ColumnIndex> used_columns;
   used_columns.reserve(col_size);
   for ( RowIndex row = 0; row < row_size; ++row )
   {
      std::size_t col;
      std::tie(row, col) = pivot(matrix, row, used_columns);
      if ( row == row_size )
      {
         break;
      }
      if ( row >= t )
      {
         L.push_back(col);
      }
      else
      {
         T.push_back(row);
      }
      eliminateColumns(matrix, row, col);
   }
   for ( ColumnIndex col = 0; col < col_size; ++col )
   {
      using Type = Row<Integer>;
      const auto& m = matrix;
      const auto& pivot_row = *std::find_if(m.cbegin(), m.cend(), [col](const Type& row) { return row[col] != 0; });
      if ( pivot_row[col] < 0 )
      {
         for ( RowIndex row = 0; row < row_size; ++row )
         {
            matrix[row][col] *= Integer(-1);
         }
      }
   }
   return std::make_pair(L, T);
}

template <typename Integer>
void algorithm::appendNegativeIdentityMatrix(Matrix<Integer>& matrix)
{
   assert( !matrix.empty() );
   const auto dim = matrix.back().size();
   matrix.reserve(matrix.size() + dim);
   for ( std::size_t i = 0; i < dim; ++i )
   {
      matrix.insert(matrix.end(), Vertex<Integer>(dim, Integer(0)));
      matrix.back()[i] = Integer(-1);
   }
}

template <typename Integer>
Equations<Integer> algorithm::extractEquations(Matrix<Integer> matrix)
{
   assert( !matrix.empty() );
   const auto original_size = matrix.size();
   appendNegativeIdentityMatrix(matrix);
   Indices used_indices;
   Indices equation_indices;
   std::tie(equation_indices, used_indices) = gaussianElimination(matrix);
   matrix.erase(matrix.begin(), matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(original_size));
   assert( !matrix.empty() && matrix.size() == matrix.back().size() );
   matrix = transpose(matrix);
   std::sort(equation_indices.rbegin(), equation_indices.rend());
   return extractEquations(matrix, equation_indices);
}

template <typename Integer>
Equations<Integer> algorithm::extractEquations(Matrix<Integer>& matrix, const Indices& indices)
{
   assert( !matrix.empty() );
   assert( std::is_sorted(indices.crbegin(), indices.crend()) );
   for ( std::size_t i = 0; i < indices.size(); ++i )
   {
      std::swap(matrix[indices[i]], matrix[matrix.size() - 1 - i]);
   }
   Equations<Integer> equations(matrix.rbegin(), matrix.rbegin() + static_cast<typename Matrix<Integer>::difference_type>(indices.size()));
   matrix.erase(matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(matrix.size() - indices.size()), matrix.end());
   return equations;
}

namespace
{
   template <typename Integer>
   void normalizeColumn(Matrix<Integer>& matrix, const ColumnIndex column)
   {
      const auto row_size = matrix.size();
      Integer gcd_val(0);
      for ( RowIndex j = 0; j < row_size; ++j )
      {
         gcd_val = algorithm::gcd(gcd_val, matrix[j][column]);
      }
      if ( gcd_val > 1 )
      {
         for ( RowIndex j = 0; j < row_size; ++j )
         {
            matrix[j][column] /= gcd_val;
         }
      }
   }

   template <typename Integer>
   void eliminateColumn(Matrix<Integer>& matrix, const RowIndex row, const ColumnIndex col, const ColumnIndex ecol)
   {
      const auto row_size = matrix.size();
      const auto a = -matrix[row][col];
      const auto b =  matrix[row][ecol];
      for ( RowIndex j = 0; j < row_size; ++j )
      {
         matrix[j][ecol] *= a;
         matrix[j][ecol] += (matrix[j][col] * b);
      }
   }

   /// Eliminate all entries in column col except row to 0, by adding rows (using row "row").
   template <typename Integer>
   void eliminateColumns(Matrix<Integer>& matrix, const RowIndex row, const ColumnIndex col)
   {
      assert( !matrix.empty() );
      assert( row < matrix.size() );
      assert( col < matrix.back().size() );
      const auto col_size = matrix.back().size();
      for ( ColumnIndex i = 0; i < col_size; ++i )
      {
         const auto b = matrix[row][i];
         if ( i != col && b != 0 )
         {
            eliminateColumn(matrix, row, col, i);
            normalizeColumn(matrix, i);
         }
      }
   }

   /// Search row-wise to get the first (row, col) with matrix[row][col] != 0.
   template <typename Integer>
   std::pair<RowIndex, ColumnIndex> pivot(Matrix<Integer>& matrix,
                                          std::size_t iteration,
                                          std::vector<ColumnIndex>& used_columns)
   {
      assert( !matrix.empty() );
      assert( iteration < matrix.size() );
      const auto row_size = matrix.size();
      const auto col_size = matrix.back().size();
      ColumnIndex row = iteration;
      ColumnIndex col = col_size;
      for ( ; row < row_size; ++row )
      {
         for ( col = 0; col < col_size; ++col )
         {
            if ( matrix[row][col] != 0 && !std::binary_search(used_columns.cbegin(), used_columns.cend(), col) )
            {
               break;
            }
         }
         if ( col < col_size )
         {
            break;
         }
      }
      used_columns.push_back(col);
      std::sort(used_columns.begin(), used_columns.end());
      return std::make_pair(row, col);
   }
}

