#include <ustl.h>
using namespace ustl;

void WriteCML (const cmemlink& l)
{
    cout << "cmemlink{" << l.size() << "}: ";
    const void* pv = l.cdata();
    const char* pc = reinterpret_cast<const char*>(pv);
    size_t nc = l.size();
    if (pc[nc - 1] == 0)
	-- nc;
    cout.write (l.begin(), nc);
    cout << endl;
}

void TestCML (void)
{
    const size_t strTestLen = 13;
    const char strTest[strTestLen] = "Hello world!";
    const char* pstrTest = strTest; // const storage is sometimes copied on pointing

    cmemlink a, b;
    a.link (pstrTest, strTestLen);
    if (a.begin() != pstrTest) {
	cout << "a.begin() failed: " << ios::hex << uintptr_t(a.begin());
        cout << " != " << uintptr_t(pstrTest) << ios::dec << endl;
    }
    a.link (pstrTest, pstrTest + strTestLen);
    if (*(const char*)(a.begin() + 5) != strTest[5]) {
	cout << "begin()[5] failed: " << *(const char*)(a.begin() + 5);
	cout << " != " << strTest[5] << endl;
    }
    if (0 != memcmp (a.begin(), strTest, strTestLen))
	cout << "memcmp failed on cmemlink" << endl;
    b.link (pstrTest, strTestLen);
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
    return (EXIT_SUCCESS);
}

