#include <ustl.h>
using namespace ustl;

void movsb (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"cld		\n\t"
	"rep movsb	\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

void movsd (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $2, %%ecx	\n\t"
	"cld		\n\t"
	"rep movsd	\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory");
}

void risc (const char* src, size_t nBytes, char* dest)
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

void unrolled (const char* src, size_t nBytes, char* dest)
{
    asm volatile (
	"shr $4, %%ecx		\n\t"
	"1:			\n\t"
	"mov (%%esi), %%eax	\n\t"
	"mov 4(%%esi), %%ebx	\n\t"
	"mov %%eax, (%%edi)	\n\t"
	"mov %%ebx, 4(%%edi)	\n\t"
	"mov 8(%%esi), %%eax	\n\t"
	"mov 12(%%esi), %%ebx	\n\t"
	"mov %%eax, 8(%%edi)	\n\t"
	"mov %%ebx, 12(%%edi)	\n\t"
	"add $16, %%esi		\n\t"
	"add $16, %%edi		\n\t"
	"dec %%ecx		\n\t"
	"jnz 1b			\n\t"
	: "=&S"(src), "=&D"(dest)
	: "0"(src), "1"(dest), "c"(nBytes)
	: "memory", "eax", "ebx");
}

#if CPU_HAS_MMX
void mmx (const char* src, size_t nBytes, char* dest)
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
#if CPU_HAS_SSE
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
#endif

#if CPU_HAS_SSE
void sse (const char* src, size_t nBytes, char* dest)
{
    for (; nBytes && (uintptr_t(src) % 16 || uintptr_t(dest) % 16); --nBytes)
	*dest++ = *src++;
    asm volatile (
	"mov %0, %%esi			\n\t"
	"mov %1, %%edi			\n\t"
	"mov %2, %%ecx			\n\t"
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
	:
	: "m"(src), "m"(dest), "m"(nBytes)
	: "memory", "xmm0", "xmm1", "xmm2", "xmm3", "esi", "edi", "ecx");
    const size_t bw = (nBytes / 64) * 64;
    src += bw;
    dest += bw;
    nBytes = nBytes % 64;
    for (; nBytes; --nBytes)
	*dest++ = *src++;
}
#endif

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

int main (void)
{
#if CPU_HAS_SSE
    TestCopyFunction ("sse", &sse);
#endif
#if CPU_HAS_MMX
    TestCopyFunction ("mmx", &mmx);
#endif
    TestCopyFunction ("copy_n", &copy_n<const char*, char*>);
    TestCopyFunction ("movsb", &movsb);
    TestCopyFunction ("movsd", &movsd);
    TestCopyFunction ("risc", &risc);
    TestCopyFunction ("unrolled", &unrolled);
    TestFillFunction ("fill_n", &fill_n<char*, char>);
    return (0);
}

