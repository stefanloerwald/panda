
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_JOB_MANAGER_PROXY
#include "job_manager_proxy.h"
#undef COMPILE_TEMPLATE_JOB_MANAGER_PROXY

#ifdef MPI_SUPPORT

using namespace panda;

template <typename Integer, typename TagType>
void panda::JobManagerProxy<Integer, TagType>::put(const Matrix<Integer>& container) const
{
   communication.toMaster(container);
}

template <typename Integer, typename TagType>
Row<Integer> panda::JobManagerProxy<Integer, TagType>::get() const
{
   return communication.fromMaster<Integer>();
}

#else

template <typename Integer, typename TagType>
void panda::JobManagerProxy<Integer, TagType>::put(const Matrix<Integer>&) const
{
}

template <typename Integer, typename TagType>
panda::Row<Integer> panda::JobManagerProxy<Integer, TagType>::get() const
{
   return panda::Row<Integer>{};
}

#endif

template <typename Integer, typename TagType>
panda::JobManagerProxy<Integer, TagType>::JobManagerProxy(const Names&, const int, const int)
:
   communication()
{
}

