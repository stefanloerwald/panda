
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_ALGORITHM_FOURIER_MOTZKIN_ELIMINATION
#include "algorithm_fourier_motzkin_elimination.h"
#undef COMPILE_TEMPLATE_ALGORITHM_FOURIER_MOTZKIN_ELIMINATION

#include <algorithm>
#include <cassert>
#include <forward_list>
#include <iostream>
#include <limits>
#include <utility>

#include "algorithm_matrix_operations.h"
#include "algorithm_row_operations.h"
#include "bitset_fixed_size.h"
#include "bitset_variable_size.h"
#include "delayed_action.h"
#include "range.h"

using namespace panda;

namespace
{
   using Index = std::size_t;
   using Indices = std::vector<Index>;
   using ColumnIndex = std::size_t;
   /// Chooses the correct Bitset type.
   template <typename Integer>
   void phaseTwoDispatch(Matrix<Integer>&, const Vertices<Integer>&);
   /// The actual FME, named phase Two in Christof.
   template <typename Bitset, typename Integer>
   void phaseTwo(Matrix<Integer>&, const Vertices<Integer>&);
   /// Abortable phase Two.
   template <typename Bitset, typename Integer>
   void phaseTwoHeuristic(Matrix<Integer>&, const Vertices<Integer>&);
   /// Identifies indices of positive, zero and negative entries.
   template <typename Integer>
   std::tuple<Indices, Indices, Indices> getIndicesNZP(const Row<Integer>&);
   /// Identifies indices of positive, zero and negative entries.
   template <typename Integer, typename Bitset>
   std::tuple<Indices, Indices, Indices> getIndicesNZP(const Row<Integer>&, const std::vector<Bitset>&, const std::size_t);
   /// Replaces the system of matrix and indices.
   template <typename Bitset, typename Integer>
   std::pair<Matrix<Integer>, std::vector<Bitset>> updateSystem(
      const Matrix<Integer>&,
      const std::vector<Bitset>&,
      const Index,
      const std::tuple<Indices, Indices, Indices>&,
      const Row<Integer>&,
      const std::forward_list<std::tuple<Index, Index, Bitset>>&);
   /// Checks minimality of the new system.
   template <typename Bitset>
   bool isMinimal(const Bitset&, const std::forward_list<std::tuple<Index, Index, Bitset>>&, const std::size_t);
   /// After extraction of equations, zero columns remain that can be removed to reduce memory usage.
   template <typename Integer>
   std::vector<ColumnIndex> eliminateZeroColumns(Matrix<Integer>&, Vertices<Integer>&);
   /// After phase 2, zero columns need to be reinserted.
   template <typename Integer>
   void reinsertZeroColumns(Matrix<Integer>&, const std::vector<ColumnIndex>&);
   /// Initialization of bitsets in phase 2.
   template <typename Bitset, typename Integer>
   std::vector<Bitset> initializeR(const Matrix<Integer>&, const Vertices<Integer>&);
   /// Elimination of one ray.
   template <typename Bitset, typename Integer>
   void projection(Matrix<Integer>&, std::vector<Bitset>&, const Vertex<Integer>&, const Index);
}

template <typename Integer>
Matrix<Integer> panda::algorithm::fourierMotzkinElimination(Matrix<Integer> input)
{
   assert( !input.empty() );
   auto matrix = input;
   appendNegativeIdentityMatrix(matrix);
   Indices used_indices;
   Indices equation_indices;
   std::tie(equation_indices, used_indices) = gaussianElimination(matrix);
   matrix.erase(matrix.begin(), matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(input.size()));
   assert( !matrix.empty() );
   assert( matrix.size() == matrix.back().size() );
   matrix = transpose(matrix);
   std::sort(equation_indices.rbegin(), equation_indices.rend());
   const auto equations = extractEquations(matrix, equation_indices);
   const auto zero_columns = eliminateZeroColumns(matrix, input);
   assert( std::is_sorted(used_indices.cbegin(), used_indices.cend()) );
   Vertices<Integer> used;
   used.reserve(used_indices.size());
   for ( auto it = used_indices.crbegin(); it != used_indices.crend(); ++it )
   {
      used.push_back(input[*it]);
      input.erase(input.begin() + static_cast<typename Matrix<Integer>::difference_type>(*it));
   }
   input.insert(input.begin(), used.cbegin(), used.cend());
   phaseTwoDispatch(matrix, input);
   reinsertZeroColumns(matrix, zero_columns);
   return matrix;
}

template <typename Integer>
Matrix<Integer> panda::algorithm::fourierMotzkinEliminationHeuristic(Matrix<Integer> input)
{
   assert( !input.empty() );
   auto matrix = input;
   appendNegativeIdentityMatrix(matrix);
   Indices used_indices;
   Indices equation_indices;
   std::tie(equation_indices, used_indices) = gaussianElimination(matrix);
   matrix.erase(matrix.begin(), matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(input.size()));
   assert( !matrix.empty() );
   assert( matrix.size() == matrix.back().size() );
   matrix = transpose(matrix);
   std::sort(equation_indices.rbegin(), equation_indices.rend());
   const auto equations = extractEquations(matrix, equation_indices);
   const auto zero_columns = eliminateZeroColumns(matrix, input);
   assert( std::is_sorted(used_indices.cbegin(), used_indices.cend()) );
   Vertices<Integer> used;
   used.reserve(used_indices.size());
   for ( auto it = used_indices.crbegin(); it != used_indices.crend(); ++it )
   {
      used.push_back(input[*it]);
      input.erase(input.begin() + static_cast<typename Matrix<Integer>::difference_type>(*it));
   }
   input.insert(input.begin(), used.cbegin(), used.cend());
   phaseTwoHeuristic<BitsetVariableSize>(matrix, input);
   reinsertZeroColumns(matrix, zero_columns);
   return matrix;
}

namespace
{
   /// Phase 2 of Fourier Motzkin Elimination needs an efficient encoding of sets.
   ///  A bitset is used for this encoding. However, bitsets with a dynamic number of
   ///  bits are less efficient than bitsets with a fixed maximum size.
   ///  This is due to stack versus heap performance.
   ///  As the number of bits equals the number of vertices, we can decide the optimal type beforehand.

   ///  As the stack size is limited, using fixed size bitsets is not scalable.
   ///  Hence, only for a small number of vertices, a fixed size bitset is used.
   ///  For all other, a generalized, heap-based, bitset is used.

   /// This method automatically chooses the optimal bitset type and executes the phase 2.
   template <typename Integer>
   void phaseTwoDispatch(Matrix<Integer>& matrix, const Vertices<Integer>& vertices)
   {
      assert( !vertices.empty() );
      static_assert(std::is_same<BitsetFixedSize<1u>::DataType, BitsetVariableSize::DataType>::value, "The datatypes of BitsetFixedSize and BitsetVariableSize do not match. This is crucial for the optimal choice of type.");
      const auto bitset_size = 1 + (vertices.size() - 1) / std::numeric_limits<typename BitsetFixedSize<1u>::DataType>::digits;
      if ( bitset_size <= 1u )
      {
         phaseTwo<BitsetFixedSize<1u>>(matrix, vertices);
      }
      else if ( bitset_size <= 2u )
      {
         phaseTwo<BitsetFixedSize<2u>>(matrix, vertices);
      }
      else if ( bitset_size <= 3u )
      {
         phaseTwo<BitsetFixedSize<3u>>(matrix, vertices);
      }
      else if ( bitset_size <= 4u )
      {
         phaseTwo<BitsetFixedSize<4u>>(matrix, vertices);
      }
      else if ( bitset_size <= 6u )
      {
         phaseTwo<BitsetFixedSize<6u>>(matrix, vertices);
      }
      else if ( bitset_size <= 8u )
      {
         phaseTwo<BitsetFixedSize<8u>>(matrix, vertices);
      }
      else if ( bitset_size <= 10u )
      {
         phaseTwo<BitsetFixedSize<10u>>(matrix, vertices);
      }
      else if ( bitset_size <= 12u )
      {
         phaseTwo<BitsetFixedSize<12u>>(matrix, vertices);
      }
      else if ( bitset_size <= 16u )
      {
         phaseTwo<BitsetFixedSize<16u>>(matrix, vertices);
      }
      else if ( bitset_size <= 20u )
      {
         phaseTwo<BitsetFixedSize<20u>>(matrix, vertices);
      }
      else if ( bitset_size <= 30u )
      {
         phaseTwo<BitsetFixedSize<30u>>(matrix, vertices);
      }
      else if ( bitset_size <= 40u )
      {
         phaseTwo<BitsetFixedSize<40u>>(matrix, vertices);
      }
      else if ( bitset_size <= 50u )
      {
         phaseTwo<BitsetFixedSize<50u>>(matrix, vertices);
      }
      else if ( bitset_size <= 75u )
      {
         phaseTwo<BitsetFixedSize<75u>>(matrix, vertices);
      }
      else if ( bitset_size <= 100u )
      {
         phaseTwo<BitsetFixedSize<100u>>(matrix, vertices);
      }
      else if ( bitset_size <= 150u )
      {
         phaseTwo<BitsetFixedSize<150u>>(matrix, vertices);
      }
      else if ( bitset_size <= 200u )
      {
         phaseTwo<BitsetFixedSize<200u>>(matrix, vertices);
      }
      else
      {
         phaseTwo<BitsetVariableSize>(matrix, vertices);
      }
   }

   template <typename Integer>
   Facets<Integer> extractFacets(const Matrix<Integer>& matrix, const Vertices<Integer>& vertices, const std::size_t start)
   {
      const auto& vs = vertices;
      Facets<Integer> facets;
      for ( const auto& row : matrix )
      {
         if ( std::all_of(vs.cbegin() + static_cast<typename Vertices<Integer>::difference_type>(start), vs.cend(), [&row](const Row<Integer>& v) { return row * v <= 0; }) )
         {
            facets.push_back(row);
         }
      }
      return facets;
   }

   template <typename Bitset>
   bool countCheck(const Bitset& Rn, const Bitset& Rp, const std::size_t max_count, const std::size_t max)
   {
      return Bitset::unionCount(Rn, Rp, max) <= max_count;
   }

   template <typename Bitset>
   bool containmentCheck(const Bitset& Rn, const Bitset& Rp, const std::size_t max, const std::vector<Bitset>& R, const Indices& indices_zero)
   {
      return std::all_of(indices_zero.cbegin(), indices_zero.cend(), [&](const std::size_t index_z)
      {
         return !Bitset::unionContains(Rn, Rp, R[index_z], max);
      });
   }

   template <typename Bitset>
   void pnrIteration(std::forward_list<std::tuple<Index, Index, Bitset>>& pnrs,
                     const std::size_t index_n,
                     const std::size_t index_p,
                     const Bitset& u,
                     const std::size_t max)
   {
      using Iterator = typename std::forward_list<std::tuple<Index, Index, Bitset>>::iterator;
      std::vector<Iterator> removal;
      for ( auto it = pnrs.begin(), bit = pnrs.before_begin(); it != pnrs.end(); ++it, ++bit )
      {
         const auto& Rpnr = std::get<2>(*it);
         if ( u.contains(Rpnr, max) )
         {
            return;
         }
         if ( Rpnr.contains(u, max) )
         {
            removal.push_back(bit);
         }
      }
      for ( const auto& entry : makeReverseRange(removal) )
      {
         pnrs.erase_after(entry);
      }
      pnrs.push_front(std::make_tuple(index_n, index_p, u));
   }

   template <typename Bitset, typename Integer>
   void projection(Matrix<Integer>& matrix, std::vector<Bitset>& R, const Vertex<Integer>& vertex, const Index index)
   {
      assert( !matrix.empty() );
      const auto d = vertex.size();
      assert( matrix.back().size() == d );
      assert( index >= d );
      const auto max_count = index + 2 - d;
      auto s = matrix * vertex;
      const auto indices = getIndicesNZP(s, R, index);
      const auto& indices_negative = std::get<0>(indices);
      const auto& indices_zero = std::get<1>(indices);
      const auto& indices_positive = std::get<2>(indices);
      std::forward_list<std::tuple<Index, Index, Bitset>> pnrs;
      for ( const auto& index_n : indices_negative )
      {
         const auto& Rn = R[index_n];
         for ( const auto& index_p : indices_positive )
         {
            const auto& Rp = R[index_p];
            if ( countCheck(Rn, Rp, max_count, index) )
            {
               if ( containmentCheck(Rn, Rp, index, R, indices_zero) )
               {
                  const auto u = Rn.merge(Rp, index);
                  pnrIteration(pnrs, index_n, index_p, u, index);
               }
            }
         }
      }
      std::tie(matrix, R) = updateSystem(matrix, R, index, indices, s, pnrs);
   }

   template <typename Integer>
   void detectBadRow(Matrix<Integer>& matrix)
   {
      for ( std::size_t j = 0; j < matrix.size(); ++j )
      {
         const auto& row = matrix[j];
         bool possible = true;
         for ( std::size_t k = 0; k + 1 < row.size() && possible; ++k )
         {
            possible = (row[k] == 0);
         }
         if ( possible && row.back() == -1 )
         {
            matrix.erase(matrix.begin() + static_cast<typename Matrix<Integer>::difference_type>(j));
         }
      }
   }

   template <typename Bitset, typename Integer>
   void phaseTwo(Matrix<Integer>& matrix, const Vertices<Integer>& vertices)
   {
      assert( !matrix.empty() );
      const auto d = matrix.back().size();
      auto R = initializeR<Bitset>(matrix, vertices);
      assert( d <= vertices.size() );
      for ( std::size_t i = d; i < vertices.size(); ++i )
      {
         const auto& vertex = vertices[i];
         auto action = makeDelayedAction([&]()
         {
            std::cerr << "Fourier-Motzkin Elimination step " << i + 1 << " / " << vertices.size() << ": " << matrix.size() << '\n';
         }, std::chrono::seconds(2));
         projection(matrix, R, vertex, i);
      }
      detectBadRow(matrix);
   }

   template <typename Bitset, typename Integer>
   void phaseTwoHeuristic(Matrix<Integer>& matrix, const Vertices<Integer>& vertices)
   {
      const auto d = matrix.size();
      auto R = initializeR<Bitset>(matrix, vertices);
      assert( d <= vertices.size() );
      for ( std::size_t i = d; i < vertices.size(); ++i )
      {
         auto facets = extractFacets(matrix, vertices, i);
         detectBadRow(facets);
         if ( !facets.empty() )
         {
            matrix = facets;
            break;
         }
         projection(matrix, R, vertices[i], i);
      }
   }

   template <typename Integer>
   std::tuple<Indices, Indices, Indices> getIndicesNZP(const Row<Integer>& s)
   {
      std::tuple<Indices, Indices, Indices> indices;
      auto& indices_negative = std::get<0>(indices);
      auto& indices_zero = std::get<1>(indices);
      auto& indices_positive = std::get<2>(indices);
      for ( std::size_t j = 0; j < s.size(); ++j )
      {
         if ( s[j] < 0 )
         {
            indices_negative.push_back(j);
         }
         else if ( s[j] == 0 )
         {
            indices_zero.push_back(j);
         }
         else // ( s[j] > 0 )
         {
            indices_positive.push_back(j);
         }
      }
      return indices;
   }

   template <typename Integer, typename Bitset>
   std::tuple<Indices, Indices, Indices> getIndicesNZP(const Row<Integer>& s, const std::vector<Bitset>& R, const std::size_t max)
   {
      auto indices = getIndicesNZP(s);
      auto& indices_zero = std::get<1>(indices);
      std::sort(indices_zero.begin(), indices_zero.end(), [&R, &max] (const Index& a, const Index& b) { return R[a].count(max) < R[b].count(max); });
      return indices;
   }

   template <typename Bitset, typename Integer>
   std::pair<Matrix<Integer>, std::vector<Bitset>> updateSystem(
      const Matrix<Integer>& matrix,
      const std::vector<Bitset>& R,
      const Index i,
      const std::tuple<Indices, Indices, Indices>& indices,
      const Row<Integer>& s,
      const std::forward_list<std::tuple<Index, Index, Bitset>>& pnrs)
   {
      const auto& indices_negative = std::get<0>(indices);
      const auto& indices_zero = std::get<1>(indices);
      Matrix<Integer> new_matrix;
      std::vector<Bitset> new_R;
      new_matrix.reserve(indices_negative.size() + indices_zero.size());
      new_R.reserve(indices_negative.size() + indices_zero.size());
      for ( const auto index_z : indices_zero )
      {
         new_matrix.push_back(matrix[index_z]);
         new_R.push_back(R[index_z]);
      }
      for ( const auto index_n : indices_negative )
      {
         new_matrix.push_back(matrix[index_n]);
         new_R.push_back(R[index_n]);
         new_R.back().set(i);
      }
      for ( const auto& pnr : pnrs )
      {
         const auto& index_n = std::get<0>(pnr);
         const auto& index_p = std::get<1>(pnr);
         new_matrix.push_back(s[index_p] * matrix[index_n] - s[index_n] * matrix[index_p]);
         const auto gcd_value = algorithm::gcd(new_matrix.back());
         if ( gcd_value > 1 )
         {
            new_matrix.back() /= gcd_value;
         }
         new_R.push_back(std::get<2>(pnr));
      }
      return std::make_pair(new_matrix, new_R);
   }

   template <typename Bitset>
   bool isMinimal(const Bitset& bitset, const std::forward_list<std::tuple<Index, Index, Bitset>>& pnrs, const std::size_t max)
   {
      for ( const auto& pnr : pnrs )
      {
         const auto& Rpnr = std::get<2>(pnr);
         if ( bitset.contains(Rpnr, max) )
         {
            return false;
         }
      }
      return true;
   }

   template <typename Integer>
   std::vector<ColumnIndex> eliminateZeroColumns(Matrix<Integer>& matrix, Vertices<Integer>& vertices)
   {
      Indices zero_columns;
      for ( std::size_t col = matrix.back().size(); col > 0; )
      {
         --col;
         const auto all_zero = std::all_of(matrix.cbegin(), matrix.cend(), [&col](const Row<Integer>& row)
         {
            return (row[col] == 0);
         });
         if ( all_zero )
         {
            zero_columns.push_back(col);
            for ( auto& row : matrix )
            {
               row.erase(row.begin() + static_cast<typename Row<Integer>::difference_type>(col));
            }
            for ( auto& vertex : vertices )
            {
               vertex.erase(vertex.begin() + static_cast<typename Vertex<Integer>::difference_type>(col));
            }
         }
      }
      assert( std::is_sorted(zero_columns.crbegin(), zero_columns.crend()) );
      return zero_columns;
   }

   template <typename Integer>
   void reinsertZeroColumns(Matrix<Integer>& matrix, const std::vector<ColumnIndex>& zero_columns)
   {
      assert( std::is_sorted(zero_columns.crbegin(), zero_columns.crend()) );
      for ( const auto col : makeReverseRange(zero_columns) )
      {
         for ( auto& row : matrix )
         {
            row.insert(row.begin() + static_cast<const typename Row<Integer>::difference_type>(col), Integer(0));
         }
      }
   }

   template <typename Bitset, typename Integer>
   std::vector<Bitset> initializeR(const Matrix<Integer>& matrix, const Vertices<Integer>& vertices)
   {
      assert( !matrix.empty() );
      assert( matrix.size() == matrix.back().size() );
      assert( !vertices.empty() );
      assert( matrix.back().size() == vertices.back().size() );
      const auto d = matrix.size();
      assert( d <= vertices.size() );
      std::vector<Bitset> R(matrix.size(), Bitset(vertices.size()));
      for ( std::size_t i = 0; i < matrix.size(); ++i )
      {
         const auto& row = matrix[i];
         for ( std::size_t j = 0; j < d; ++j )
         {
            const auto& vertex = vertices[j];
            if ( row * vertex < 0 )
            {
               R[i].set(j);
            }
         }
      }
      return R;
   }
}

