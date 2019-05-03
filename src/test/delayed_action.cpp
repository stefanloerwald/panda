
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "delayed_action.h"

#include <thread>

using namespace panda;

namespace
{
   void exception();
   void executed();
   void not_executed_early();
   void not_executed_dismissed();
}

int main()
try
{
   exception();
   executed();
   not_executed_early();
   not_executed_dismissed();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   using std::chrono::seconds;
   using std::chrono::milliseconds;

   void exception()
   {
      ASSERT_NOTHROW(makeDelayedAction([](){}, seconds(1)), "Creating a delayed action may not throw.");
   }

   void executed()
   {
      bool executed = false;
      {
         auto action = makeDelayedAction([&executed](){ executed = true; }, seconds(0));
      }
      std::string message = "The scope for the action object should have been closed";
      message += " after the minimum waiting time.";
      ASSERT(executed, message);
      executed = false;
      {
         auto action = makeDelayedAction([&executed](){ executed = true; }, milliseconds(1));
         std::this_thread::sleep_for(milliseconds(10));
      }
      message = "The scope for the action object should have been closed after the minimum waiting time.";
      ASSERT(executed, message);
   }

   void not_executed_early()
   {
      bool executed = false;
      {
         auto action = makeDelayedAction([&executed](){ executed = true; }, seconds(10));
      }
      std::string message = "The scope for the action object should have been closed";
      message += " before the minimum waiting time.";
      ASSERT(!executed, message);
   }

   void not_executed_dismissed()
   {
      bool executed = false;
      {
         auto action = makeDelayedAction([&executed](){ executed = true; }, seconds(0));
         action.dismiss();
      }
      ASSERT(!executed, "Action was dismissed.");
   }
}

