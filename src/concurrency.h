
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   namespace concurrency
   {
      /// Returns the number of threads to be used in parallelized operations.
      int numberOfThreads(int, char**);
   }
}

