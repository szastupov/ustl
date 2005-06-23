// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// ualgobase.cc
//
// Copy and fill optimizations are here.
//

#include "ualgobase.h"

namespace ustl {

//----------------------------------------------------------------------
// Copy functions
//----------------------------------------------------------------------

#if __i386__
static inline void movsb_cld (void) __attribute__((always_inline));
static inline void movsb_cld (void) { asm volatile ("cld":::); }

static inline void movsb (const char*& src, size_t nBytes, char*& dest) __attribute__((always_inline));
static inline void movsb (const char*& src, size_t nBytes, char*& dest)
{
    asm volatile ("rep movsb"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

#if CPU_HAS_MMX
static inline void simd_block_copy (const char* src, char* dest) __attribute__((always_inline));
static inline void simd_block_cleanup (void) __attribute__((always_inline));

static inline void simd_block_copy (const char* src, char* dest)
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
void copy_n_fast (const void* vsrc, size_t nBytes, void* vdest)
{
    const char* src (reinterpret_cast<const char*>(vsrc));
    char* dest (reinterpret_cast<char*>(vdest));
    movsb_cld();
    const size_t nHeadBytes = min (nBytes, Align(uintptr_t(src), 16U) - uintptr_t(src));
    movsb (src, nHeadBytes, dest);
    nBytes -= nHeadBytes;
    if (!(uintptr_t(dest) % 16)) {
	const size_t nMiddleBlocks = nBytes / 32;
	for (uoff_t i = 0; i < nMiddleBlocks; ++ i) {
	    prefetch (src + 512, 0, 0);
	    simd_block_copy (src, dest);
	    src += 32;
	    dest += 32;
	}
	simd_block_cleanup();
	nBytes %= 32;
    }
    movsb (src, nBytes, dest);
}

/// The fastest optimized backwards raw memory copy.
void copy_n_backward (const void* vsrc, size_t count, void* vdest)
{
    const char* src (reinterpret_cast<const char*>(vsrc));
    const char* last (src + count);
    char* dest (reinterpret_cast<char*>(vdest) + count);
    prefetch (src, 0, 1);
    prefetch (dest - count, 1, 1);
    typedef unsigned long vec_t;
    const size_t multi = sizeof(vec_t) / sizeof(uint8_t);
    size_t nCarriers = count / multi;
    if (nCarriers) {
	for (; count && (uintptr_t(last) % sizeof(vec_t) || uintptr_t(dest) % sizeof(vec_t)); --count)
	    *--dest = *--last;
	nCarriers = count / multi;
	if (nCarriers) {
	    const vec_t* csrc ((const vec_t*) last);
	    vec_t* cdest ((vec_t*) dest);
	    do { *--cdest = *--csrc; } while (--nCarriers);
	    last = (const char*) csrc;
	    dest = (char*) cdest;
	    count %= multi;
	}
    }
    for (; count; --count)
	*--dest = *--last;
}
#endif // CPU_HAS_MMX
#endif // __i386__

} // namespace ustl

