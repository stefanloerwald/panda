
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <chrono>

namespace panda
{
   template <typename Runnable>
   class DelayedAction;

   /// Creates a delayed action that runs if at destruction the elapsed time since
   /// creation of the delayed action exceeds the specified amount of seconds.
   template <typename Runnable>
   DelayedAction<Runnable> makeDelayedAction(Runnable&&, std::chrono::seconds);
   /// Creates a delayed action that runs if at destruction the elapsed time since
   /// creation of the delayed action exceeds the specified amount of milliseconds.
   template <typename Runnable>
   DelayedAction<Runnable> makeDelayedAction(Runnable&&, std::chrono::milliseconds);
   /// Creates a delayed action that runs if at destruction the elapsed time since
   /// creation of the delayed action exceeds the specified amount of nanoseconds.
   template <typename Runnable>
   DelayedAction<Runnable> makeDelayedAction(Runnable&&, std::chrono::nanoseconds);

   /// A delayed action is a wrapper for executing a function at the end of a scope given that a time limit expired.
   /// The action will be executed only if the time limit expired and the action was not dismissed.
   /// It will only ever execute at the end of scope, so it doesn't execute directly after the time provided.
   template <typename Runnable>
   class DelayedAction
   {
      public:
         friend DelayedAction<Runnable> makeDelayedAction<>(Runnable&&, std::chrono::nanoseconds);
      private:
         /// Constructor.
         DelayedAction(Runnable&&, std::chrono::nanoseconds);
      public:
         /// Destructor.
         ~DelayedAction();
         /// Deleted copy constructor.
         DelayedAction(const DelayedAction&) = delete;
         /// Default move constructor.
         DelayedAction(DelayedAction&&) = default;
         /// If the delayed action is dismissed, the action will not be performed
         /// no matter how the scope is exited.
         void dismiss() noexcept;
      private:
         bool dismissed;
         Runnable runnable;
         std::chrono::time_point<std::chrono::system_clock> start;
         std::chrono::nanoseconds minimum_waiting_time;
   };
}

#include "delayed_action.tpp"

