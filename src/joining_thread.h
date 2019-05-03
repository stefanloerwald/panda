
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <thread>

namespace panda
{
   /// Wrapper for std::thread that automatically joins at end of scope.
   class JoiningThread
   {
      public:
         /// Constructor taking a Function with its arguments.
         template <typename Function, typename... Args>
         JoiningThread(Function&&, Args&&...);
         /// Default move constructor.
         JoiningThread(JoiningThread&&) = default;
         /// Destructor.
         ~JoiningThread();
         /// Copy constructor is deleted.
         JoiningThread(const JoiningThread&) = delete;
         /// Copy assignment operator is deleted.
         JoiningThread& operator=(const JoiningThread&) = delete;
      private:
         std::thread thread;
   };
}

#include "joining_thread.tpp"

