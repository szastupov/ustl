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
/// \file uutility.h
///
/// \brief Utility templates.
///
/// Everything in here except min(), max(), distance(), and advance()
/// are uSTL extensions and are absent from other STL implementations.
///

#ifndef UUTILITY_H_6A58BD296269A82A4AAAA4FD19FDB3AC
#define UUTILITY_H_6A58BD296269A82A4AAAA4FD19FDB3AC

#include "utypes.h"
#include <assert.h>
#ifdef HAVE_BYTESWAP_H
    #include <byteswap.h>
#endif

namespace ustl {

#ifdef __GNUC__
    /// Returns the number of elements in a static vector
    #define VectorSize(v)	(sizeof(v) / sizeof(*v))
#else
    // Old compilers will not be able to evaluate *v on an empty vector.
    // The tradeoff here is that VectorSize will not be able to measure arrays of local structs.
    #define VectorSize(v)	(sizeof(v) / ustl::size_of_elements(1, v))
#endif

/// Returns the number of bits in the given type
#define BitsInType(t)	(sizeof(t) * CHAR_BIT)

/// Returns the mask of type \p t with the lowest \p n bits set.
#define BitMask(t,n)	(t(~t(0)) >> ((sizeof(t) * CHAR_BIT) - (n)))

/// Argument that is used only in debug builds (as in an assert)
#ifndef NDEBUG
    #define DebugArg(x)	x
#else
    #define DebugArg(x)
#endif

/// Shorthand for container iteration.
#define foreach(type,i,ctr)	for (type i = (ctr).begin(); i != (ctr).end(); ++ i)
/// Shorthand for container reverse iteration.
#define eachfor(type,i,ctr)	for (type i = (ctr).rbegin(); i != (ctr).rend(); ++ i)

/// The alignment performed by default.
const size_t c_DefaultAlignment = sizeof(void*);

/// Returns the minimum of \p a and \p b
template <typename T1, typename T2>
inline const T1 min (const T1& a, const T2& b)
{
    return (a < b ? a : b);
}

/// Returns the maximum of \p a and \p b
template <typename T1, typename T2>
inline const T1 max (const T1& a, const T2& b)
{
    return (b < a ? a : b);
}

/// \brief Divides \p n1 by \p n2 and rounds the result up.
/// This is in contrast to regular division, which rounds down.
/// Negative numbers are rounded down because they are an unusual case, supporting
/// which would require a branch. Since this is frequently used in graphics, the
/// speed is important.
///
template <typename T1, typename T2>
inline T1 DivRU (T1 n1, T2 n2)
{
    return (n1 / n2 + (n1 % n2 > 0));
}

/// \brief Rounds \p n up to be divisible by \p grain
template <typename T>
inline T Align (T n, T grain = c_DefaultAlignment)
{
    if (grain == 4)
	return (n % 4 ? (n & ~3) + 4 : n);
    else if (grain == 2)
	return (n % 2 ? ++n : n);
    else if (grain == 8)
	return (n % 8 ? (n & ~7) + 8 : n);
    else if (grain == 16)
	return (n % 16 ? (n & ~15) + 16 : n);
    else {
	const T remainder = n % grain;
	return (remainder ? n + (grain - remainder) : n);
    }
}

/// Returns the recommended alignment for type \p T.
template <typename T>
inline size_t alignof (T)
{
    return (__alignof__(T));
}

/// Offsets an iterator
template <typename T>
inline T advance (T i, ssize_t offset)
{
    return (i + offset);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/// Offsets a void pointer
template <>
inline const void* advance (const void* p, ssize_t offset)
{
    assert (p || !offset);
    return (reinterpret_cast<const uint8_t*>(p) + offset);
}

/// Offsets a void pointer
template <>
inline void* advance (void* p, ssize_t offset)
{
    assert (p || !offset);
    return (reinterpret_cast<uint8_t*>(p) + offset);
}
#endif

/// Returns the difference \p p1 - \p p2
template <typename T1, typename T2>
inline ptrdiff_t distance (T1 i1, T2 i2)
{
    return (i2 - i1);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define UNVOID_DISTANCE(T1const,T2const)				   \
template <> inline ptrdiff_t distance (T1const void* p1, T2const void* p2) \
{ return ((T2const uint8_t*)(p2) - (T1const uint8_t*)(p1)); }
UNVOID_DISTANCE(,)
UNVOID_DISTANCE(const,const)
UNVOID_DISTANCE(,const)
UNVOID_DISTANCE(const,)
#undef UNVOID_DISTANCE
#endif

/// \brief Returns the absolute value of \p v
/// Unlike the stdlib functions, this is inline and works with all types.
template <typename T>
inline T absv (T v)
{
    return (v < 0 ? -v : v);
}

/// \brief Returns -1 for negative values, 1 for positive, and 0 for 0
template <typename T>
inline T sign (T v)
{
    return (0 < v ? 1 : (v < 0 ? -1 : 0));
}

/// Returns the absolute value of the distance i1 and i2
template <typename T1, typename T2>
inline size_t abs_distance (T1 i1, T2 i2)
{
    return (absv (distance(i1, i2)));
}

/// Returns the size of \p n elements of size \p T
template <typename T>
inline size_t size_of_elements (size_t n, const T*)
{
    return (n * sizeof(T));
}

#ifdef HAVE_BYTESWAP_H

template <typename T>
inline T bswap (const T& v)
{
    switch (BitsInType(T)) {
	default:	return (v);
	case 16:	return (T (bswap_16 (uint16_t (v))));
	case 32:	return (T (bswap_32 (uint32_t (v))));
#if defined(bswap_64) && defined(HAVE_INT64_T)
	case 64:	return (T (bswap_64 (uint64_t (v))));
#endif
    };
}

#if __BYTE_ORDER == __BIG_ENDIAN
template <typename T> inline T le_to_native (const T& v) { return (bswap (v)); }
template <typename T> inline T be_to_native (const T& v) { return (v); }
template <typename T> inline T native_to_le (const T& v) { return (bswap (v)); }
template <typename T> inline T native_to_be (const T& v) { return (v); }
#elif __BYTE_ORDER == __LITTLE_ENDIAN
template <typename T> inline T le_to_native (const T& v) { return (v); }
template <typename T> inline T be_to_native (const T& v) { return (bswap (v)); }
template <typename T> inline T native_to_le (const T& v) { return (v); }
template <typename T> inline T native_to_be (const T& v) { return (bswap (v)); }
#else
    #error Your system does not define __BYTE_ORDER. Please define it in config.h
#endif // __BYTE_ORDER
#endif // HAVE_BYTESWAP_H

/// Template for for_each to call delete
template <typename T>
inline void Delete (T*& p)
{
    delete p;
    p = NULL;
}

/// Template for for_each to call delete
template <typename T>
inline void DeleteVector (T*& p)
{
    delete [] p;
    p = NULL;
}

/// Template of making != from ! and ==
template <typename T>
inline bool operator!= (const T& x, const T& y)
{
    return (!(x == y));
}

/// Template of making > from <
template <typename T>
inline bool operator> (const T& x, const T& y)
{
    return (y < x);
}

/// Template of making <= from < and ==
template <typename T>
inline bool operator<= (const T& x, const T& y)
{
    return (x == y || x < y);
}

/// Template of making >= from < and ==
template <typename T>
inline bool operator>= (const T& x, const T& y)
{
    return (x == y || y < x);
}

/// Packs \p s multiple times into \p b. Useful for loop unrolling.
template <typename TSmall, typename TBig>
inline void pack_type (TSmall s, TBig& b)
{
    const size_t n = sizeof(TBig) / sizeof(TSmall);
    b = s;
    // Calls to min are here to avoid warnings for shifts bigger than the type. min will be gone when optimized.
    if (n < 2) return;
    b = (b << min (BitsInType(TSmall), BitsInType(TBig))) | b;
    if (n < 4) return;
    b = (b << min (BitsInType(TSmall) * 2, BitsInType(TBig))) | b;
    if (n < 8) return;
    b = (b << min (BitsInType(TSmall) * 4, BitsInType(TBig))) | b;
}

/// Sets the contents of \p pm to 1 and returns true if the previous value was 0.
inline bool TestAndSet (int* pm)
{
#if CPU_HAS_CMPXCHG8
    register bool rv;
    int oldVal (1);
    asm volatile ( // cmpxchg compares to %eax and swaps if equal
	"cmpxchgl %3, %1\n\t"
	"sete %0"
	: "=r" (rv), "=m" (*pm), "=r" (oldVal)
	: "2" (oldVal), "a" (0)
	: "memory");
    return (rv);
#elif __i386__
    int oldVal (1);
    asm volatile ("xchgl %0, %1" : "=r"(oldVal), "=m"(*pm) : "0"(oldVal), "m"(*pm) : "memory");
    return (!oldVal);
#elif __sparc32__	// This has not been tested
    int rv;
    asm volatile ("ldstub %1, %0" : "=r"(rv), "=m"(*pm) : "m"(pm));
    return (!rv);
#else
    const int oldVal (*pm);
    *pm = 1;
    return (!oldVal);
#endif
}

namespace simd {
/// Call after you are done using SIMD algorithms for 64 bit tuples.
#if CPU_HAS_MMX
    #if CPU_HAS_3DNOW
	inline void reset_mmx (void) { asm ("femms":::"mm0","mm1","mm2","mm3","mm4","mm5","mm6","mm7"); }
    #else
	inline void reset_mmx (void) { asm ("emms":::"mm0","mm1","mm2","mm3","mm4","mm5","mm6","mm7"); }
    #endif
#else
    inline void reset_mmx (void) {}
#endif
} // namespace simd

/// \brief Type that is not size_t
///
/// Because size_t may be declared as unsigned long or unsigned int on
/// different machines, this macro is convenient when defining overloads
/// of size_t to use other types.
///
#ifdef SIZE_T_IS_LONG
    #define NOT_SIZE_T_I_OR_L	unsigned int
#else
    #define NOT_SIZE_T_I_OR_L	unsigned long
#endif

/// \brief Required when you want to overload size_t and a pointer.
///
/// The compiler will happily cast a number to a pointer and declare
/// that the overload is ambiguous unless you define overloads for all
/// possible integral types that a number may represent. This behaviour,
/// although braindead, is in the ANSI standard, and thus not a bug. If
/// you want to change the standard, the best solution is to disallow any
/// implicit casts to pointer from an integral type. Ironically, such an
/// implicit cast is already detected by gcc.
///
#define OVERLOAD_POINTER_AND_SIZE_T_V2(name, arg1type)						\
    inline void	name (arg1type a1, short a2)			{ name (a1, size_t(a2)); }	\
    inline void	name (arg1type a1, unsigned short a2)		{ name (a1, size_t(a2)); }	\
    inline void	name (arg1type a1, int a2)			{ name (a1, size_t(a2)); }	\
    inline void	name (arg1type a1, long a2)			{ name (a1, size_t(a2)); }	\
    inline void	name (arg1type a1, NOT_SIZE_T_I_OR_L a2)	{ name (a1, size_t(a2)); }

} // namespace ustl

#endif

