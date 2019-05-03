
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <utility>

namespace panda
{
   template <typename>
   struct ReverseRange;

   /// Creates a ReverseRange that makes a container accessible in reverse order.
   /// Recommended usage is
   /// for ( auto& : makeReverseRange(range) )
   /// {
   ///    ...
   /// }
   template <typename Container>
   ReverseRange<Container> makeReverseRange(Container&&);

   /// A ReverseRange is a utility struct for accessing elements of a range in reverse order in a range-based-for-loop.
   /// Recommended usage is
   /// for ( auto& : makeReverseRange(range) )
   /// {
   ///    ...
   /// }
   template <typename Container>
   struct ReverseRange
   {
      // friends
         /// makeReverseRange is a friend of ReverseRange, to be able to access the constructor.
         friend ReverseRange<Container> makeReverseRange<Container>(Container&&);
      // private types
      private:
         /// Type of iterator pointing to the last element.
         using BeginType = decltype(std::declval<Container>().rbegin());
         /// Type of iterator pointing to the first element.
         using EndType = decltype(std::declval<Container>().rend());
      // interface methods
      public:
         /// returns an iterator pointing to the last element of the range.
         /// Not intended for direct usage (implicitely in range-based for loops only).
         BeginType begin();
         /// returns an iterator pointing to the first element of the range.
         /// Not intended for direct usage (implicitely in range-based for loops only).
         EndType end();
      // private methods
      private:
         /// Private constructor. use makeReverseRange to create a ReverseRange instead.
         ReverseRange(Container&);
      // private data
      private:
         /// Reference to the actual container.
         Container& container_ref;
   };
}

#include "range.tpp"

