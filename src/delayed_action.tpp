
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <thread>

template <typename Runnable>
panda::DelayedAction<Runnable> panda::makeDelayedAction(Runnable&& runnable, std::chrono::seconds time)
{
   using std::chrono::duration_cast;
   using std::chrono::nanoseconds;
   return makeDelayedAction(std::forward<Runnable>(runnable), duration_cast<nanoseconds>(time));
}

template <typename Runnable>
panda::DelayedAction<Runnable> panda::makeDelayedAction(Runnable&& runnable, std::chrono::milliseconds time)
{
   using std::chrono::duration_cast;
   using std::chrono::nanoseconds;
   return makeDelayedAction(std::forward<Runnable>(runnable), duration_cast<nanoseconds>(time));
}

template <typename Runnable>
panda::DelayedAction<Runnable> panda::makeDelayedAction(Runnable&& runnable, std::chrono::nanoseconds min_time_from_now)
{
   return DelayedAction<Runnable>(std::forward<Runnable>(runnable), min_time_from_now);
}

template <typename Runnable>
panda::DelayedAction<Runnable>::DelayedAction(Runnable&& runnable_, std::chrono::nanoseconds min_time_from_now)
:
   dismissed(false),
   runnable(runnable_),
   start(std::chrono::system_clock::now()),
   minimum_waiting_time(min_time_from_now)
{
}

template <typename Runnable>
panda::DelayedAction<Runnable>::~DelayedAction()
{
   if ( !dismissed && (std::chrono::system_clock::now() - start) >= minimum_waiting_time )
   {
      runnable();
   }
}

template <typename Runnable>
void panda::DelayedAction<Runnable>::dismiss() noexcept
{
   dismissed = true;
}
