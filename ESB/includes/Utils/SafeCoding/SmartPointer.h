//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//				SafeAllocator.h
//
//	This is an implement of smart-pointer and smart-interface.
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ___JYFR_JYM_SAFEALLOCATOR_H___
#define ___JYFR_JYM_SAFEALLOCATOR_H___


#include <SafeLock.h>

#define ___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___	\
		inline T* operator->()						\
		{											\
			if(m_pPointRef != NULL)					\
				return m_pPointRef->m_pPointer;		\
			return NULL;							\
		}											\
		inline const T* operator->() const			\
		{	if(m_pPointRef != NULL)					\
				return m_pPointRef->m_pPointer;		\
			return NULL;							\
		}											\
													\
		inline T& operator*()						\
		{											\
			return *operator->();					\
		}											\
		inline const T& operator*() const			\
		{											\
			return *operator->();					\
		}											\
													\
		inline operator T*()						\
		{											\
			return operator->();					\
		}											\
		inline operator const T*() const			\
		{											\
			return operator->();					\
		}												

#ifndef ASSERT
#define ASSERT(X) if(!X) throw;
#endif


namespace SafeAllocator
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

	// 用于释放用“new”方法申请的单个对象
	template<class T> inline void __NormalFree(T* p)
	{
		delete p;
	}

	// 智能指针 - 强引用指针，可直接用于单个“new”对象的智能指针
	//
	// 作用：		指针在不同模块多次引用的复杂情况下，确保内存不发生泄漏，无效指针的访问
	//
	// 用法：		TSafePointer<type> smartPointer(new type);			//生成一个对象并为其配备强引用
	//				TSafePointer<type> smartPointer1(smartPointer);		//在已有的智能指针上添加个强引用
	//				TSafePointer<type> smartPointer2 = smartPointer;	//在已有的智能指针上添加个强引用
	//				TSafePointer<type> smartPointer3 = smartPointer1;	//在已有的智能指针上添加个强引用
	//				smartPointer = smartPointerX;						// 释放当前强引用，在另一个已有的智能指针上添加一个强引用

	template<class T, void freer(T*) = __NormalFree>
	class TSafePointer
	{
	protected:
		struct POINTREF
		{
		public:
			IDisposable* const	m_pPointer;
			volatile DWORD		m_dwRefCount;
			volatile DWORD		m_dwWeakRefCount;
			CCriticalSection	m_lkPointer;

			POINTREF() : m_pPointer(NULL),
				m_dwRefCount(0),
				m_dwWeakRefCount(0)
			{
			}
			~POINTREF()
			{
				SAFELOCK(m_lkPointer);
				if (m_pPointer != NULL)
					m_pPointer->Dispose();
			}
		};
	protected:
		mutable POINTREF		*m_pPointRef;
	public:
		inline TSafePointer(T *pPointer = NULL) : m_pPointRef(NULL)
		{
			// TODO: Runtime check
			if (pPointer == NULL)
				return;

			if ((m_pPointRef = new POINTREF) == NULL)
				throw;

			{
				SAFELOCK(m_pPointRef->m_lkPointer);
				m_pPointRef->m_pPointer = pPointer;
				++(m_pPointRef->m_dwRefCount);
			}
		}
		inline TSafePointer(const TSafePointer<T, freer> &scr) : m_pPointRef(scr.m_pPointRef)
		{
			if (scr.m_pPointRef == NULL)
				return;

			m_pPointRef = scr.m_pPointRef;
			{
				SAFELOCK(m_pPointRef->m_lkRefCount);
				++(m_pPointRef->m_dwRefCount);
			}
		}
		inline ~TSafePointer()
		{
			_ReleaseRef();
		}


		inline DWORD GetStrongRefCount() const
		{
			if (m_pPointRef != NULL)
				return m_pPointRef->m_dwRefCount;
			else
				return 0;
		};
		inline DWORD GetWeakRefCount() const
		{
			if (m_pPointRef != NULL)
				return m_pPointRef->m_dwWeakRefCount;
			else
				return 0;
		}
		inline DWORD GetAllRefCount() const { return GetStrongRefCount() + GetWeakRefCount(); };

		inline TSafePointer<T, freer>& operator =(T *pPointer) // 释放当前引用数并建立新的强引用
		{
			if (m_pPointRef != NULL &&
				pPointer == m_pPointRef->m_pPointer) //避免循环引用
				throw;

			// TODO: 添加运行时指针检测


			_ReleaseRef();

			if (pPointer == NULL)
				return *this;

			if ((m_pPointRef = new POINTREF) == NULL)
				throw;

			{
				SAFELOCK(m_pPointRef->m_lkPointer);
				m_pPointRef->m_pPointer = pPointer;
			}
			{
				SAFELOCK(m_pPointRef->m_lkRefCount);
				++(m_pPointRef->m_dwRefCount);
			}
			return *this;
		}

		inline TSafePointer<T, freer>& operator =(const TSafePointer<T, freer>& scr)
		{

			if (m_pPointRef != NULL &&
				scr.m_pPointRef->m_pPointer == m_pPointRef->m_pPointer) //避免循环引用
				throw;

			_ReleaseRef();

			if (scr.m_pPointRef == NULL)
				return *this;

			m_pPointRef = scr.m_pPointRef;

			if (m_pPointRef != NULL)
			{
				SAFELOCK(m_pPointRef->m_lkRefCount);
				++(m_pPointRef->m_dwRefCount);
			}
			return *this;
		}

		___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___;

		inline T** operator &()
		{
			if (m_pPointRef == NULL)
				return NULL;
			return &m_pPointRef->m_pPointer;
		};
	protected:
	private:
		inline void _ClearRefCountPointers()
		{
			if (m_pPointRef != NULL)
				SafeDelete(m_pPointRef);
		}

		inline void _ReleaseRef()
		{
			if (m_pPointRef == NULL)
				return;

			m_pPointRef->m_lkRefCount.Lock();

			if (m_pPointRef->m_dwRefCount > 0)
				--(m_pPointRef->m_dwRefCount);

			if (m_pPointRef->m_dwRefCount == 0)
			{
				{
					SAFELOCK(m_pPointRef->m_lkPointer);  // 锁定指针
					if (m_pPointRef->m_pPointer != NULL)
					{
						freer(m_pPointRef->m_pPointer);	//释放指针
						m_pPointRef->m_pPointer = NULL;
					}
				}

				m_pPointRef->m_lkWeakRefCount.Lock(); //锁定全局weak引用
				if (m_pPointRef->m_dwWeakRefCount == 0)
				{
					_ClearRefCountPointers();
				}

				if (m_pPointRef != NULL)
					m_pPointRef->m_lkRefCount.Unlock();
			}
			if (m_pPointRef != NULL)
				m_pPointRef->m_lkRefCount.Unlock();

			m_pPointRef = NULL;
		}



	public:
		class TWeakPointer
		{
			mutable POINTREF *m_pPointRef;
		public:
			inline TWeakPointer(const TSafePointer<T, freer> &p) : m_pPointRef(p.m_pPointRef)
			{
				if (m_pPointRef == NULL)
					throw;

				{
					SAFELOCK(m_pPointRef->m_lkWeakRefCount);
					++(m_pPointRef->m_dwWeakRefCount);
				}
			}


			inline TWeakPointer(const TWeakPointer &p) : m_pPointRef(p.m_pPointRef)
			{
				if (m_pPointRef == NULL)
					throw;

				{
					SAFELOCK(m_pPointRef->m_lkWeakRefCount);
					++(m_pPointRef->m_dwWeakRefCount);
				}
			}

			inline ~TWeakPointer()
			{
				_ReleaseRef();
			}

			inline BOOL IsValid()
			{
				if (m_pPointRef == NULL)
					return FALSE;
				if (m_pPointRef->m_pPointer == NULL)
					return FALSE;
				return m_pPointRef->m_pdwRefCount > 0;
			}

			inline DWORD GetStrongRefCount() const { return m_pPointRef->m_dwRefCount; };
			inline DWORD GetWeakRefCount() const { return m_pPointRef->m_dwWeakRefCount; };
			inline DWORD GetAllRefCount() const { return GetStrongRefCount() + GetWeakRefCount(); };

			___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___

				inline TWeakPointer& operator =(const TWeakPointer& scr)
			{
				if (scr.m_pPointRef == NULL)
					throw;
				if (m_pPointRef != NULL &&
					scr.m_pPointRef->m_pPointer == m_pPointRef->m_pPointer) //避免循环引用
					throw;

				_ReleaseRef();

				m_pPointRef = scr.m_pPointRef;

				{
					SAFELOCK(m_pPointRef->m_lkWeakRefCount);
					++(m_pPointRef->m_dwWeakRefCount);
				}
				return *this;
			}
			inline TWeakPointer& operator =(const TSafePointer<T, freer>& scr)
			{
				if (scr.m_pPointRef == NULL)
					throw;
				if (m_pPointRef != NULL &&
					scr.m_pPointRef->m_pPointer == m_pPointRef->m_pPointer) //避免循环引用
					throw;

				_ReleaseRef();

				m_pPointRef = scr.m_pPointRef;

				{
					SAFELOCK(m_pPointRef->m_lkWeakRefCount);
					++(m_pPointRef->m_dwWeakRefCount);
				}

				return *this;
			}
		private:
			inline void _ClearRefCountPointers()
			{
				if (m_pPointRef != NULL)
				{
					m_pPointRef->m_lkPointer.Lock();
					SafeDelete(m_pPointRef);
				}
			}

			void _ReleaseRef()
			{
				if (m_pPointRef == NULL)
					return;

				m_pPointRef->m_lkRefCount.Lock();
				m_pPointRef->m_lkWeakRefCount.Lock();

				if (m_pPointRef->m_dwWeakRefCount > 0)
					--m_pPointRef->m_dwWeakRefCount;

				if (GetAllRefCount() == 0)
				{
					_ClearRefCountPointers();
				}
				else
				{
					m_pPointRef->m_lkWeakRefCount.Unlock();
					m_pPointRef->m_lkRefCount.Unlock();
				}
				m_pPointRef = NULL;
			}
			inline void operator &() {}; // 不允许显示方式取地址
		};
	};


	template<class T> inline void __FreeArray(T* p)
	{
		delete[] p;
	}

	template<class T>
	class TSafeArrayPointer : public TSafePointer<T, __FreeArray>
	{
	public:
		inline TSafeArrayPointer(T *p = NULL) : TSafePointer(p)
		{
		}
		inline TSafeArrayPointer(const TSafeArrayPointer<T> &scr) : TSafePointer(scr)
		{
		}

		___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___;

		inline T** operator &()
		{
			return TSafePointer<T, __FreeArray>::operator &();
		};

		inline TSafeArrayPointer<T>& operator =(const TSafeArrayPointer<T>&scr) { return (TSafeArrayPointer<T>&)TSafePointer<T, __FreeArray>::operator =(scr); };

		using TSafePointer<T, __FreeArray>::TWeakPointer;
	private:
		//inline void operator =(TSafePointer<T, __FreeArray>&){};
	};


	template<class T> inline void __SafeInterfaceFree(T* p)
	{
		p->Release();
	}

	template<class T>
	class TSafeInterface : public TSafePointer<T, __SafeInterfaceFree>
	{
	public:
		inline TSafeInterface(T* pointer = NULL) : TSafePointer(pointer)
		{
		}
		inline TSafeInterface(const TSafeInterface<T> &scr) : TSafePointer(scr)
		{
		}

		___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___;

		inline T** operator &()
		{
			return TSafePointer<T, __SafeInterfaceFree>::operator &();
		};

		inline TSafeInterface<T>& operator =(const TSafeInterface<T>&scr)
		{
			TSafePointer<T, __SafeInterfaceFree>::operator =(scr);
			return *this;
		};
		inline TSafeInterface<T>& operator =(T *scr)
		{
			TSafePointer<T, __SafeInterfaceFree>::operator =(scr);
			return *this;
		};

		using TSafePointer<T, __SafeInterfaceFree>::TWeakPointer;
	private:
		inline void operator =(TSafePointer<T, __SafeInterfaceFree>&) {};
	};


	template<class T, T* allocator()>
	class TSafeInterfaceShell : public TSafeInterface<T>
	{
	public:
		inline TSafeInterfaceShell() : TSafeInterface(allocator())
		{
		}
		inline TSafeInterfaceShell(const TSafeInterfaceShell<T, allocator> &scr) : TSafeInterface(scr)
		{
		}

		___JYFR_JYM_SAFEALLOCATOR_H___OPERATORS___;

		inline T** operator &()
		{
			return TSafePointer<T, __SafeInterfaceFree>::operator &();
		};

		inline TSafeInterfaceShell<T, allocator>& operator =(const TSafeInterfaceShell<T, allocator>&scr)
		{
			TSafeInterface<T>::operator =(scr);
			return *this;
		};
		inline TSafeInterfaceShell<T, allocator>& operator =(T *scr)
		{
			TSafeInterface<T>::operator =(scr);
			return *this;
		};

		using TSafePointer<T, __SafeInterfaceFree>::TWeakPointer;
	private:
		inline void operator =(TSafePointer<T, __SafeInterfaceFree>&) {};
	};


	interface IReleasable
	{
		virtual void Release() {};
	};
};

#define SREF_TAG(CLASS_NAME, RELEASE_FUNC) SafeAllocator::TSafePointer<CLASS_NAME, RELEASE_FUNC>
#define SWEAKREF_TAG(CLASS_NAME, RELEASE_FUNC) SafeAllocator::TSafePointer<CLASS_NAME, RELEASE_FUNC>::TWeakPointer

#define SREF(CLASS_NAME) SafeAllocator::TSafePointer<CLASS_NAME>
#define SREF_ARR(CLASS_NAME) SafeAllocator::TSafeArrayPointer<CLASS_NAME>
#define SREF_INT(CLASS_NAME) SafeAllocator::TSafeInterface<CLASS_NAME>

#endif //___JYFR_JYM_SAFEALLOCATOR_H___


