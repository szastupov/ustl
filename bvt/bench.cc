// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include <ustl.h>
#include <time.h>
using namespace ustl;

//----------------------------------------------------------------------
// Copy functions
//----------------------------------------------------------------------

#if __i386__
extern "C" void movsb_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"cld		\n\t"
	"rep movsb"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

extern "C" void movsd_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $2, %%ecx	\n\t"
	"cld		\n\t"
	"rep movsl"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

extern "C" void risc_copy (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $2, %%ecx		\n\t"
	"1:			\n\t"
	"mov (%%esi), %%eax	\n\t"
	"add $4, %%esi		\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"add $4, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "eax");
}

extern "C" void unroll_copy (const char* src, size_t nBytes, char* dest)
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
	"jnz 1b"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "eax", "edx");
}

#if CPU_HAS_MMX
extern "C" void mmx_copy (const char* src, size_t nBytes, char* dest)
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
	"movq %%mm0, (%%edi)	\n\t"
	"movq %%mm1, 8(%%edi)	\n\t"
	"movq %%mm2, 16(%%edi)	\n\t"
	"movq %%mm3, 24(%%edi)	\n\t"
	"add $32, %%esi		\n\t"
	"add $32, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "mm0", "mm1", "mm2", "mm3");
    simd::reset_mmx();
}
#endif // CPU_HAS_MMX

#if CPU_HAS_SSE
extern "C" void sse_copy (const char* src, size_t nBytes, char* dest)
{
    const size_t nHeadBytes = min (nBytes, Align(uintptr_t(src), 16U) - uintptr_t(src));
    for (uoff_t i = 0; i < nHeadBytes; ++ i)
	*dest++ = *src++;
    nBytes -= nHeadBytes;
    if (!(uintptr_t(dest) % 16)) {
	const size_t nMiddleBlocks = nBytes / 32;
	for (uoff_t i = 0; i < nMiddleBlocks; ++ i) {
	    prefetch (src + 512, 0, 0);
	    asm volatile (
		"movaps (%%esi), %%xmm0		\n\t"
		"movaps 16(%%esi), %%xmm1	\n\t"
		"movntps %%xmm0, (%%edi)	\n\t"
		"movntps %%xmm1, 16(%%edi)"
		: : "S"(src), "D"(dest)
		: "memory", "xmm0", "xmm1", "xmm2", "xmm3");
	    src += 32;
	    dest += 32;
	}
	nBytes %= 32;
    }
    for (uoff_t i = 0; i < nBytes; ++ i)
	*dest++ = *src++;
}
#endif // CPU_HAS_SSE
#endif // __i386__

extern "C" void memcpy_copy (const char* src, size_t nBytes, char* dest)
{
    memcpy (dest, src, nBytes);
}

template <typename CopyFunction>
void TestCopyFunction (const char* name, CopyFunction pfn)
{
    const uoff_t misalignment = 0;
    const uoff_t headBytes = 0;
    const uoff_t tailBytes = 0;

    const size_t nIter = 128;
    const size_t nBytes = 1024 * 1024 + misalignment;

    string buf1 (nBytes), buf2 (nBytes);
    iota (buf1.begin(), buf1.end(), '\x1');
    fill (buf2, 0);
    const clock_t first = clock();
    for (uoff_t i = 0; i < nIter; ++ i)
	(*pfn)(buf1.cdata() + headBytes, nBytes - headBytes - tailBytes, buf2.data() + headBytes + misalignment);
    clock_t last = clock();
    last += (last == first);
    const size_t mbps = nIter * CLOCKS_PER_SEC / (last - first);
    cout << name << " transfer rate is " << mbps << " Mbps, data is ";
    size_t nBad = 0;
    for (uoff_t i = headBytes; i < buf1.size() - tailBytes; ++ i)
	nBad += (buf1[i] != buf2[i + misalignment]);
    if (!nBad)
	cout << "GOOD" << endl;
    else {
	cout << "BAD" << endl;
	for (uoff_t i = headBytes; i < buf1.size() - tailBytes; ++ i)
	    if (buf1[i] != buf2[i + misalignment])
		cout << "\t\t" << i << "\tbuf1: " << (int) buf1[i] << ", buf2: " << (int) buf2[i + misalignment] << endl;
    }
    cout.flush();
}

//----------------------------------------------------------------------
// Fill functions
//----------------------------------------------------------------------

#if __i386__
extern "C" void stosb_fill (const char* dest, size_t nBytes, char v)
{
    asm volatile (
	"cld		\n\t"
	"rep stosb	\n\t"
	: "=&D"(dest)
	: "0"(dest), "a"(v), "c"(nBytes)
	: "memory");
}

extern "C" void stosd_fill (const char* dest, size_t nBytes, char v)
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

extern "C" void risc_fill (const char* dest, size_t nBytes, char v)
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

extern "C" void unroll_fill (const char* dest, size_t nBytes, char v)
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
extern "C" void mmx_fill (const char* dest, size_t nBytes, char v)
{
    uint64_t lv;
    pack_type (v, lv);
    asm volatile (
	"shr $5, %%ecx		\n\t"
#if CPU_HAS_3DNOW
	"prefetchw 512(%%edi)	\n\t"
#endif
	"1:			\n\t"
	"movq %2, (%%edi)	\n\t"
	"movq %2, 8(%%edi)	\n\t"
	"movq %2, 16(%%edi)	\n\t"
	"movq %2, 24(%%edi)	\n\t"
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
    string buf1 (nBytes), buf2 (nBytes);
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
    TestCopyFunction ("memcpy_copy\t", &memcpy_copy);

    return (EXIT_SUCCESS);
}

