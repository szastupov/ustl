// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

void TestStreams (void)
{
    const uint8_t magic_Char = 0x12;
    const uint16_t magic_Short = 0x1234;
    const uint32_t magic_Int = 0x12345678;
    const float magic_Float = 0.12345678;
    const double magic_Double = 0.123456789123456789;
    const bool magic_Bool = true;

    char c = magic_Char;
    unsigned char uc = magic_Char;
    int i = magic_Int;
    short si = magic_Short;
    long li = magic_Int;
    unsigned int ui = magic_Int;
    unsigned short usi = magic_Short;
    unsigned long uli = magic_Int;
    float f = magic_Float;
    double d = magic_Double;
    bool bv = magic_Bool;

    size_t totalSize = stream_size_of(c);
    totalSize += stream_size_of(uc);
    totalSize = Align (totalSize, alignof(bv));
    totalSize += stream_size_of(bv);
    totalSize = Align (totalSize, alignof(i));
    totalSize += stream_size_of(i);
    totalSize += stream_size_of(ui);
    totalSize = Align (totalSize);
    totalSize += stream_size_of(li);
    totalSize += stream_size_of(uli);
    totalSize = Align (totalSize, alignof(f));
    totalSize += stream_size_of(f);
    totalSize = Align (totalSize, alignof(d));
    totalSize += stream_size_of(d);
    totalSize += stream_size_of(si);
    totalSize += stream_size_of(usi);

    memblock b;
    b.resize (totalSize);
    b.fill (b.begin(), "\xCD", 1, b.size());
    ostream os (b);

    os << c;
    os << uc;
    os << ios::talign<bool>() << bv;
    os << ios::talign<int>() << i;
    os << ui;
    os << ios::align() << li;
    os << uli;
    os << ios::talign<float>() << f;
    os << ios::talign<double>() << d;
    os << si;
    os << usi;
    if (b.size() == os.pos())
	cout << "Correct";
    else
	cout << "Incorrect (" << os.pos() << " of " << b.size() << ')';
    cout << " number of bytes written" << endl;
    cout.flush();

    c = 0;
    uc = 0;
    bv = false;
    i = ui = li = uli = 0;
    f = 0; d = 0;
    si = usi = 0;

    istream is (b);
    is >> c;
    is >> uc;
    is >> ios::talign<bool>() >> bv;
    is >> ios::talign<int>() >> i;
    is >> ui;
    is >> ios::align() >> li;
    is >> uli;
    is >> ios::talign<float>() >> f;
    is >> ios::talign<double>() >> d;
    is >> si;
    is >> usi;
    if (is.pos() != b.size())
	cout << "Positional error" << endl;

    cout << "Values:" << endl;
    cout.format ("char:    0x%02X\n", static_cast<int>(c));
    cout.format ("u_char:  0x%02X\n", static_cast<int>(uc));
    cout.format ("bool:    %d\n", static_cast<int>(bv));
    cout.format ("int:     0x%08X\n", i);
    cout.format ("u_int:   0x%08X\n", ui);
    cout.format ("long:    0x%08lX\n", li);
    cout.format ("u_long:  0x%08lX\n", uli);
    cout.format ("float:   %.8f\n", f);
    cout.format ("double:  %.16f\n", d);
    cout.format ("short:   0x%04X\n", static_cast<int>(si));
    cout.format ("u_short: 0x%04X\n", static_cast<int>(usi));
    cout << endl;

    cout << "Binary dump:" << endl;
    const unsigned char* pc = reinterpret_cast<const unsigned char*>(b.cdata());
    const unsigned char* pcEnd = pc + b.size();
    const size_t c_nCharsPerLine = 8;
    const unsigned char* pcNL = pc + c_nCharsPerLine;
    while (pc < pcEnd) {
	if (pc == pcNL) {
	    cout << "" << endl;
	    pcNL += c_nCharsPerLine;
	}
	cout.format ("%02X ", *pc++);
    }
    cout << endl;
}

StdBvtMain (TestStreams)

