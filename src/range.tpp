
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

template <typename Container>
panda::ReverseRange<Container>::ReverseRange(Container& container)
:
   container_ref(container)
{
}

template <typename Container>
typename panda::ReverseRange<Container>::BeginType panda::ReverseRange<Container>::begin()
{
   return container_ref.rbegin();
}

template <typename Container>
typename panda::ReverseRange<Container>::EndType panda::ReverseRange<Container>::end()
{
   return container_ref.rend();
}

template <typename Container>
panda::ReverseRange<Container> panda::makeReverseRange(Container&& container)
{
   return ReverseRange<Container>(std::forward<Container>(container));
}

