#include "stdafx.h"
#include "UtilsRuntime.h"

#include <Utils/SafeCoding/SmtPtr.h>
#include <Utils/Thread.h>


using namespace Utils::Thread;

namespace Utils
{
	namespace SafeCoding
	{
		class CSmtPtrImp
		{
		private:
			struct POINTREF
			{
			public:
				void				*m_pPointer;
				volatile DWORD		m_dwRefCount;
				ICriticalSection	*m_plkRefCountOp;
				volatile DWORD		m_dwWeakRefCount;
				ICriticalSection	*m_plkPointer;
				void				(*m_freer)(void*);

				POINTREF(void* ptr, void(*freer)(void*)) : m_pPointer(ptr),
					m_dwRefCount(1),
					m_plkRefCountOp(CreateCriticalSection()),
					m_dwWeakRefCount(0),
					m_plkPointer(CreateCriticalSection()),
					m_freer(freer)
				{
				}
				~POINTREF()
				{
					m_plkPointer->Lock();
					if(m_pPointer)
						m_freer(m_pPointer);
					m_plkPointer->Unlock();
					m_plkPointer->Dispose();
					m_plkRefCountOp->Dispose();
				}
			};
			volatile POINTREF *m_pPtrRef;
		public:
			CSmtPtrImp(void *ptr, void(*freer)(void*));
			CSmtPtrImp(CSmtPtrImp &src);
			~CSmtPtrImp();
			void AddRef();
			void ReleaseRef();
			DWORD GetStrongRefCount() const;
			DWORD GetWeakRefCount() const;
			void* GetPointer();
			void*& GetPointerRef();
			void ResetPointer(void *ptr, void(*freer)(void*));
			void ResetImp(const CSmtPtrImp* src);
		};
	};
};

using namespace Utils::SafeCoding;

// CSmtPtrImpShell

CSmtPtrImpShell::CSmtPtrImpShell(void *ptr, void(*freer)(void*))
{
	::InterlockedExchangePointer((volatile PVOID*)&pImp, new CSmtPtrImp(ptr, freer));
	if (pImp == NULL)
	{
		// TODO: throw
	}
}

CSmtPtrImpShell::CSmtPtrImpShell(const CSmtPtrImpShell& src)
{
	::InterlockedExchangePointer((volatile PVOID*)&pImp, new CSmtPtrImp(*const_cast<CSmtPtrImp*>(src.pImp)));
	if (pImp == NULL)
	{
		// TODO: throw
	}
}

CSmtPtrImpShell::~CSmtPtrImpShell()
{
	delete pImp;
}

void CSmtPtrImpShell::AddRef()
{
	const_cast<CSmtPtrImp*>(pImp)->AddRef();
}

void CSmtPtrImpShell::ReleaseRef()
{
	const_cast<CSmtPtrImp*>(pImp)->ReleaseRef();
}

DWORD CSmtPtrImpShell::GetStrongRefCount() const
{
	return const_cast<CSmtPtrImp*>(pImp)->GetStrongRefCount();
}

DWORD CSmtPtrImpShell::GetWeakRefCount() const
{
	return const_cast<CSmtPtrImp*>(pImp)->GetWeakRefCount();
}

void* CSmtPtrImpShell::GetPointer()
{
	return const_cast<CSmtPtrImp*>(pImp)->GetPointer();
}

void*& CSmtPtrImpShell::GetPointerRef()
{
	return const_cast<CSmtPtrImp*>(pImp)->GetPointerRef();
}

void CSmtPtrImpShell::ResetPointer(void *ptr, void(*freer)(void*))
{
	const_cast<CSmtPtrImp*>(pImp)->ResetPointer(ptr, freer);
}

void CSmtPtrImpShell::ResetImp(const CSmtPtrImpShell& src)
{
	const_cast<CSmtPtrImp*>(pImp)->ResetImp(const_cast<CSmtPtrImp*>(src.pImp));
}
// CSmtPtrImp

CSmtPtrImp::CSmtPtrImp(void *ptr, void(*freer)(void*))
	: m_pPtrRef(NULL)
{
	//if (ptr == nullptr)
	//	return;
	ASSERT(freer != NULL);
	ResetPointer(ptr, freer);
}

CSmtPtrImp::CSmtPtrImp(CSmtPtrImp &src)
{
	src.AddRef();
	m_pPtrRef = src.m_pPtrRef;
	if (m_pPtrRef == NULL)
	{
		// TODO: throw
	}
}

CSmtPtrImp::~CSmtPtrImp()
{
	ReleaseRef();
}

void CSmtPtrImp::AddRef()
{
	if (m_pPtrRef)
	{
		SLOCK(m_pPtrRef->m_plkRefCountOp);
		if(m_pPtrRef->m_dwRefCount != 0)
			::InterlockedIncrement(&m_pPtrRef->m_dwRefCount);
	}
}

void CSmtPtrImp::ReleaseRef()
{
	if (m_pPtrRef)
	{
		m_pPtrRef->m_plkRefCountOp->Lock();
		if (::InterlockedDecrement(&m_pPtrRef->m_dwRefCount) == 0)
		{
			m_pPtrRef->m_plkRefCountOp->Unlock();
			delete m_pPtrRef;
			m_pPtrRef = NULL;
			return;
		}
		m_pPtrRef->m_plkRefCountOp->Unlock();
	}
}

DWORD CSmtPtrImp::GetStrongRefCount() const
{
	if (m_pPtrRef)
		return m_pPtrRef->m_dwRefCount;
	else
		return -1;
}

DWORD CSmtPtrImp::GetWeakRefCount() const
{
	if (m_pPtrRef)
		return m_pPtrRef->m_dwWeakRefCount;
	else
		return -1;
}

void* CSmtPtrImp::GetPointer()
{
	if (m_pPtrRef)
		return m_pPtrRef->m_pPointer;
	else
		return NULL;
}

void*& CSmtPtrImp::GetPointerRef()
{
	return const_cast<POINTREF*>(m_pPtrRef)->m_pPointer;
}

void CSmtPtrImp::ResetPointer(void *ptr, void(*freer)(void*))
{
	ReleaseRef();
	if(ptr)
	{
		::InterlockedExchangePointer((volatile PVOID*)&m_pPtrRef, new POINTREF(ptr, freer));
		if (m_pPtrRef == NULL)
		{
			// TODO: throw
		}
	}
}

void CSmtPtrImp::ResetImp(const CSmtPtrImp* src)
{
	if (m_pPtrRef != NULL &&
		src->m_pPtrRef->m_pPointer == m_pPtrRef->m_pPointer) //???????
	{
		// TODO: throw
		return;
	}
	ReleaseRef();
	::InterlockedExchangePointer((volatile PVOID*)&m_pPtrRef, const_cast<POINTREF*>(src->m_pPtrRef));
	AddRef();
}