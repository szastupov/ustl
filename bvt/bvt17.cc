#include <ustl.h>
using namespace ustl;

int main (void)
{
    bitset<30> bs1;
    cout << "bitset<" << bs1.size() << "> bs1: capacity() = " << bs1.capacity() << ", sizeof() = " << sizeof(bs1) << endl;
    cout << bs1 << endl;
    bs1.set();
    bs1.set (6, false);
    cout << bs1 << endl;
    bs1.flip();
    cout << bs1 << endl;
    bs1.flip();
    cout << bs1 << endl;
    bs1.reset();
    cout << bs1 << endl;
    bs1.set (4);
    bs1.set (7);
    bs1.set (8);
    cout << bs1 << endl;
    cout << "test(7) == " << bs1.test(7);
    cout << ", [9] = " << bs1[9];
    cout << ", [8] = " << bs1[8] << endl;
    cout << "~bs1 == " << ~bs1 << endl;
    cout << "to_value == 0x" << ios::hex << bs1.to_value() << ios::dec << endl;

    bitset<70> bs2 ("0101101");
    cout << "bitset<" << bs2.size() << "> bs2: capacity() = " << bs2.capacity() << ", sizeof() = " << sizeof(bs2) << endl;
    cout << bs2 << endl;

    bitset<256> bs3 (0x3030);
    cout << "bitset<" << bs3.size() << "> bs3: capacity() = " << bs3.capacity() << ", sizeof() = " << sizeof(bs3) << endl;
    cout << "bs3.to_value() == 0x" << ios::hex << bs3.to_value() << ios::dec << endl;

    bitset<30> bs4 (bs1);
    if (bs1 == bs4)
	cout << "bs4 == bs1" << endl;

    return (0);
}

