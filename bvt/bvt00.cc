// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

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
    const char hello[] = "Hello world!";
    const char* phello = hello; // const storage is sometimes copied on pointing

    cmemlink a, b;
    a.link (phello, VectorSize(hello));
    if (a.begin() != phello) {
	cout << "a.begin() failed: " << ios::hex << uintptr_t(a.begin());
        cout << " != " << uintptr_t(phello) << ios::dec << endl;
    }
    a.link (VectorRange (hello));
    if (*(const char*)(a.begin() + 5) != hello[5]) {
	cout << "begin()[5] failed: " << *(const char*)(a.begin() + 5);
	cout << " != " << hello[5] << endl;
    }
    if (0 != memcmp (a.begin(), hello, VectorSize(hello)))
	cout << "memcmp failed on cmemlink" << endl;
    b.static_link (hello);
    WriteCML (a);
    WriteCML (b);
    if (!(a == b))
	cout << "operator== failed on cmemlink" << endl;
    b.resize (VectorSize(hello) - 5);
    a = b;
    WriteCML (a);
}

StdBvtMain (TestCML)

