
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

template<typename Function>
panda::ScopeGuard<Function>::ScopeGuard(const Function& fn)
:
   dismissed(false),
   function(fn)
{
}

template<typename Function>
panda::ScopeGuard<Function>::ScopeGuard(Function&& fn)
:
   dismissed(false),
   function(std::move(fn))
{
}

template<typename Function>
panda::ScopeGuard<Function>::ScopeGuard(ScopeGuard&& other)
:
   dismissed(other.dismissed),
   function(std::move(other.function))
{
   other.dismissed = true;
}

template<typename Function>
panda::ScopeGuard<Function>::~ScopeGuard() noexcept
{
   if ( !dismissed )
   {
      function();
   }
}

template<typename Function>
void panda::ScopeGuard<Function>::dismiss() noexcept
{
   dismissed = true;
}

template <typename Function, typename DecayType>
panda::ScopeGuard<DecayType> panda::makeScopeGuard(Function&& function)
{
   return ScopeGuard<DecayType>(std::forward<Function>(function));
}

