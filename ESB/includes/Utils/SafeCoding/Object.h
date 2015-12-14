#ifndef _INCLUDE_UTILS_SAFECODING_OBJECT_H_
#define _INCLUDE_UTILS_SAFECODING_OBJECT_H_

//#define OFFSETOF(type, member)	(size_t)&(((type *)0)->member)

//#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - OFFSETOF(type,member) );})

template<class P, class M>
size_t offsetof(const M P::*member)
{
	return (size_t)&(reinterpret_cast<P*>(0)->*member);
}

template<class P, class M>
P* container_of(M* ptr, const M P::*member)
{
	return (P*)((char*)ptr - offsetof(member));
}

#endif // _INCLUDE_UTILS_SAFECODING_OBJECT_H_