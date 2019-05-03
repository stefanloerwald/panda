
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <new>

namespace panda
{
   template <typename Function>
   class ScopeGuard;

   /// Creates a ScopeGuard with proper type.
   /// Do not construct a ScopeGuard yourself as you need to specify the template parameter yourself.
   /// Recommended usage:
   /// auto guard = makeScopeGuard([]()
   /// {
   ///    // cleanup
   /// });
   template <typename Function, typename decayType = typename std::decay<Function>::type>
   ScopeGuard<decayType> makeScopeGuard(Function&&);

   /// A ::ScopeGuard is used to assert that a certain function is executed
   /// at the end of a scope no matter how the scope is exited. If a method
   /// might throw or returns at several points, it is tedious to perform
   /// proper clean-up on every exit point. A ::ScopeGuard automates this.
   template <typename Function>
   class ScopeGuard
   {
      // friends
         /// friend function to construct a ScopeGuard without explicitely naming the template type.
         friend ScopeGuard<Function> makeScopeGuard<Function>(Function&&);
      // interface methods
      public:
         /// Move-Constructor.
         ScopeGuard(ScopeGuard&&);
         /// Destructor, which executes the method, unless dismissed.
         ~ScopeGuard() noexcept;
         /// If a ::ScopeGuard is dismissed, the function won't get executed on destruction.
         void dismiss() noexcept;
      // private data
      private:
         bool dismissed;
         Function function;
      // private methods
      private:
         /// Constructor taking the method that will be executed on destruction unless dismissed.
         /// Private, because makeScopeGuard is a cleaner way to construct a ScopeGuard.
         explicit ScopeGuard(const Function&);
         /// Constructor taking the method that will be executed on destruction unless dismissed.
         /// Private, because makeScopeGuard is a cleaner way to construct a ScopeGuard.
         explicit ScopeGuard(Function&&);
      // explicitely deleted methods
      private:
         /// Deleted default constructor. There is no such thing as a default ScopeGuard.
         ScopeGuard() = delete;
         /// Deleted copy constructor. ScopeGuards are not meant to be passed around,
         /// as this violates the idea of beeing bound to a scope.
         ScopeGuard(const ScopeGuard&) = delete;
         /// Deleted dynamic allocation. ScopeGuards are designed to only live in the
         /// current scope, hence dynamic allocation is useless.
         void* operator new(std::size_t) = delete;
   };
}

#include "scope_guard.tpp"

