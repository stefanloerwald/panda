
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "position_guarded_file.h"

#include <cassert>

using namespace panda;

panda::PositionGuardedFile::PositionGuardedFile(std::ifstream& file_)
:
   file(file_),
   initial_position(file.tellg())
{
}
panda::PositionGuardedFile::~PositionGuardedFile()
{
   file.clear();
   file.seekg(initial_position);
   assert( file.tellg() == initial_position );
}

