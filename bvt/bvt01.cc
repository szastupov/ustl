// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//

#include <ustl.h>
using namespace ustl;

void WriteCML (const cmemlink& l)
{
    cout << "memlink{" << l.size() << "}: ";
    const char* pc = reinterpret_cast<const char*>(l.cdata());
    size_t nc = l.size();
    if (pc[nc - 1] == 0)
	-- nc;
    cout.write (l.cdata(), nc);
    cout << endl;
}

void TestML (void)
{
    memlink::value_type strTest[] = "abcdefghijklmnopqrstuvwzyz";
    const size_t strTestLen = strlen((const char*) strTest);
    memlink::const_pointer cstrTest = strTest;

    memlink a, b;
    a.link (strTest, strTestLen);
    if (a.begin() != strTest)
	cout << "begin() failed on memlink" << endl;
    a.link (strTest, strTest + strTestLen);
    if (a.begin() + 5 != &strTest[5])
	cout << "begin() + 5 failed on memlink" << endl;
    if (0 != memcmp (a.begin(), strTest, strTestLen))
	cout << "memcmp failed on memlink" << endl;
    WriteCML (a);
    b.link (cstrTest, strTestLen);
    if (b.begin() != NULL)
	cout << "begin() of const failed on cmemlink" << endl;
    if (b.cmemlink::begin() != cstrTest)
	cout << "begin() failed on cmemlink" << endl;
    WriteCML (b);
    if (!(a == b))
	cout << "operator== failed on cmemlink" << endl;
    b.resize (strTestLen - 2);
    a = b;
    if (a.begin() != NULL)
	cout << "begin() after assignment failed on cmemlink" << endl;
    a.link (strTest, strTestLen);
    WriteCML (a);
    a.insert (a.begin() + 5, 9);
    a.fill (a.begin() + 5, "-", 1, 9);
    WriteCML (a);
    a.erase (a.begin() + 9, 7);
    a.fill (a.end() - 7, "=", 1, 7);
    WriteCML (a);
    a.fill (a.begin() + 5, "TEST", 4, 3); 
    WriteCML (a);
    a.copy (cstrTest, strTestLen);
    WriteCML (a);
}

int main (void)
{
    TestML();
    return (EXIT_SUCCESS);
}

