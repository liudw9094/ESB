/////////////////////////////////////////////////////
//
//
//				SafeAllocator.h
//
//	This is an implement of smart-pointer.
//
/////////////////////////////////////////////////////

#ifndef _INCLUDES_UTILS_SMTPTR_H_
#define _INCLUDES_UTILS_SMTPTR_H_

#ifndef UTILSRUNTIME_API
#define UTILSRUNTIME_API
#endif // UTILSRUNTIME_API

//#include "../../Thread/CriticalSection.h"
										

#ifndef ASSERT
#define ASSERT(X) if(!X) throw;
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

namespace Utils
{
	namespace SafeCoding
	{
		template<class T>
		inline void SafeDelete(T* &p)
		{
			if (p == NULL)
				return;
			delete p;
			p = NULL;
		}

		template<class T>
		inline void SafeDeleteArray(T* &p)
		{
			if (p == NULL)
				return;
			delete[] p;
			p = NULL;
		}

		template<class T>
		inline void SafeRelease(T* &p)
		{
			if (p == NULL)
				return;
			p->Release();
			p = NULL;
		}

		template<class T>
		inline void SafeDispose(T* &p)
		{
			if (p == NULL)
				return;
			p->Dispose();
			p = NULL;
		}

		class CSmtPtrImp;
		class UTILSRUNTIME_API CSmtPtrImpShell
		{
		private:
			CSmtPtrImp *pImp;
		public:
			CSmtPtrImpShell(void *ptr, void(*freer)(void*));
			CSmtPtrImpShell(const CSmtPtrImpShell& src);
			~CSmtPtrImpShell();
			void AddRef();
			void ReleaseRef();
			DWORD GetStrongRefCount() const;
			DWORD GetWeakRefCount() const;
			void* GetPointer();
			void*& GetPointerRef();
			void ResetPointer(void *ptr, void(*freer)(void*));
			void ResetImp(const CSmtPtrImpShell& src);
		};



		template<class T>
		void _Disposer(T *p)
		{
			IDisposable *vp = static_cast<IDisposable*>(p);
			vp->Dispose();
		}

		template<class T, void freer(void*) = reinterpret_cast<void (*)(void*)>(_Disposer<T>)>
		class CSmtPtr
		{
		protected:
			CSmtPtrImpShell m_smPtr;
		public:
			inline CSmtPtr(T *ptr = NULL) : m_smPtr(ptr, freer)
			{
			}
			inline CSmtPtr(const CSmtPtr<T, freer> &scr) : m_smPtr(scr.m_smPtr)
			{
			}
			inline ~CSmtPtr()
			{
			}

			inline void AddRef()
			{
				m_smPtr.AddRef();
			}

			inline void ReleaseRef()
			{
				m_smPtr.ReleaseRef();
			}

			inline DWORD GetStrongRefCount() const
			{
				return m_smPtr.GetStrongRefCount();
			}

			inline DWORD GetWeakRefCount() const
			{
				return m_smPtr.GetWeakRefCount();
			}

			inline CSmtPtr<T, freer>& operator =(T *pPointer)
			{
				m_smPtr.ResetPointer(pPointer, freer);
				return *this;
			}

			inline CSmtPtr<T, freer>& operator =(const CSmtPtr<T, freer>& src)
			{
				m_smPtr.ResetImp(src.m_smPtr);
				return *this;
			}

			inline T* operator->()
			{
				return reinterpret_cast<T*>(m_smPtr.GetPointer());
			}

			inline const T* operator->() const
			{
				return  reinterpret_cast<T*>(const_cast<CSmtPtrImpShell*>(&m_smPtr)->GetPointer());
			}
			
			inline T& operator*()
			{
				return *operator->();
			}

			inline const T& operator*() const
			{
				return *operator->();
			}

			inline operator T*()
			{
				return operator->();
			}

			inline operator const T*() const
			{
				return operator->();
			}

			inline T** operator &()
			{
				return &m_smPtr.GetPointerRef();
			}

		protected:
		private:
			inline void _ClearRefCountPointers()
			{
				if (m_pPointRef != NULL)
					SafeDelete(m_pPointRef);
			}

		public:

			//class TWeakPointer
			//{
			//	mutable POINTREF *m_pPointRef;
			//public:
			//	inline TWeakPointer(const TSafePointer<T, freer> &p) : m_pPointRef(p.m_pPointRef)
			//	{
			//		if (m_pPointRef == NULL)
			//			throw;

			//		{
			//			SAFELOCK(m_pPointRef->m_lkWeakRefCount);
			//			++(m_pPointRef->m_dwWeakRefCount);
			//		}
			//	}


			//	inline TWeakPointer(const TWeakPointer &p) : m_pPointRef(p.m_pPointRef)
			//	{
			//		if (m_pPointRef == NULL)
			//			throw;

			//		{
			//			SAFELOCK(m_pPointRef->m_lkWeakRefCount);
			//			++(m_pPointRef->m_dwWeakRefCount);
			//		}
			//	}

			//	inline ~TWeakPointer()
			//	{
			//		_ReleaseRef();
			//	}

			//	inline BOOL IsValid()
			//	{
			//		if (m_pPointRef == NULL)
			//			return FALSE;
			//		if (m_pPointRef->m_pPointer == NULL)
			//			return FALSE;
			//		return m_pPointRef->m_pdwRefCount > 0;
			//	}

			//	inline DWORD GetStrongRefCount() const { return m_pPointRef->m_dwRefCount; };
			//	inline DWORD GetWeakRefCount() const { return m_pPointRef->m_dwWeakRefCount; };
			//	inline DWORD GetAllRefCount() const { return GetStrongRefCount() + GetWeakRefCount(); };

			//	___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___

			//		inline TWeakPointer& operator =(const TWeakPointer& scr)
			//	{
			//		if (scr.m_pPointRef == NULL)
			//			throw;
			//		if (m_pPointRef != NULL &&
			//			scr.m_pPointRef->m_pPointer == m_pPointRef->m_pPointer) //避免循环引用
			//			throw;

			//		_ReleaseRef();

			//		m_pPointRef = scr.m_pPointRef;

			//		{
			//			SAFELOCK(m_pPointRef->m_lkWeakRefCount);
			//			++(m_pPointRef->m_dwWeakRefCount);
			//		}
			//		return *this;
			//	}
			//	inline TWeakPointer& operator =(const TSafePointer<T, freer>& scr)
			//	{
			//		if (scr.m_pPointRef == NULL)
			//			throw;
			//		if (m_pPointRef != NULL &&
			//			scr.m_pPointRef->m_pPointer == m_pPointRef->m_pPointer) //避免循环引用
			//			throw;

			//		_ReleaseRef();

			//		m_pPointRef = scr.m_pPointRef;

			//		{
			//			SAFELOCK(m_pPointRef->m_lkWeakRefCount);
			//			++(m_pPointRef->m_dwWeakRefCount);
			//		}

			//		return *this;
			//	}
			//private:
			//	inline void _ClearRefCountPointers()
			//	{
			//		if (m_pPointRef != NULL)
			//		{
			//			m_pPointRef->m_lkPointer.Lock();
			//			SafeDelete(m_pPointRef);
			//		}
			//	}

			//	void _ReleaseRef()
			//	{
			//		if (m_pPointRef == NULL)
			//			return;

			//		m_pPointRef->m_lkRefCount.Lock();
			//		m_pPointRef->m_lkWeakRefCount.Lock();

			//		if (m_pPointRef->m_dwWeakRefCount > 0)
			//			--m_pPointRef->m_dwWeakRefCount;

			//		if (GetAllRefCount() == 0)
			//		{
			//			_ClearRefCountPointers();
			//		}
			//		else
			//		{
			//			m_pPointRef->m_lkWeakRefCount.Unlock();
			//			m_pPointRef->m_lkRefCount.Unlock();
			//		}
			//		m_pPointRef = NULL;
			//	}
			//	inline void operator &() {};
			//};
		};
	};
};

#define SREF_TAG(CLASS_NAME, RELEASE_FUNC) ::Utils::SafeCoding::CSmtPtr<CLASS_NAME, RELEASE_FUNC>

#define SREF(CLASS_NAME) ::Utils::SafeCoding::CSmtPtr<CLASS_NAME>


#endif //_INCLUDES_UTILS_SMTPTR_H_


