// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//

#include <ustl.h>
#include <time.h>
using namespace ustl;

//----------------------------------------------------------------------
// Copy functions
//----------------------------------------------------------------------

#if __i386__
void movsb_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"cld		\n\t"
	"rep movsb	\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

void movsd_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $2, %%ecx	\n\t"
	"cld		\n\t"
	"rep movsl	\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

void risc_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $2, %%ecx		\n\t"
	"1:			\n\t"
	"mov (%%esi), %%eax	\n\t"
	"add $4, %%esi		\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"add $4, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "eax");
}

void unroll_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $4, %%ecx		\n\t"
	"1:			\n\t"
	"mov (%%esi), %%eax	\n\t"
	"mov 4(%%esi), %%edx	\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"mov %%edx, 4(%%edi)	\n\t"
	"mov 8(%%esi), %%eax	\n\t"
	"mov 12(%%esi), %%edx	\n\t"
	"mov %%eax, 8(%%edi)	\n\t"
	"mov %%edx, 12(%%edi)	\n\t"
	"add $16, %%esi		\n\t"
	"add $16, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "eax", "edx");
}

#if CPU_HAS_MMX
void mmx_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $5, %%ecx		\n\t"
	"1:			\n\t"
#if CPU_HAS_3DNOW
	"prefetch 512(%%esi)	\n\t"
#else
	"prefetchnta 512(%%esi)	\n\t"
#endif
	"movq (%%esi), %%mm0	\n\t"
	"movq 8(%%esi), %%mm1	\n\t"
	"movq 16(%%esi), %%mm2	\n\t"
	"movq 24(%%esi), %%mm3	\n\t"
#if 0
	"movntq %%mm0, (%%edi)	\n\t"
	"movntq %%mm1, 8(%%edi)	\n\t"
	"movntq %%mm2, 16(%%edi)\n\t"
	"movntq %%mm3, 24(%%edi)\n\t"
#else
	"movq %%mm0, (%%edi)	\n\t"
	"movq %%mm1, 8(%%edi)	\n\t"
	"movq %%mm2, 16(%%edi)	\n\t"
	"movq %%mm3, 24(%%edi)	\n\t"
#endif
	"add $32, %%esi		\n\t"
	"add $32, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	"emms"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "mm0", "mm1", "mm2", "mm3");
}
#endif // CPU_HAS_MMX

#if CPU_HAS_SSE
void sse_copy (const char* src, size_t nBytes, char* dest)
{
    for (; nBytes && (uintptr_t(src) % 16 || uintptr_t(dest) % 16); --nBytes)
	*dest++ = *src++;
    asm volatile (
	"shr $6, %%ecx			\n\t"
	"1:				\n\t"
	"prefetch 512(%%esi)		\n\t"
	"movaps (%%esi), %%xmm0		\n\t"
	"movaps 16(%%esi), %%xmm1	\n\t"
	"movaps 32(%%esi), %%xmm2	\n\t"
	"movaps 48(%%esi), %%xmm3	\n\t"
	"movntps %%xmm0, (%%edi)	\n\t"
	"movntps %%xmm1, 16(%%edi)	\n\t"
	"movntps %%xmm2, 32(%%edi)	\n\t"
	"movntps %%xmm3, 48(%%edi)	\n\t"
	"add $64, %%esi			\n\t"
	"add $64, %%edi			\n\t"
	"dec %%ecx			\n\t"
	"jnz 1b				\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "xmm0", "xmm1", "xmm2", "xmm3");
    nBytes = nBytes % 64;
    for (; nBytes; --nBytes)
	*dest++ = *src++;
}
#endif // CPU_HAS_SSE
#endif // __i386__

template <typename CopyFunction>
void TestCopyFunction (const char* name, CopyFunction pfn)
{
    const size_t nIter = 256;
    const size_t nBytes = 1024 * 1024;
    memblock buf1 (nBytes), buf2 (nBytes);
    iota (buf1.begin(), buf1.end(), '\x1');
    fill (buf2, 0);
    clock_t first = clock();
    for (uoff_t i = 0; i < nIter; ++ i)
	(*pfn)(buf1.cdata(), nBytes, buf2.data());
    clock_t last = clock();
    last += (last == first);
    const size_t mbps = nIter * CLOCKS_PER_SEC / (last - first);
    cout << name << " transfer rate is " << mbps << " Mbps, data is ";
    if (buf1 == buf2)
	cout << "GOOD" << endl;
    else
	cout << "BAD" << endl;
    cout.flush();
}

//----------------------------------------------------------------------
// Fill functions
//----------------------------------------------------------------------

#if __i386__
void stosb_fill (const char* dest, size_t nBytes, char v)
{
    asm volatile (
	"cld		\n\t"
	"rep stosb	\n\t"
	: "=&D"(dest)
	: "0"(dest), "a"(v), "c"(nBytes)
	: "memory");
}

void stosd_fill (const char* dest, size_t nBytes, char v)
{
    uint32_t lv;
    pack_type (v, lv);
    asm volatile (
	"shr $2, %%ecx		\n\t"
	"cld			\n\t"
	"rep stosl		\n\t"
	: "=&D"(dest)
	: "0"(dest), "a"(lv), "c"(nBytes)
	: "memory");
}

void risc_fill (const char* dest, size_t nBytes, char v)
{
    uint32_t lv;
    pack_type (v, lv);
    asm volatile (
	"shr $2, %%ecx		\n\t"
	"1:			\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"add $4, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	: "=&D"(dest)
	: "0"(dest), "a"(lv), "c"(nBytes)
	: "memory");
}

void unroll_fill (const char* dest, size_t nBytes, char v)
{
    uint32_t lv;
    pack_type (v, lv);
    asm volatile (
	"shr $4, %%ecx		\n\t"
	"1:			\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"mov %%eax, 4(%%edi)	\n\t"
	"mov %%eax, 8(%%edi)	\n\t"
	"mov %%eax, 12(%%edi)	\n\t"
	"add $16, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	: "=&D"(dest)
	: "0"(dest), "a"(lv), "c"(nBytes)
	: "memory");
}

#if CPU_HAS_MMX && HAVE_INT64_T
void mmx_fill (const char* dest, size_t nBytes, char v)
{
    uint64_t lv;
    pack_type (v, lv);
    asm volatile (
	"shr $5, %%ecx		\n\t"
#if CPU_HAS_3DNOW
	"prefetchw 512(%%edi)	\n\t"
#endif
	"1:			\n\t"
#if 0
	"movntq %2, (%%edi)	\n\t"
	"movntq %2, 8(%%edi)	\n\t"
	"movntq %2, 16(%%edi)	\n\t"
	"movntq %2, 24(%%edi)	\n\t"
#else
	"movq %2, (%%edi)	\n\t"
	"movq %2, 8(%%edi)	\n\t"
	"movq %2, 16(%%edi)	\n\t"
	"movq %2, 24(%%edi)	\n\t"
#endif
	"add $32, %%esi		\n\t"
	"add $32, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	"emms"
	: "=&D"(dest)
	: "0"(dest), "y"(lv), "c"(nBytes)
	: "memory");
}
#endif // CPU_HAS_MMX
#endif // __i386__

template <typename FillFunction>
void TestFillFunction (const char* name, FillFunction pfn)
{
    const size_t nIter = 256;
    const size_t nBytes = 1024 * 1024;
    memblock buf1 (nBytes), buf2 (nBytes);
    iota (buf1.begin(), buf1.end(), '\x1');
    fill (buf2, 42);
    clock_t first = clock();
    for (uoff_t i = 0; i < nIter; ++ i)
	(*pfn)(buf1.data(), nBytes, char(42));
    clock_t last = clock();
    last += (last == first);
    const size_t mbps = nIter * CLOCKS_PER_SEC / (last - first);
    cout << name << " transfer rate is " << mbps << " Mbps, data is ";
    if (buf1 == buf2)
	cout << "GOOD" << endl;
    else
	cout << "BAD" << endl;
    cout.flush();
}

//----------------------------------------------------------------------

int main (void)
{
    cout << "Testing fill" << endl;
    cout << "---------------------------------------------------------" << endl;
    TestFillFunction ("fill_n\t\t", &fill_n<char*, char>);
#if __i386__
#if CPU_HAS_MMX && HAVE_INT64_T
    TestFillFunction ("mmx_fill\t", &mmx_fill);
#endif
    TestFillFunction ("stosb_fill\t", &stosb_fill);
    TestFillFunction ("stosd_fill\t", &stosd_fill);
    TestFillFunction ("unroll_fill\t", &unroll_fill);
    TestFillFunction ("risc_fill\t", &risc_fill);
#endif

    cout << endl;
    cout << "Testing copy" << endl;
    cout << "---------------------------------------------------------" << endl;
    TestCopyFunction ("copy_n\t\t", &copy_n<const char*, char*>);
#if __i386__
#if CPU_HAS_SSE
    TestCopyFunction ("sse_copy\t", &sse_copy);
#endif
#if CPU_HAS_MMX
    TestCopyFunction ("mmx_copy\t", &mmx_copy);
#endif
    TestCopyFunction ("movsb_copy\t", &movsb_copy);
    TestCopyFunction ("movsd_copy\t", &movsd_copy);
    TestCopyFunction ("risc_copy\t", &risc_copy);
    TestCopyFunction ("unroll_copy\t", &unroll_copy);
#endif

    return (EXIT_SUCCESS);
}

