
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#ifdef HAS_FEATURE_THREAD_LOCAL
   #error Multiple definition of macro HAS_FEATURE_THREAD_LOCAL
#endif

#ifdef __GNUC__
   // thread_local is not supported prior to gcc 4.8
   #if __GNUC__ < 4  || (__GNUC__ == 4 && __GNUC_MINOR__ <= 7)
      #define HAS_FEATURE_THREAD_LOCAL 0
   #else
      #define HAS_FEATURE_THREAD_LOCAL 1
   #endif
#else
   // we are assuming that the compiler has the feature "thread_local".
   // If builds break because of thread_local stuff, add another rule here.
   #define HAS_FEATURE_THREAD_LOCAL 1
#endif

