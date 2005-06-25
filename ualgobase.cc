// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// ualgobase.cc
//
// Copy and fill optimizations are here.
//

#ifndef NDEBUG	// Optimized code here. asserts slow it down, and are checked elsewhere.
#define NDEBUG
#endif

#include "ualgobase.h"

namespace ustl {

//----------------------------------------------------------------------
// Copy functions
//----------------------------------------------------------------------

#if __i386__
static inline void movsb_dir_up (void) __attribute__((always_inline));
static inline void movsb_dir_up (void) { asm volatile ("cld":::); }

static inline void movsb_dir_down (void) __attribute__((always_inline));
static inline void movsb_dir_down (void) { asm volatile ("std":::); }

static inline void movsb (const void*& src, size_t nBytes, void*& dest) __attribute__((always_inline));
static inline void movsb (const void*& src, size_t nBytes, void*& dest)
{
    asm volatile ("rep movsb"
	: "=&S"(src), "=&D"(dest), "=&c"(nBytes)
	: "0"(src), "1"(dest), "2"(nBytes)
	: "memory");
}

static inline void movsd (const void*& src, size_t nWords, void*& dest) __attribute__((always_inline));
static inline void movsd (const void*& src, size_t nWords, void*& dest)
{
    asm volatile ("rep movsl"
	: "=&S"(src), "=&D"(dest), "=&c"(nWords)
	: "0"(src), "1"(dest), "2"(nWords)
	: "memory");
}

#if CPU_HAS_MMX
#define MMX_ALIGN	16U	// Data must be aligned on this grain
#define MMX_BS		32U	// Assembly routines copy data this many bytes at a time.

static inline void simd_block_copy (const void* src, void* dest) __attribute__((always_inline));
static inline void simd_block_cleanup (void) __attribute__((always_inline));

static inline void simd_block_copy (const void* src, void* dest)
{
    #if CPU_HAS_SSE
    asm volatile (
	"movaps (%%esi), %%xmm0		\n\t"
	"movaps 16(%%esi), %%xmm1	\n\t"
	"movntps %%xmm0, (%%edi)	\n\t"
	"movntps %%xmm1, 16(%%edi)"
	: : "S"(src), "D"(dest)
	: "memory", "xmm0", "xmm1", "xmm2", "xmm3");
    #else
    asm volatile (
	"movq (%%esi), %%mm0	\n\t"
	"movq 8(%%esi), %%mm1	\n\t"
	"movq 16(%%esi), %%mm2	\n\t"
	"movq 24(%%esi), %%mm3	\n\t"
	"movq %%mm0, (%%edi)	\n\t"
	"movq %%mm1, 8(%%edi)	\n\t"
	"movq %%mm2, 16(%%edi)	\n\t"
	"movq %%mm3, 24(%%edi)"
	:
	: "S"(src), "D"(dest)
	: "memory", "mm0", "mm1", "mm2", "mm3", "st", "st(1)", "st(2)", "st(3)");
    #endif
}

static inline void simd_block_cleanup (void)
{
    #if !CPU_HAS_SSE
	simd::reset_mmx();
    #endif
    asm volatile ("sfence":::);
}

/// The fastest optimized raw memory copy.
void copy_n_fast (const void* src, size_t nBytes, void* dest)
{
    movsb_dir_up();
    size_t nHeadBytes = Align(uintptr_t(src), MMX_ALIGN) - uintptr_t(src);
    nHeadBytes = min (nHeadBytes, nBytes);
    movsb (src, nHeadBytes, dest);
    nBytes -= nHeadBytes;
    if (!(uintptr_t(dest) % MMX_ALIGN)) {
	const size_t nMiddleBlocks = nBytes / MMX_BS;
	for (uoff_t i = 0; i < nMiddleBlocks; ++ i) {
	    prefetch (advance (src, 512), 0, 0);
	    simd_block_copy (src, dest);
	    src = advance (src, MMX_BS);
	    dest = advance (dest, MMX_BS);
	}
	simd_block_cleanup();
	nBytes %= MMX_BS;
    }
    movsb (src, nBytes, dest);
}
#endif // CPU_HAS_MMX

/// The fastest optimized backwards raw memory copy.
void copy_n_backward_fast (const void* first, const void* last, void* result)
{
    prefetch (first, 0, 0);
    prefetch (result, 1, 0);
    size_t nBytes (distance (first, last));
    movsb_dir_down();
    size_t nHeadBytes = uintptr_t(last) % 4;
    last = advance (last, -1);
    result = advance (result, -1);
    movsb (last, nHeadBytes, result);
    nBytes -= nHeadBytes;
    if (uintptr_t(result) % 4 == 3) {
	const size_t nMiddleBlocks = nBytes / 4;
	last = advance (last, -3);
	result = advance (result, -3);
	movsd (last, nMiddleBlocks, result);
	nBytes %= 4;
    }
    movsb (last, nBytes, result);
    movsb_dir_up();
}
#endif // __i386__

} // namespace ustl

