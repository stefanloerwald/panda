
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <fstream>

namespace panda
{
   class PositionGuardedFile
   {
      public:
         /// Constructor.
         PositionGuardedFile(std::ifstream&);
         /// Destructor. Restoring the original position.
         ~PositionGuardedFile();
      public:
         std::ifstream& file;
      private:
         using FilePosition = decltype(std::declval<std::ifstream>().tellg());
         FilePosition initial_position;
   };
}

