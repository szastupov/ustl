// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

void PrintVector (const vector<int>& v)
{
    cout << "{ ";
    vector<int>::const_iterator i = v.begin();
    while (i < v.end())
	cout << *i++ << ' ';
    cout << "}" << endl;
}

class A {
public:
    A (void)
    { cout << "A::A" << endl; }
    A (const A&)
    { cout << "Copy A::A" << endl; }
    const A& operator= (const A&)
    { cout << "A::operator=" << endl; return (*this); }
    ~A (void)
    { cout << "A::~A" << endl; }
};

NOT_STREAMABLE(A)

void TestVector (void)
{
    const int c_TestNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 14, 15, 16, 17, 18 };
    vector<int> v;
    v.push_back (1);
    PrintVector (v);
    v.reserve (20);
    cout << "Reserved to capacity() == " << v.capacity();
    cout << " (" << v.size() << " used, ";
    if (v.max_size() == SIZE_MAX / sizeof(int))
	cout << "SIZE_MAX/elsize";
    else
	cout << v.max_size();
    cout << " max)" << endl;
    v.insert (v.begin() + 1, 1 + VectorRange(c_TestNumbers));
    PrintVector (v);
    cout.format ("front() = %d, back() = %d\n", v.front(), v.back());
    v.erase (v.begin());
    v.pop_back();
    PrintVector (v);
    v.insert (v.begin() + 10, 3, 666);
    v.at(5) = 777;
    PrintVector (v);
    v.resize (v.size() - 5);
    if (v.empty())
	cout << "v is now empty" << endl;
    PrintVector (v);
    cout << "v[5] == " << v[5] << endl;
    v.clear();
    if (v.empty())
	cout << "v is now empty" << endl;
    vector<int> v2 (20, 66);
    PrintVector (v2);
    v2.assign (20, 33);
    PrintVector (v2);
    v.assign (VectorRange (c_TestNumbers));
    PrintVector (v);
    if (v == v2)
	cout << "v == v2" << endl;
    v2 = v;
    if (v == v2)
	cout << "v == v2" << endl;
    vector<A> ctv;
    A a;
    ctv.assign (3, a);
    ctv.pop_back();
    cout << "Class insertion testing successful" << endl;
    cout.flush();
}

StdBvtMain (TestVector)

