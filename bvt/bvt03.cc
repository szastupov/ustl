#include <ustl.h>
using namespace ustl;

int main (void)
{
    const u_char magic_Char = 0x12;
    const u_short magic_Short = 0x1234;
    const u_long magic_Int = 0x12345678;
    const float magic_Float = 0.12345678;
    const double magic_Double = 0.123456789123456789;
    const bool magic_Bool = true;

    char c = magic_Char;
    u_char uc = magic_Char;
    int i = magic_Int;
    short si = magic_Short;
    long li = magic_Int;
    u_int ui = magic_Int;
    u_short usi = magic_Short;
    u_long uli = magic_Int;
    float f = magic_Float;
    double d = magic_Double;
    bool bv = magic_Bool;

    memblock b;
    b.resize (64);
    b.fill (b.begin(), "\xCD", 1, 64);
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
    b.resize (os.pos());
    cout << os.pos() << " bytes written" << endl;

    c = 0;
    uc = 0;
    bv = false;
    i = ui = li = uli = 0;
    f = 0; d = 0;
    si = usi = 0;

    istream is (b);
    is >> c;
    is >> uc;
    is >> ios::align(sizeof(bool)) >> bv;
    is >> ios::align(sizeof(int)) >> i;
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
    const u_char* pc = reinterpret_cast<const u_char*>(b.cdata());
    const u_char* pcEnd = pc + b.size();
    const size_t c_nCharsPerLine = 8;
    const u_char* pcNL = pc + c_nCharsPerLine;
    while (pc < pcEnd) {
	if (pc == pcNL) {
	    cout << "" << endl;
	    pcNL += c_nCharsPerLine;
	}
	cout.format ("%02X ", *pc++);
    }
    cout << endl;

    return (0);
}

