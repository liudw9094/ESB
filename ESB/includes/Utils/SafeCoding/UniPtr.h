#ifndef _INCLUDES_UTILS_FACTORY_H_
#define _INCLUDES_UTILS_FACTORY_H_

#include <functional>

template<class _Creater, class _Intf>
class CUniPtr<_Creater _creater>
{
	auto p;
public:
	template<class... _Types>
	inline CUniPtr(_Types&&... _Args)
		: p(std::invoke(_creater, std::forward<_Types>(_Args)...))
	{
	}
	inline ~CUniPtr()
	{
		p->Dispose();
	}
	inline auto operator -> decltype(p)
	{	if (m_pPointRef != NULL)
		return m_pPointRef->m_pPointer;
		return NULL;					
	}
	inline auto operator -> const decltype(p)
	{	if (m_pPointRef != NULL)
		return m_pPointRef->m_pPointer;
		return NULL;	
	}
	inline auto operator*() -> decltype(p)&
	{									
		return *operator->();			
	}									
	inline auto operator*() -> const decltype(p)&
	{										
		return *operator->();				
	}										
};

#endif //_INCLUDES_UTILS_FACTORY_H_