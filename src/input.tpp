
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "cast.h"

template <typename Integer>
std::tuple<panda::Vertices<Integer>, panda::Names, panda::Maps, panda::Inequalities<Integer>> panda::input::vertices(int argc, char** argv)
{
   const auto data = vertices<int>(argc, argv);
   return std::make_tuple(cast<Integer>(std::get<0>(data)),  // vertices
                          std::get<1>(data),                 // names
                          std::get<2>(data),                 // maps
                          cast<Integer>(std::get<3>(data))); // inequalities
}

template <typename Integer>
std::tuple<panda::Inequalities<Integer>, panda::Names, panda::Maps, panda::Vertices<Integer>> panda::input::inequalities(int argc, char** argv)
{
   const auto data = inequalities<int>(argc, argv);
   return std::make_tuple(cast<Integer>(std::get<0>(data)),  // inequalities
                          std::get<1>(data),                 // names
                          std::get<2>(data),                 // maps
                          cast<Integer>(std::get<3>(data))); // vertices
}

