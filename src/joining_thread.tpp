
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

template <typename Function, typename... Args>
panda::JoiningThread::JoiningThread(Function&& f, Args&&... args)
:
   thread(std::forward<Function>(f), std::forward<Args>(args)...)
{
}

