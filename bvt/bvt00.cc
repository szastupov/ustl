#include <cmemlink.h>
#include <stdlib.h>
#include <memory.h>
#include <fdostream.h>
#include <mistream.h>
#include <uios.h>
using namespace ustl;

void WriteCML (const cmemlink& l)
{
    cout << "cmemlink{" << l.size() << "}: ";
    const void* pv = l;
    const char* pc = reinterpret_cast<const char*>(pv);
    size_t nc = l.size();
    if (pc[nc - 1] == 0)
	-- nc;
    cout.write (l, nc);
    cout << endl;
}

void TestCML (void)
{
    const size_t strTestLen = 13;
    const char strTest[strTestLen] = "Hello world!";

    cmemlink a, b;
    a.link (strTest, strTestLen);
    if (a.begin() != strTest)
	cout << "CData() failed on cmemlink" << endl;
    if (a.begin() + 5 != &strTest[5])
	cout << "CData() failed on cmemlink" << endl;
    if (0 != memcmp (a, strTest, strTestLen))
	cout << "memcmp failed on cmemlink" << endl;
    b.link (strTest, strTestLen);
    WriteCML (a);
    WriteCML (b);
    if (!(a == b))
	cout << "operator== failed on cmemlink" << endl;
    b.resize (strTestLen - 5);
    a = b;
    WriteCML (a);
}

int main (void)
{
    TestCML();
    return (0);
}

