// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
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
inline void iterator_swap (Iterator& a, Iterator& b)
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
    for (; count; --count)
	*result++ = *first++;
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
    for (; count; --count)
	*first++ = value;
    return (first);
}

//----------------------------------------------------------------------
// Optimized versions for standard types
//----------------------------------------------------------------------

template <typename T>
inline T* unrolled_copy (const T* first, size_t count, T* result)
{
    __builtin_prefetch (first, 0, 1);
    __builtin_prefetch (result, 1, 1);
    typedef u_long vec_t;
    const size_t multi = sizeof(vec_t) / sizeof(T);
    size_t nCarriers = count / multi;
    if (nCarriers > 2) {
	for (; count && (uintptr_t(first) % sizeof(vec_t) || uintptr_t(result) % sizeof(vec_t)); --count)
	    *result++ = *first++;
	nCarriers = count / multi;
	if (nCarriers) {
	    const vec_t* csrc ((const vec_t*) first);
	    vec_t* cdest ((vec_t*) result);
	    do { *cdest++ = *csrc++; } while (--nCarriers);
	    first = (const T*) csrc;
	    result = (T*) cdest;
	    count = count % multi;
	}
    }
    for (; count; --count)
	*result++ = *first++;
    return (result);
}

template <typename T>
inline T* unrolled_fill (T* first, size_t count, const T v)
{
    typedef u_long vec_t;
    __builtin_prefetch (first, 1, 1);
    const size_t multi = sizeof(vec_t) / sizeof(T);
    size_t nCarriers = count / multi;
    if (nCarriers > 2) {					// This is faster than count > 64 (???)
	for (; uintptr_t(first) % sizeof(vec_t); --count)	// This is faster than calculating nToAlign
	    *first++ = v;
	nCarriers = count / multi;
	vec_t *cdest ((vec_t*) first), cv;
	pack_type (v, cv);
	do { *cdest++ = cv; } while (--nCarriers);
	first = (T*) cdest;
	count = count % multi;
    }
    for (; count; --count)
	*first++ = v;
    return (first);
}

#ifdef CPU_HAS_3DNOW
    #define MMX_RESET_FPU	__builtin_ia32_femms
#else
    #define MMX_RESET_FPU
#endif

#define MMX_UNROLLED_COPY(type, vtype)		\
template <>					\
inline type* unrolled_copy (const type* first, size_t count, type* result)	\
{								\
    __builtin_prefetch (first, 0, 1);				\
    __builtin_prefetch (result, 1, 1);				\
    typedef u_long vec_t __attribute__((mode(vtype)));		\
    const size_t multi = (sizeof(vec_t) / sizeof(type)) * 4;	\
    size_t nCarriers = count / multi;				\
    if (nCarriers > 2) {					\
	for (; count && (uintptr_t(first) % sizeof(vec_t) || uintptr_t(result) % sizeof(vec_t)); --count)	\
	    *result++ = *first++;				\
	nCarriers = count / multi;				\
	if (nCarriers) {					\
	    const vec_t* csrc ((const vec_t*) first);		\
	    vec_t* cdest ((vec_t*) result), di[4];		\
	    do {						\
		di[0] = csrc[0];				\
		di[1] = csrc[1];				\
		di[2] = csrc[2];				\
		di[3] = csrc[3];				\
		cdest[0] = di[0];				\
		cdest[1] = di[1];				\
		cdest[2] = di[2];				\
		cdest[3] = di[3];				\
		cdest += 4;					\
		csrc += 4;					\
	    } while (--nCarriers);				\
	    MMX_RESET_FPU();					\
	    first = (const type*) csrc;				\
	    result = (type*) cdest;				\
	    count = count % multi;				\
	}							\
    }								\
    for (; count; --count)					\
	*result++ = *first++;					\
    return (result);						\
}

// There is no advantage in using MMX for copy_backward, because distance
// between last and result must be a multiple of 8, which rarely happens.
// Unrolling the loop creates a considerable improvement though.
//
#define UNROLLED_COPY_BACKWARD(ctype, type)	\
template <>					\
inline type* copy_backward (ctype* first, ctype* last, type* result)	\
{						\
    size_t count = distance (first, last);	\
    __builtin_prefetch (first, 0, 1);		\
    __builtin_prefetch (result - count, 1, 1);	\
    typedef u_long vec_t;					\
    const size_t multi = sizeof(vec_t) / sizeof(type);		\
    size_t nCarriers = count / multi;				\
    if (nCarriers) {						\
	for (; count && (uintptr_t(first) % sizeof(vec_t) || uintptr_t(result) % sizeof(vec_t)); --count)	\
	    *--result = *--last;				\
	nCarriers = count / multi;				\
	if (nCarriers) {					\
	    const vec_t* csrc ((const vec_t*) last);		\
	    vec_t* cdest ((vec_t*) result);			\
	    do { *--cdest = *--csrc; } while (--nCarriers);	\
	    last = (ctype*) csrc;		\
	    result = (type*) cdest;		\
	    count %= multi;			\
	}					\
    }						\
    for (; count; --count)			\
	*--result = *--last;			\
    return (result);				\
}

#define MMX_UNROLLED_FILL(type, vtype, vinit)				\
template <>								\
inline type* unrolled_fill (type* first, size_t count, const type v)	\
{									\
    __builtin_prefetch (first, 1, 1);					\
    typedef u_long vec_t __attribute__((mode(vtype)));			\
    const size_t multi = (sizeof(vec_t) / sizeof(type)) * 8;		\
    size_t nCarriers = count / multi;					\
    if (nCarriers > 3) {						\
	for (; uintptr_t(first) % sizeof(vec_t); --count)		\
	    *first++ = v;			\
	nCarriers = count / multi;		\
	vec_t *cdest ((vec_t*) first);		\
	const vec_t cv = vinit;			\
	do {					\
	    cdest[0] = cv; cdest[1] = cv;	\
	    cdest[2] = cv; cdest[3] = cv;	\
	    cdest[4] = cv; cdest[5] = cv;	\
	    cdest[6] = cv; cdest[7] = cv;	\
	    cdest += 8;				\
	} while (--nCarriers);			\
	MMX_RESET_FPU();			\
	first = (type*) cdest;			\
	count = count % multi;			\
    }						\
    for (; count; --count)			\
	*first++ = v;				\
    return (first);				\
}

#define __MMX_EIGHT_ARGS	{v,v,v,v,v,v,v,v}
#define __MMX_FOUR_ARGS		{v,v,v,v}
#define __MMX_TWO_ARGS		{v,v}
#ifdef HAVE_VECTOR_EXTENSIONS
MMX_UNROLLED_COPY(uint8_t, V8QI)
MMX_UNROLLED_COPY(uint16_t, V4HI)
MMX_UNROLLED_COPY(uint32_t, V2SI)
MMX_UNROLLED_COPY(float, V2SF)
MMX_UNROLLED_FILL(uint8_t, V8QI, __MMX_EIGHT_ARGS)
MMX_UNROLLED_FILL(uint16_t, V4HI, __MMX_FOUR_ARGS)
MMX_UNROLLED_FILL(uint32_t, V2SI, __MMX_TWO_ARGS)
MMX_UNROLLED_FILL(float, V2SF, __MMX_TWO_ARGS)
#endif
#undef __MMX_EIGHT_ARGS
#undef __MMX_FOUR_ARGS
#undef __MMX_TWO_ARGS

#define UNROLLED_COPY_SPECIALIZATION(type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return (unrolled_copy (first, distance (first, last), result)); }			\
template <> inline type* copy_n (const type* first, size_t count, type* result)		\
{ return (unrolled_copy (first, count, result)); }
UNROLLED_COPY_SPECIALIZATION(uint8_t)
UNROLLED_COPY_SPECIALIZATION(uint16_t)
#if HAVE_VECTOR_EXTENSIONS || (SIZE_OF_LONG > 4)
UNROLLED_COPY_SPECIALIZATION(uint32_t)
#endif
#if HAVE_VECTOR_EXTENSIONS
UNROLLED_COPY_SPECIALIZATION(float)
#endif
#undef UNROLLED_COPY_SPECIALIZATION
#define UNROLLED_FILL_SPECIALIZATION(type)						\
template <> inline void fill (type* first, type* last, const type& value)		\
{ unrolled_fill (first, distance (first, last), value); }				\
template <> inline type* fill_n (type* first, size_t count, const type& value)	\
{ return (unrolled_fill (first, count, value)); }
UNROLLED_FILL_SPECIALIZATION(uint8_t)
UNROLLED_FILL_SPECIALIZATION(uint16_t)
#if HAVE_VECTOR_EXTENSIONS || (SIZE_OF_LONG > 4)
UNROLLED_FILL_SPECIALIZATION(uint32_t)
#endif
#if HAVE_VECTOR_EXTENSIONS
UNROLLED_FILL_SPECIALIZATION(float)
#endif
#undef UNROLLED_FILL_SPECIALIZATION

// Specializations for void* and char*, aliasing the above optimized versions.
#define COPY_ALIAS_FUNC(type, alias_type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return ((type*) copy ((const alias_type*) first, (const alias_type*) last, (alias_type*) result)); }
COPY_ALIAS_FUNC(void, uint8_t)
COPY_ALIAS_FUNC(char, uint8_t)
COPY_ALIAS_FUNC(int8_t, uint8_t)
COPY_ALIAS_FUNC(int16_t, uint16_t)
COPY_ALIAS_FUNC(int32_t, uint32_t)
#undef COPY_ALIAS_FUNC
UNROLLED_COPY_BACKWARD(const uint8_t, uint8_t)
#define COPY_BACKWARD_ALIAS_FUNC(ctype, type, alias_type)				\
template <> inline type* copy_backward (ctype* first, ctype* last, type* result)	\
{ return ((type*) copy_backward ((const alias_type*) first, (const alias_type*) last, (alias_type*) result)); }
COPY_BACKWARD_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(void, void, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const void, void, uint8_t)
#undef COPY_BACKWARD_ALIAS_FUNC
#define FILL_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline void fill (type* first, type* last, const v_type& value)	\
{ fill ((alias_type*) first, (alias_type*) last, (const alias_type&) value); }
FILL_ALIAS_FUNC(void, uint8_t, char)
FILL_ALIAS_FUNC(void, uint8_t, u_char)
FILL_ALIAS_FUNC(char, uint8_t, char)
FILL_ALIAS_FUNC(char, uint8_t, u_char)
FILL_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_ALIAS_FUNC(int16_t, uint16_t, int16_t)
FILL_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#undef FILL_ALIAS_FUNC
#define COPY_N_ALIAS_FUNC(type, alias_type)					\
template <> inline type* copy_n (const type* first, size_t count, type* result)	\
{ return ((type*) copy_n ((const alias_type*) first, count, (alias_type*) result)); }
COPY_N_ALIAS_FUNC(void, uint8_t)
COPY_N_ALIAS_FUNC(char, uint8_t)
COPY_N_ALIAS_FUNC(int8_t, uint8_t)
COPY_N_ALIAS_FUNC(int16_t, uint16_t)
COPY_N_ALIAS_FUNC(int32_t, uint32_t)
#undef COPY_N_ALIAS_FUNC
#define FILL_N_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline type* fill_n (type* first, size_t n, const v_type& value)	\
{ return ((type*) fill_n ((alias_type*) first, n, (const alias_type&) value)); }
FILL_N_ALIAS_FUNC(void, uint8_t, char)
FILL_N_ALIAS_FUNC(void, uint8_t, u_char)
FILL_N_ALIAS_FUNC(char, uint8_t, char)
FILL_N_ALIAS_FUNC(char, uint8_t, u_char)
FILL_N_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_N_ALIAS_FUNC(int16_t, uint16_t, int16_t)
FILL_N_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#undef FILL_N_ALIAS_FUNC

} // namespace ustl

#endif

