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
    while (count) {
	*result = *first;
	++ result;
        -- count;
	++ first;
    }
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
/// [first, first+n). That is, for every iterator i in [first, first+n),
/// it performs the assignment *i = value. The return value is first + n.
/// \ingroup GeneratorAlgorithms
///
template <typename OutputIterator, typename T>
inline OutputIterator fill_n (OutputIterator first, size_t n, const T& value)
{
    while (n) {
	*first++ = value;
	-- n;
    }
    return (first);
}

// Optimized versions for standard types
template <typename T>
inline T* unrolled_copy (const T* first, size_t count, T* result)
{
    __builtin_prefetch (first, 0, 1);
    __builtin_prefetch (result, 1, 1);
    if (result >= first && result < first + count) {
	first += count;
	result += count;
	while (count) {
	    *--result = *--first;
	    -- count;
	}
    } else {
	typedef u_long carrier_t;
	while (count && (uintptr_t(first) % sizeof(carrier_t) || uintptr_t(result) % sizeof(carrier_t))) {
	    *result++ = *first++;
	    -- count;
	}
	size_t nCarriers = count / (sizeof(carrier_t) / sizeof(T));
	if (nCarriers) {
	    const carrier_t* csrc ((const carrier_t*) first);
	    carrier_t* cdest ((carrier_t*) result);
	    do {
		*cdest++ = *csrc++;
	    } while (--nCarriers);
	    first = (const T*) csrc;
	    result = (T*) cdest;
	    count = count % sizeof(carrier_t);
	}
	while (count) {
	    *result++ = *first++;
	    -- count;
	}
    }
    return (result);
}

template <typename T>
inline T* unrolled_fill (T* first, size_t count, const T& value)
{
    __builtin_prefetch (first, 1, 1);
    typedef u_long carrier_t;
    while (count && uintptr_t(first) % sizeof(carrier_t)) {
	*first++ = value;
	-- count;
    }
    size_t repCount (sizeof(carrier_t) / sizeof(T));
    size_t nCarriers = count / repCount;
    if (nCarriers) {
	carrier_t* cdest ((carrier_t*) first);
	carrier_t cvalue (value);
	if (repCount > 1) {
	    cvalue = (cvalue << BitsInType(T)) | cvalue;
	    if (repCount > 2) {
		cvalue = (cvalue << BitsInType(T) * 2) | cvalue;
		if (repCount > 4)
		    cvalue = (cvalue << BitsInType(T) * 4) | cvalue;
	    }
	}
	do {
	    *cdest++ = cvalue;
	} while (--nCarriers);
	first = (T*) cdest;
	count = count % sizeof(carrier_t);
    }
    while (count) {
	*first++ = value;
	-- count;
    }
    return (first);
}

#define UNROLLED_COPY_SPECIALIZATION(type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return (unrolled_copy (first, distance (first, last), result)); }			\
template <> inline type* copy_n (const type* first, size_t count, type* result)		\
{ return (unrolled_copy (first, count, result)); }
UNROLLED_COPY_SPECIALIZATION(uint8_t)
UNROLLED_COPY_SPECIALIZATION(uint16_t)
#if SIZE_OF_LONG > 4
UNROLLED_COPY_SPECIALIZATION(uint32_t)
#endif
#undef UNROLLED_COPY_SPECIALIZATION
#define UNROLLED_FILL_SPECIALIZATION(type)						\
template <> inline void fill (type* first, type* last, const type& value)		\
{ unrolled_fill (first, distance (first, last), value); }				\
template <> inline type* fill_n (type* first, size_t count, const type& value)	\
{ return (unrolled_fill (first, count, value)); }
UNROLLED_FILL_SPECIALIZATION(uint8_t)
UNROLLED_FILL_SPECIALIZATION(uint16_t)
#if SIZE_OF_LONG > 4
UNROLLED_FILL_SPECIALIZATION(uint32_t)
#endif
#undef UNROLLED_FILL_SPECIALIZATION

// Specializations for void* and char*, aliasing the above optimized versions.
#define COPY_ALIAS_FUNC(type, alias_type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return ((type*) copy ((const alias_type*) first, (const alias_type*) last, (alias_type*) result)); }
COPY_ALIAS_FUNC(void, uint8_t)
COPY_ALIAS_FUNC(char, uint8_t)
#undef COPY_ALIAS_FUNC
#define FILL_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline void fill (type* first, type* last, const v_type& value)	\
{ fill ((alias_type*) first, (alias_type*) last, (const alias_type&) value); }
FILL_ALIAS_FUNC(void, uint8_t, char)
FILL_ALIAS_FUNC(void, uint8_t, u_char)
FILL_ALIAS_FUNC(char, uint8_t, char)
FILL_ALIAS_FUNC(char, uint8_t, u_char)
#undef FILL_ALIAS_FUNC
#define COPY_N_ALIAS_FUNC(type, alias_type)					\
template <> inline type* copy_n (const type* first, size_t count, type* result)	\
{ return ((type*) copy_n ((const alias_type*) first, count, (alias_type*) result)); }
COPY_N_ALIAS_FUNC(void, uint8_t)
COPY_N_ALIAS_FUNC(char, uint8_t)
#undef COPY_N_ALIAS_FUNC
#define FILL_N_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline type* fill_n (type* first, size_t n, const v_type& value)	\
{ return ((type*) fill_n ((alias_type*) first, n, (const alias_type&) value)); }
FILL_N_ALIAS_FUNC(void, uint8_t, char)
FILL_N_ALIAS_FUNC(void, uint8_t, u_char)
FILL_N_ALIAS_FUNC(char, uint8_t, char)
FILL_N_ALIAS_FUNC(char, uint8_t, u_char)
#undef FILL_N_ALIAS_FUNC

} // namespace ustl

#endif

