
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "list.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace panda;

int main()
try
{
   // We do not care for the output here.
   SILENCE_CERR();
   std::cout.rdbuf(nullptr);
   {  // A get call has to block until new data is available
      List<int, tag::facet> list({});
      list.put(Facets<int>{{0}});
      ASSERT(list.get() == Facet<int>{0}, "Data returned is invalid.");
      std::atomic<bool> empty = ATOMIC_VAR_INIT(true);
      std::atomic<bool> ready = ATOMIC_VAR_INIT(false);
      std::mutex mutex;
      std::condition_variable cv;
      std::thread getter([&]()
      {
         {
            std::unique_lock<std::mutex> lock(mutex);
            ready.store(true);
            cv.notify_all();
         }
         list.get();
         ASSERT(!empty.load(), "The list.get() call may only suceed once the setter has done its job.");
      });
      std::thread setter([&]()
      {
         std::unique_lock<std::mutex> lock(mutex);
         cv.wait(lock, [&]() { return ready.load(); });
         empty.store(false);
         list.put(Facets<int>{{1}});
      });
      getter.join();
      setter.join();
   }
   { // A blocked get call has to be unblocked once the empty state is detected
      List<int, tag::facet> list({});
      list.put(Facets<int>{{0}});
      ASSERT(list.get() == Facet<int>{0}, "Data returned is invalid.");
      std::atomic<bool> empty = ATOMIC_VAR_INIT(true);
      std::atomic<bool> ready = ATOMIC_VAR_INIT(false);
      std::mutex mutex;
      std::condition_variable cv;
      std::thread getter([&]()
      {
         {
            std::unique_lock<std::mutex> lock(mutex);
            ready.store(true);
            cv.notify_all();
         }
         list.get();
         ASSERT(!empty.load(), "The list.get() call may only suceed once the setter has done its job.");
      });
      std::thread setter([&]()
      {
         std::unique_lock<std::mutex> lock(mutex);
         cv.wait(lock, [&]() { return ready.load(); });
         empty.store(false);
         list.put(Facets<int>{{0}});
         list.get();
      });
      getter.join();
      setter.join();
   }
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

