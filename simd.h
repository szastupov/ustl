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
/// \file simd.h
/// \brief SIMD-type algorithms, with hardware acceleration, if available.
///
/// All algorithms are container-based because iterator syntax is just too
/// damn verbose and because the specializations need to be able to tell
/// how many elements are in the container in order to choose proper SIMD
/// instruction set (i.e.: 4 floats select SSE, while 2 floats select 3dNow!)
/// Specializations are only for the tuple template because the container
/// must be of a fixed and compile-time-known size for the compiler to be
/// able to choose the specialization.
///

#ifndef SIMD_H_39BE2D970DF4BD00508CCFFB482496F9
#define SIMD_H_39BE2D970DF4BD00508CCFFB482496F9

#include "ualgo.h"
#include "ulimits.h"

namespace ustl {
namespace simd {

//----------------------------------------------------------------------
// Generic algorithms
//----------------------------------------------------------------------

/// Applies \p op to each element in \p op1.
template <typename Ctr, typename UnaryOperation>
inline void packop (Ctr& op1, UnaryOperation op)
{
    transform (op1.begin(), op1.end(), op);
}

/// Copies \p op1 into \p result.
template <typename Ctr>
inline void passign (const Ctr& op1, Ctr& result)
{
    assert (op1.size() <= result.size());
    copy (op1, result.begin());
}

/// Copies \p result.size() elements from \p op1 to \p result.
template <typename Ctr>
inline void ipassign (typename Ctr::const_iterator op1, Ctr& result)
{
    copy_n (op1, result.size(), result.begin());
}

/// Applies \p op to each element in \p op1 and \p op2 and stores in \p op2.
template <typename Ctr, typename BinaryOperation>
inline void packop (const Ctr& op1, Ctr& op2, BinaryOperation op)
{
    assert (op2.size() <= op1.size());
    transform (op2.begin(), op2.end(), op1.begin(), op2.begin(), op);
}

/// Applies \p op to corresponding elements in \p op1 and \p op2 and stores in \p result.
template <typename Ctr, typename BinaryOperation>
inline void packop (const Ctr& op1, const Ctr& op2, Ctr& result, BinaryOperation op)
{
    assert (op1.size() <= op2.size() && op1.size() <= result.size());
    passign (op1, result);
    packop (op2, result);
}

/// Call after you are done using SIMD algorithms for 64 bit tuples.
#if CPU_HAS_MMX
    #if CPU_HAS_3DNOW
	inline void reset_mmx (void) { asm ("femms"); }
    #else
	inline void reset_mmx (void) { asm ("emms"); }
    #endif
#else
    inline void reset_mmx (void) {}
#endif

// Functionoids for SIMD operations, like saturation arithmetic, shifts, etc.
template <class T> struct fpadds: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const
    { const T c_MaxValue = numeric_limits<T>::max();
      return ((b > c_MaxValue - a) ? c_MaxValue : a + b); } };
template <class T> struct fpsubs: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const
    { const T c_MinValue = numeric_limits<T>::min();
      return ((a < c_MinValue + b) ? c_MinValue : a - b); } };
template <class T> struct fpshl	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a << b); } };
template <class T> struct fpshr	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a >> b); } };
template <class T> struct fpmin	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (min (a, b)); } };
template <class T> struct fpmax	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (max (a, b)); } };
template <class T> struct fpavg	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return ((a + b) / 2); } };
template <class T> struct fpreciprocal	: public unary_function<T,T> { inline T operator()(const T& a) const { return (1 / a); } };
#ifdef __GNUC__
template <class T> struct fpsqrt	: public unary_function<T,T> { inline T operator()(const T& a) const { return (__builtin_sqrtf (a)); } };
template <class T> struct fprecipsqrt	: public unary_function<T,T> { inline T operator()(const T& a) const { return (1 / __builtin_sqrtf (a)); } };
#endif

#define SIMD_PACKEDOP1(name, operation)		\
template <typename Ctr>				\
inline void name (const Ctr& op1)		\
{						\
    typedef typename Ctr::value_type value_t;	\
    packop (op1, operation<value_t>());		\
}
#define SIMD_PACKEDOP2(name, operation)		\
template <typename Ctr>				\
inline void name (const Ctr& op1, Ctr& op2)	\
{						\
    typedef typename Ctr::value_type value_t;	\
    packop (op1, op2, operation<value_t>());	\
}
#define SIMD_PACKEDOP3(name, operation)			\
template <typename Ctr>					\
inline void name (const Ctr& op1, const Ctr& op2, Ctr& result)	\
{							\
    typedef typename Ctr::value_type value_t;		\
    packop (op1, op2, result, operation<value_t>());	\
}

SIMD_PACKEDOP2 (padd, plus)
SIMD_PACKEDOP2 (psub, minus)
SIMD_PACKEDOP2 (pmul, multiplies)
SIMD_PACKEDOP2 (pdiv, divides)
SIMD_PACKEDOP2 (pand, bitwise_and)
SIMD_PACKEDOP2 (por, bitwise_or)
SIMD_PACKEDOP2 (pxor, bitwise_xor)
SIMD_PACKEDOP2 (pshl, fpshl)
SIMD_PACKEDOP2 (pshr, fpshr)
SIMD_PACKEDOP2 (psubs, fpsubs)
SIMD_PACKEDOP2 (pmin, fpmin)
SIMD_PACKEDOP2 (pmax, fpmax)
SIMD_PACKEDOP2 (pavg, fpavg)

SIMD_PACKEDOP1 (precip, fpreciprocal)
SIMD_PACKEDOP1 (psqrt, fpsqrt)
SIMD_PACKEDOP1 (precipsqrt, fprecipsqrt)

SIMD_PACKEDOP3 (padd, plus)
SIMD_PACKEDOP3 (psub, minus)
SIMD_PACKEDOP3 (pmul, multiplies)
SIMD_PACKEDOP3 (pdiv, divides)
SIMD_PACKEDOP3 (pand, bitwise_and)
SIMD_PACKEDOP3 (por, bitwise_or)
SIMD_PACKEDOP3 (pxor, bitwise_xor)
SIMD_PACKEDOP3 (pshl, fpshl)
SIMD_PACKEDOP3 (pshr, fpshr)
SIMD_PACKEDOP3 (padds, fpadds)
SIMD_PACKEDOP3 (psubs, fpsubs)
SIMD_PACKEDOP3 (pmin, fpmin)
SIMD_PACKEDOP3 (pmax, fpmax)
SIMD_PACKEDOP3 (pavg, fpavg)

#undef SIMD_PACKEDOP3
#undef SIMD_PACKEDOP2
#undef SIMD_PACKEDOP1

//----------------------------------------------------------------------
// Vector types to cast tuple data to
//----------------------------------------------------------------------

typedef uintmax_t v8qi_t __attribute__((mode(V8QI)));
typedef uintmax_t v4hi_t __attribute__((mode(V4HI)));
typedef uintmax_t v8hi_t __attribute__((mode(V8HI)));
typedef uintmax_t v2si_t __attribute__((mode(V2SI)));
typedef uintmax_t v4si_t __attribute__((mode(V4SI)));
typedef uintmax_t v1di_t __attribute__((mode(V1DI)));
typedef uintmax_t v2sf_t __attribute__((mode(V2SF)));
typedef uintmax_t v4sf_t __attribute__((mode(V4SF)));
typedef uintmax_t v2df_t __attribute__((mode(V2DF)));

//----------------------------------------------------------------------
// Hardware accelerated specializations
//----------------------------------------------------------------------

#define SIMD_PKOP2_SPEC(n, type, optype)	\
template <>					\
inline void packop (const tuple<n,type>& oin, tuple<n,type>& oout, optype<type>)
#define SIMD_PASSIGN_SPEC(n, type)		\
template <>					\
inline void passign (const tuple<n,type>& oin, tuple<n,type>& oout)
#define SIMD_IPASSIGN_SPEC(n, type)		\
template <>					\
inline void ipassign (tuple<n,type>::const_iterator oin, tuple<n,type>& oout)

#if CPU_HAS_MMX
#define STD_MMX_ARGS(ptr_t)		"=&y"(*(ptr_t*)oout.begin()) : \
					"y"(*(const ptr_t*)oin.begin()), \
					"0"(*(ptr_t*)oout.begin())
SIMD_PASSIGN_SPEC(8,uint8_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,plus)		{ asm ("paddb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,minus)	{ asm ("psubb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,fpadds)	{ asm ("paddusb %1, %0" : STD_MMX_ARGS(v8qi_t));}
SIMD_PKOP2_SPEC(8,uint8_t,fpsubs)	{ asm ("psubusb %1, %0" : STD_MMX_ARGS(v8qi_t));}

SIMD_PASSIGN_SPEC(8,int8_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,plus)		{ asm ("paddb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,minus)		{ asm ("psubb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,fpadds)	{ asm ("paddsb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,fpsubs)	{ asm ("psubsb %1, %0" : STD_MMX_ARGS(v8qi_t));	}

SIMD_PASSIGN_SPEC(4,uint16_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,plus)	{ asm ("paddw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,minus)	{ asm ("psubw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
/// \fixme psllw doesn't seem to do anything
//SIMD_PKOP2_SPEC(4,uint16_t,fpshl)	{ asm ("psllw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
//SIMD_PKOP2_SPEC(4,uint16_t,fpshr)	{ asm ("psrlw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,fpadds)	{ asm ("paddusw %1, %0" : STD_MMX_ARGS(v4hi_t));}
SIMD_PKOP2_SPEC(4,uint16_t,fpsubs)	{ asm ("psubusw %1, %0" : STD_MMX_ARGS(v4hi_t));}

SIMD_PASSIGN_SPEC(4,int16_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,plus)		{ asm ("paddw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,minus)	{ asm ("psubw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v4hi_t));	}
//SIMD_PKOP2_SPEC(4,int16_t,fpshl)	{ asm ("psllw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
//SIMD_PKOP2_SPEC(4,int16_t,fpshr)	{ asm ("psrlw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,fpadds)	{ asm ("paddsw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,fpsubs)	{ asm ("psubsw %1, %0" : STD_MMX_ARGS(v4hi_t));	}

SIMD_PASSIGN_SPEC(2,uint32_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,uint32_t,plus)	{ asm ("paddd %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,uint32_t,minus)	{ asm ("psubd %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,uint32_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,uint32_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,uint32_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v2si_t));	}
//SIMD_PKOP2_SPEC(2,uint32_t,fpshl)	{ asm ("pslld %1, %0" : STD_MMX_ARGS(v2si_t));	}
//SIMD_PKOP2_SPEC(2,uint32_t,fpshr)	{ asm ("psrld %1, %0" : STD_MMX_ARGS(v2si_t));	}

SIMD_PASSIGN_SPEC(2,int32_t)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,int32_t,plus)		{ asm ("paddd %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,int32_t,minus)	{ asm ("psubd %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,int32_t,bitwise_and)	{ asm ("pand  %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,int32_t,bitwise_or)	{ asm ("por   %1, %0" : STD_MMX_ARGS(v2si_t));	}
SIMD_PKOP2_SPEC(2,int32_t,bitwise_xor)	{ asm ("pxor  %1, %0" : STD_MMX_ARGS(v2si_t));	}
//SIMD_PKOP2_SPEC(2,int32_t,fpshl)	{ asm ("pslld %1, %0" : STD_MMX_ARGS(v2si_t));	}
//SIMD_PKOP2_SPEC(2,int32_t,fpshr)	{ asm ("psrld %1, %0" : STD_MMX_ARGS(v2si_t));	}

#if CPU_HAS_SSE || CPU_HAS_3DNOW
SIMD_PKOP2_SPEC(8,uint8_t,fpavg)	{ asm ("pavgb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,int8_t,fpavg)		{ asm ("pavgb %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(4,uint16_t,fpavg)	{ asm ("pavgw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,fpavg)	{ asm ("pavgw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,fpmin)	{ asm ("pminub %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(8,uint8_t,fpmax)	{ asm ("pmaxub %1, %0" : STD_MMX_ARGS(v8qi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,fpmax)	{ asm ("pmaxsw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
SIMD_PKOP2_SPEC(4,int16_t,fpmin)	{ asm ("pminsw %1, %0" : STD_MMX_ARGS(v4hi_t));	}
#endif // CPU_HAS_SSE || CPU_HAS_3DNOW

#if CPU_HAS_3DNOW
SIMD_PASSIGN_SPEC(2,float)		{ asm ("movq  %1, %0" : STD_MMX_ARGS(v2sf_t));	}
SIMD_PKOP2_SPEC(2,float,plus)		{ asm ("pfadd %1, %0" : STD_MMX_ARGS(v2sf_t));	}
SIMD_PKOP2_SPEC(2,float,minus)		{ asm ("pfsub %1, %0" : STD_MMX_ARGS(v2sf_t));	}
SIMD_PKOP2_SPEC(2,float,multiplies)	{ asm ("pfmul %1, %0" : STD_MMX_ARGS(v2sf_t));	}
SIMD_PKOP2_SPEC(2,float,fpmin)		{ asm ("pfmin %1, %0" : STD_MMX_ARGS(v2sf_t));	}
SIMD_PKOP2_SPEC(2,float,fpmax)		{ asm ("pfmax %1, %0" : STD_MMX_ARGS(v2sf_t));	}
#endif // CPU_HAS_3DNOW

#undef STD_MMX_ARGS

#define STD_MMX_I_ARGS(ptr_t)		"=&y"(*(ptr_t*)oout.begin()) :	\
					"y"(*(const ptr_t*)oin), 	\
					"0"(*(ptr_t*)oout.begin())
SIMD_IPASSIGN_SPEC(8,uint8_t)		{ asm ("movq  %1, %0" : STD_MMX_I_ARGS(v8qi_t));}
SIMD_IPASSIGN_SPEC(4,uint16_t)		{ asm ("movq  %1, %0" : STD_MMX_I_ARGS(v4hi_t));}
SIMD_IPASSIGN_SPEC(2,uint32_t)		{ asm ("movq  %1, %0" : STD_MMX_I_ARGS(v2si_t));}
SIMD_IPASSIGN_SPEC(2,float)		{ asm ("movq  %1, %0" : STD_MMX_I_ARGS(v2sf_t));}
#undef STD_MMX_I_ARGS

#endif // CPU_HAS_MMX

#if CPU_HAS_SSE
#define STD_SSE_ARGS(ptr_t)		"=&x"(*(ptr_t*)oout.begin()) : \
					"x"(*(const ptr_t*)oin.begin()), \
					"0"(*(ptr_t*)oout.begin())
SIMD_PASSIGN_SPEC(4,float)		{ asm ("movups %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,plus)		{ asm ("addps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,minus)		{ asm ("subps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,multiplies)	{ asm ("mulps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,divides)	{ asm ("divps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,bitwise_and)	{ asm ("andps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,bitwise_or)	{ asm ("orps  %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,bitwise_xor)	{ asm ("xorps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,fpmax)		{ asm ("maxps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
SIMD_PKOP2_SPEC(4,float,fpmin)		{ asm ("minps %1, %0" : STD_SSE_ARGS(v4sf_t));	}
#undef STD_SSE_ARGS
#define STD_SSE_I_ARGS(ptr_t)		"=&y"(*(ptr_t*)oout.begin()) :	\
					"y"(*(const ptr_t*)oin), 	\
					"0"(*(ptr_t*)oout.begin())
SIMD_IPASSIGN_SPEC(4,float)		{ asm ("movq %1, %0" : STD_SSE_I_ARGS(v4sf_t));}
#undef STD_SSE_I_ARGS
#endif // CPU_HAS_SSE

#undef SIMD_PACKEDOP_SPEC

} // namespace simd
} // namespace ustl

#endif

