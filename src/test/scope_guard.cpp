
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "scope_guard.h"

using namespace panda;

namespace
{
   void regularExit();
   void exception();
   void dismissedRegularExit();
   void dismissedException();
}

int main()
try
{
   regularExit();
   exception();
   dismissedRegularExit();
   dismissedException();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void regularExit()
   {
      int i = 0;
      {
         auto guard = makeScopeGuard([&i]()
         {
            ++i;
         });
      }
      ASSERT(i == 1, "ScopeGuard didn't execute function on scope exit.");
   }
   void exception()
   {
      int i = 0;
      try
      {
         auto guard = makeScopeGuard([&i]()
         {
            ++i;
         });
         throw 1;
      }
      catch ( ... )
      {
      }
      ASSERT(i == 1, "ScopeGuard didn't execute function on scope exit.");
   }
   void dismissedRegularExit()
   {
      int i = 0;
      {
         auto guard = makeScopeGuard([&i]()
         {
            ++i;
         });
         guard.dismiss();
      }
      ASSERT(i == 0, "ScopeGuard executed function on scope exit, although dismissed.");
   }
   void dismissedException()
   {
      int i = 0;
      try
      {
         auto guard = makeScopeGuard([&i]()
         {
            ++i;
         });
         guard.dismiss();
         throw 1;
      }
      catch ( ... )
      {
      }
      ASSERT(i == 0, "ScopeGuard executed function on scope exit, although dismissed.");
   }
}

