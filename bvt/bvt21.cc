// Tests functions from uutility.h
//

#include <ustring.h>
#include <fdostream.h>
#include <uios.h>
using namespace ustl;

int main (void)
{
    cout << "DivRU(13,5) = " << DivRU(13,5) << endl;
    cout << "DivRU(15,5) = " << DivRU(15,5) << endl;
    cout << "DivRU(-12,5) = " << DivRU(-12,5) << endl;
    cout << endl;
    cout << "Align(5) = " << Align(5) << endl;
    cout << "Align(5,2) = " << Align(5,2) << endl;
    cout << "Align(17,7) = " << Align(17,7) << endl;
    cout << "Align(14,7) = " << Align(14,7) << endl;
    cout << endl;
    cout << "advance(42,0) = " << advance(42,0) << endl;
    cout << "advance(42,3) = " << advance(42,3) << endl;
    const void *cvp = (const void*) 0x1234;
    void* vp = (void*) 0x4321;
    cout << ios::hex;
    cout << "cvp = " << cvp << endl;
    cout << "vp = " << vp << endl;
    cout << "advance(cvp,5) = " << advance(cvp,5) << endl;
    cout << "advance(vp,4) = " << advance(vp,4) << endl;
    cout << "distance(cvp,vp) = " << distance(cvp,vp) << endl;
    cout << "distance(vp,cvp) = " << distance(vp,cvp) << endl;
    cout << ios::dec << endl;
    const int c_Numbers[] = { 1, 2, 3, 4, 5 };
    const int c_Empty[] = { };
    cout << "VectorSize(c_Numbers[5]) = " << VectorSize(c_Numbers) << endl;
    #ifdef __GNUC__
	cout << "VectorSize(c_Numbers[0]) = " << VectorSize(c_Empty) << endl;
    #endif
    cout << endl;
    cout << "BitsInType(uint32_t) = " << BitsInType(uint32_t) << endl;
    cout << "BitsInType(int16_t) = " << BitsInType(int16_t) << endl;
    cout << "BitsInType(char) = " << BitsInType(char) << endl;
    cout << ios::hex << endl;
    cout << "BitMask(uint32_t,12) = " << BitMask(uint32_t,12) << endl;
    cout << "BitMask(uint16_t,1) = " << BitMask(uint16_t,1) << endl;
    cout << "BitMask(uint8_t,8) = " << BitMask(uint8_t,8) << endl;
    cout << "BitMask(uint16_t,0) = " << BitMask(uint16_t,0) << endl;
    cout << endl;
    #ifdef HAVE_BYTESWAP_H
	uint16_t v1 = 0x1234;
	cout << "byteswap.h exists." << endl;
	cout << "bswap(" << v1 << ") = " << bswap(v1) << endl;
	cout << "le_to_native(" << v1 << ") = " << le_to_native(v1) << endl;
	cout << "be_to_native(" << v1 << ") = " << be_to_native(v1) << endl;
	cout << "native_to_le(" << v1 << ") = " << native_to_le(v1) << endl;
	cout << "native_to_be(" << v1 << ") = " << native_to_be(v1) << endl;
	uint32_t v2 = 0x12345678;
	cout << "bswap(" << v2 << ") = " << bswap(v2) << endl;
	cout << "le_to_native(" << v2 << ") = " << le_to_native(v2) << endl;
	cout << "be_to_native(" << v2 << ") = " << be_to_native(v2) << endl;
	cout << "native_to_le(" << v2 << ") = " << native_to_le(v2) << endl;
	cout << "native_to_be(" << v2 << ") = " << native_to_be(v2) << endl;
	#ifdef bswap_64
	    uint64_t v3 = 0x123456789ABCDEF0ULL;
	    cout << "bswap(" << v3 << ") = " << bswap(v3) << endl;
	    cout << "le_to_native(" << v3 << ") = " << le_to_native(v3) << endl;
	    cout << "be_to_native(" << v3 << ") = " << be_to_native(v3) << endl;
	    cout << "native_to_le(" << v3 << ") = " << native_to_le(v3) << endl;
	    cout << "native_to_be(" << v3 << ") = " << native_to_be(v3) << endl;
	#endif
    #else
	cout << "byteswap.h does not exist." << endl;
    #endif
    cout << ios::dec << endl;
    cout << "min(3,4) = " << min(3,4) << endl;
    cout << "min(6U,1U) = " << min(6U,1U) << endl;
    cout << "max(-3,-6) = " << max(-3,-6) << endl;
    cout << "max(-3L,6L) = " << max(-3L,6L) << endl;
    return (0);
}

