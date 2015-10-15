#ifndef _SAFELOCK_H__INCLUDE_
#define _SAFELOCK_H__INCLUDE_

#include "SectionLock.h"

namespace SafeLock
{
#ifdef WIN32
	//比较后赋值，若*Destination == Comperand，则Destination = value
	inline long __stdcall IfEquelSetValue32(long volatile*Destination,long value,long Comperand)
	{
	   __asm
	   {
			  mov     ecx, Destination;
			  mov     edx, value;
			  mov     eax, Comperand;
			  lock cmpxchg [ecx], edx; //if [ecx] == eax, then [ecx] == edx, else eax = [ecx]
		}
	}

	//赋值
	inline long __stdcall SetValue32(long volatile* Target,long Value)
	{
	   __asm
	   {
		  mov      ecx, Target;
		  mov      edx, Value;
	label:
		  lock cmpxchg [ecx], edx; //if [ecx] == eax, then [ecx] == edx, else eax = [ecx]
		  jnz      short label;
	   }
	}

	//比较自加
	inline long __stdcall NotEquelThenIncrease32(long volatile* Target,long Value)
	{
	   __asm
	   {
		  mov      ecx, Target;
		  mov      edx, Value;
		  mov		eax, 1;
		  lock cmp	[ecx], edx;
		  lock jz	lable;
		  lock xadd [ecx], eax; //if [ecx] == eax, then [ecx] == edx, else eax = [ecx]
lable:
	   }
	}

	//自减
	inline long __stdcall Decrement32(long volatile* Addend)
	{
	   __asm
	   {
		  mov     ecx, Addend;
		  mov     eax, 0FFFFFFFFh;//-1
		  lock xadd [ecx], eax; //加-1
		  dec     eax;
	   }
	}

	//自增
	inline long __stdcall Increment32(long volatile* Addend)
	{
	   __asm
	   {
		  mov      ecx, Addend;
		  mov      eax, 1;
		  lock xadd [ecx], eax; //加
		  inc      eax;
	   }
	}

	//相加
	inline long __stdcall Add32(long volatile* Addend,long Value)
	{
	   __asm
	   {
		  mov      ecx, Addend;
		  mov      eax, Value;
		  lock xadd [ecx], eax;
	   }
	}
	//相加
	inline long __stdcall Add32(long volatile* Addend,long volatile* Value)
	{
	   __asm
	   {
		  mov		ecx, Addend;
		  lock mov	eax, [Value];
		  lock xadd [ecx], eax;
	   }
	}
#endif WIN32



#ifdef WIN32
#define IfEquelSetIntValue	IfEquelSetValue32
#define SetIntValue			SetValue32
#endif

	typedef struct _tagVLong
	{
		long volatile value;
		_tagVLong(){}
		_tagVLong(long v)  : value(v){}
		operator long volatile&()
		{
			return value;
		}
		operator unsigned long volatile&()
		{
			return (unsigned long volatile&)value;
		}
	} VLONG;
	typedef struct _tagVULong
	{
		unsigned long volatile value;
		_tagVULong(){}
		_tagVULong(unsigned long v) : value(v){}
	} VULONG;

	inline VLONG& operator ++(VLONG& ref)
	{
		Increment32(&ref.value);
		return ref;
	}
	inline VLONG& operator --(VLONG& ref)
	{
		Decrement32(&ref.value);
		return ref;
	}
	inline VLONG& operator +(VLONG& ref, const long n)
	{
		Add32(&ref.value, n);
		return ref;
	}
	inline VLONG& operator -(VLONG& ref, const long n)
	{
		Add32(&ref.value, -n);
		return ref;
	}
};

#endif //_SAFELOCK_H__INCLUDE_
