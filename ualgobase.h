// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// ualgobase.h
//
// Implementation of STL algorithms.
//
// The function prototypes are copied
// exactly from the SGI version of STL documentation along with comments about
// their use. The code is NOT the same, though the functionality usually is.
//

#ifndef UALGOBASE_H_683A0BE77546133C4CE0E3622CFAA2EB
#define UALGOBASE_H_683A0BE77546133C4CE0E3622CFAA2EB

#include "uutility.h"
#include <string.h>

namespace ustl {

/// Assigns the contents of a to b and the contents of b to a.
/// This is used as a primitive operation by many other algorithms. 
/// \ingroup SwapAlgorithms
///
template <typename Assignable> 
inline void swap (Assignable& a, Assignable& b)
{
    Assignable tmp = a;
    a = b;
    b = tmp;
}

/// Equivalent to swap (*a, *b)
/// \ingroup SwapAlgorithms
///
template <typename Iterator> 
inline void iterator_swap (Iterator a, Iterator b)
{
    swap (*a, *b);
}

/// Copy copies elements from the range [first, last) to the range
/// [result, result + (last - first)). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
/// for every integer n from 0 to last - first, copy performs the assignment
/// *(result + n) = *(first + n). Assignments are performed in forward order,
/// i.e. in order of increasing n. 
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++result, ++first)
	*result = *first;
    return (result);
}

/// Copy_n copies elements from the range [first, first + n) to the range
/// [result, result + n). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
/// for every integer i from 0 up to (but not including) n, copy_n performs
/// the assignment *(result + i) = *(first + i). Assignments are performed
/// in forward order, i.e. in order of increasing n.
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy_n (InputIterator first, size_t count, OutputIterator result)
{
    for (; count; --count, ++result, ++first)
	*result = *first;
    return (result);
}

/// \brief Copy copies elements from the range (last, first] to result.
/// \ingroup MutatingAlgorithms
/// Copies elements starting at last, decrementing both last and result.
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy_backward (InputIterator first, InputIterator last, OutputIterator result)
{
    while (first != last)
	*--result = *--last;
    return (result);
}

/// For_each applies the function object f to each element in the range
/// [first, last); f's return value, if any, is ignored. Applications are
/// performed in forward order, i.e. from first to last. For_each returns
/// the function object after it has been applied to each element.
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename UnaryFunction>
inline UnaryFunction for_each (InputIterator first, InputIterator last, UnaryFunction f)
{
    for (; first != last; ++first)
	f (*first);
    return (f);
}

/// Fill assigns the value value to every element in the range [first, last).
/// That is, for every iterator i in [first, last),
/// it performs the assignment *i = value.
/// \ingroup GeneratorAlgorithms
///
template <typename ForwardIterator, typename T>
inline void fill (ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first)
	*first = value;
}

/// Fill_n assigns the value value to every element in the range
/// [first, first+count). That is, for every iterator i in [first, first+count),
/// it performs the assignment *i = value. The return value is first + count.
/// \ingroup GeneratorAlgorithms
///
template <typename OutputIterator, typename T>
inline OutputIterator fill_n (OutputIterator first, size_t count, const T& value)
{
    for (; count; --count, ++first)
	*first = value;
    return (first);
}

#if CPU_HAS_MMX
extern "C" void copy_n_fast (const void* src, size_t count, void* dest);
#else
inline void copy_n_fast (const void* src, size_t count, void* dest)
    { memcpy (dest, src, count); }
#endif
#if __i386__
extern "C" void copy_n_backward_fast (const void* first, const void* last, void* result);
#else
inline void copy_n_backward_fast (const void* first, const void* last, void* result)
{
    const size_t nBytes (distance (first, last));
    memmove (advance (dest, -nBytes), first, nBytes);
}
#endif

//----------------------------------------------------------------------
// Optimized versions for standard types
//----------------------------------------------------------------------

#if WANT_UNROLLED_COPY

#if CPU_HAS_MMX
    #define CARRIER_SIZE	32
    #if CPU_HAS_3DNOW
	#define MMX_RESET	"femms"
	#define MMX_PREFETCH	"prefetch 512(%%esi)		\n\t"
	#define MMX_PREFETCHW	"prefetchw 512(%%edi)		\n\t"
    #else
	#define MMX_RESET	"emms"
	#define MMX_PREFETCH
	#define MMX_PREFETCHW
    #endif
    #if CPU_HAS_SSE
	#define UNROLLED_FILL_MMX_STORE		\
	    "movntq %%mm0, (%%edi)  \n\t"	\
	    "movntq %%mm0, 8(%%edi) \n\t"	\
	    "movntq %%mm0, 16(%%edi)\n\t"	\
	    "movntq %%mm0, 24(%%edi)\n\t"
    #else
	#define UNROLLED_FILL_MMX_STORE		\
	    "movq %%mm0, (%%edi)  \n\t"		\
	    "movq %%mm0, 8(%%edi) \n\t"		\
	    "movq %%mm0, 16(%%edi)\n\t"		\
	    "movq %%mm0, 24(%%edi)\n\t"
    #endif
    #define UNROLLED_FILL_LOOP(src, dest, count)\
	const size_t nMMTs = 8 / sizeof(T);	\
	T vv [nMMTs];				\
	vv[0] = src;				\
	asm volatile ("movq (%0), %%mm0" : : "g"(vv) : "mm0");\
	switch (nMMTs) {			\
	    case 8:	asm volatile ("punpcklbw %%mm0, %%mm0" : : : "mm0");\
	    case 4:	asm volatile ("punpcklwd %%mm0, %%mm0" : : : "mm0");\
	    case 2:	asm volatile ("punpckldq %%mm0, %%mm0" : : : "mm0");\
	};					\
	asm volatile (				\
	    MMX_PREFETCHW			\
	    "1: "				\
	    UNROLLED_FILL_MMX_STORE		\
	    "add $32, %%edi		\n\t"	\
	    "dec %%ecx			\n\t"	\
	    "jnz 1b			\n\t"	\
	    MMX_RESET				\
	    : "=&D"(dest)			\
	    : "0"(dest), "c"(count)		\
	    : "memory", "mm0");
#else
    #define CARRIER_SIZE	sizeof(unsigned long)
    #define UNROLLED_FILL_LOOP(src, dest, count)		\
	unsigned long *cdest ((unsigned long*) dest), cv;	\
	pack_type (v, cv);					\
	do { *cdest++ = cv; } while (--count);			\
	dest = (T*) cdest;
#endif

template <typename T>
inline T* unrolled_copy (const T* first, size_t count, T* result)
{
    copy_n_fast (first, count * sizeof(T), result);
    return (advance (result, count));
}

template <typename T>
T* unrolled_fill (T* first, size_t count, const T v)
{
    const size_t multi = CARRIER_SIZE / sizeof(T);
    size_t nCarriers = count / multi;
    if (nCarriers > 2) {					// This is faster than count > 64 (???)
	for (; uintptr_t(first) % CARRIER_SIZE; --count)	// This is faster than calculating nToAlign
	    *first++ = v;
	nCarriers = count / multi;
	UNROLLED_FILL_LOOP(v, first, nCarriers);
	count = count % multi;
    }
    for (; count; --count)
	*first++ = v;
    return (first);
}
#undef CARRIER_SIZE
#undef UNROLLED_FILL_LOOP
#undef UNROLLED_FILL_MMX_STORE
#undef MMX_RESET
#undef MMX_PREFETCH
#undef MMX_PREFETCHW

template <>
inline uint8_t* copy_backward (const uint8_t* first, const uint8_t* last, uint8_t* result)
{
    copy_n_backward_fast (first, last, result);
    return (result);
}

#define UNROLLED_COPY_SPECIALIZATION(type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return (unrolled_copy (first, distance (first, last), result)); }			\
template <> inline type* copy_n (const type* first, size_t count, type* result)		\
{ return (unrolled_copy (first, count, result)); }
#define UNROLLED_FILL_SPECIALIZATION(type)						\
template <> inline void fill (type* first, type* last, const type& value)		\
{ unrolled_fill (first, distance (first, last), value); }				\
template <> inline type* fill_n (type* first, size_t count, const type& value)	\
{ return (unrolled_fill (first, count, value)); }
UNROLLED_COPY_SPECIALIZATION(uint8_t)
UNROLLED_COPY_SPECIALIZATION(uint16_t)
UNROLLED_FILL_SPECIALIZATION(uint8_t)
UNROLLED_FILL_SPECIALIZATION(uint16_t)
#if CPU_HAS_MMX || (SIZE_OF_LONG > 4)
UNROLLED_COPY_SPECIALIZATION(uint32_t)
UNROLLED_FILL_SPECIALIZATION(uint32_t)
#endif
#if CPU_HAS_MMX
UNROLLED_COPY_SPECIALIZATION(float)
UNROLLED_FILL_SPECIALIZATION(float)
#endif
#undef UNROLLED_FILL_SPECIALIZATION
#undef UNROLLED_COPY_SPECIALIZATION
#endif // WANT_UNROLLED_COPY

// Specializations for void* and char*, aliasing the above optimized versions.
//
// All these need duplication with const and non-const arguments, since
// otherwise the compiler will default to the unoptimized version for
// pointers not const in the caller's context, such as local variables.
// These are all inline, but they sure slow down compilation... :(
//
#define COPY_ALIAS_FUNC(ctype, type, alias_type)			\
template <> inline type* copy (ctype* first, ctype* last, type* result)	\
{ return ((type*) copy ((const alias_type*) first, (const alias_type*) last, (alias_type*) result)); }
#if WANT_UNROLLED_COPY
#if HAVE_THREE_CHAR_TYPES
COPY_ALIAS_FUNC(const char, char, uint8_t)
COPY_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_ALIAS_FUNC(const int8_t, int8_t, uint8_t)
COPY_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_ALIAS_FUNC(const int16_t, int16_t, uint16_t)
COPY_ALIAS_FUNC(int16_t, int16_t, uint16_t)
COPY_ALIAS_FUNC(uint16_t, uint16_t, uint16_t)
#if CPU_HAS_MMX || (SIZE_OF_LONG > 4)
COPY_ALIAS_FUNC(const int32_t, int32_t, uint32_t)
COPY_ALIAS_FUNC(int32_t, int32_t, uint32_t)
COPY_ALIAS_FUNC(uint32_t, uint32_t, uint32_t)
#endif
#endif
COPY_ALIAS_FUNC(const void, void, uint8_t)
COPY_ALIAS_FUNC(void, void, uint8_t)
#undef COPY_ALIAS_FUNC
#define COPY_BACKWARD_ALIAS_FUNC(ctype, type, alias_type)				\
template <> inline type* copy_backward (ctype* first, ctype* last, type* result)	\
{ return ((type*) copy_backward ((const alias_type*) first, (const alias_type*) last, (alias_type*) result)); }
#if WANT_UNROLLED_COPY
#if HAVE_THREE_CHAR_TYPES
COPY_BACKWARD_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_BACKWARD_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(int16_t, int16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const int16_t, int16_t, uint8_t)
#endif
COPY_BACKWARD_ALIAS_FUNC(void, void, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const void, void, uint8_t)
#undef COPY_BACKWARD_ALIAS_FUNC
#define FILL_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline void fill (type* first, type* last, const v_type& value)	\
{ fill ((alias_type*) first, (alias_type*) last, (const alias_type&) value); }
FILL_ALIAS_FUNC(void, uint8_t, char)
FILL_ALIAS_FUNC(void, uint8_t, uint8_t)
#if WANT_UNROLLED_COPY
#if HAVE_THREE_CHAR_TYPES
FILL_ALIAS_FUNC(char, uint8_t, char)
FILL_ALIAS_FUNC(char, uint8_t, uint8_t)
#endif
FILL_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_ALIAS_FUNC(int16_t, uint16_t, int16_t)
#if CPU_HAS_MMX || (SIZE_OF_LONG > 4)
FILL_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#endif
#endif
#undef FILL_ALIAS_FUNC
#define COPY_N_ALIAS_FUNC(ctype, type, alias_type)					\
template <> inline type* copy_n (ctype* first, size_t count, type* result)	\
{ return ((type*) copy_n ((const alias_type*) first, count, (alias_type*) result)); }
COPY_N_ALIAS_FUNC(const void, void, uint8_t)
COPY_N_ALIAS_FUNC(void, void, uint8_t)
#if WANT_UNROLLED_COPY
#if HAVE_THREE_CHAR_TYPES
COPY_N_ALIAS_FUNC(const char, char, uint8_t)
COPY_N_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_N_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_N_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_N_ALIAS_FUNC(const int8_t, int8_t, uint8_t)
COPY_N_ALIAS_FUNC(int16_t, int16_t, uint16_t)
COPY_N_ALIAS_FUNC(uint16_t, uint16_t, uint16_t)
COPY_N_ALIAS_FUNC(const int16_t, int16_t, uint16_t)
#if CPU_HAS_MMX || (SIZE_OF_LONG > 4)
COPY_N_ALIAS_FUNC(int32_t, int32_t, uint32_t)
COPY_N_ALIAS_FUNC(uint32_t, uint32_t, uint32_t)
COPY_N_ALIAS_FUNC(const int32_t, int32_t, uint32_t)
#endif
#endif
#undef COPY_N_ALIAS_FUNC
#define FILL_N_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline type* fill_n (type* first, size_t n, const v_type& value)	\
{ return ((type*) fill_n ((alias_type*) first, n, (const alias_type&) value)); }
FILL_N_ALIAS_FUNC(void, uint8_t, char)
FILL_N_ALIAS_FUNC(void, uint8_t, uint8_t)
#if WANT_UNROLLED_COPY
#if HAVE_THREE_CHAR_TYPES
FILL_N_ALIAS_FUNC(char, uint8_t, char)
FILL_N_ALIAS_FUNC(char, uint8_t, uint8_t)
#endif
FILL_N_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_N_ALIAS_FUNC(int16_t, uint16_t, int16_t)
#if CPU_HAS_MMX || (SIZE_OF_LONG > 4)
FILL_N_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#endif
#endif
#undef FILL_N_ALIAS_FUNC

} // namespace ustl

#endif

