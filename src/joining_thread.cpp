
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "joining_thread.h"

using namespace panda;

panda::JoiningThread::~JoiningThread()
{
   if ( thread.joinable() )
   {
      thread.join();
   }
}

