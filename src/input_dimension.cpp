
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_dimension.h"

#include <stdexcept>

using namespace panda;

std::size_t panda::input::implementation::dimension(std::istream& stream)
{
   while ( stream && stream.peek() != '=' )
   {
      stream.get();
   }
   stream.get(); // consume '='
   std::size_t dim;
   if ( !(stream >> dim) )
   {
      throw std::invalid_argument("Argument to field \"dimension\" is invalid.");
   }
   return dim;
}

