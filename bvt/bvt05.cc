// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//

#include <ustl.h>
using namespace ustl;

void PrintVector (const vector<int>& v)
{
    cout << "{ ";
    vector<int>::const_iterator i = v.begin();
    while (i < v.end())
	cout << *i++ << ' ';
    cout << "}" << endl;
}

void printint (int i)
{
    cout << i << ' ';
}

inline bool is_even (int i)
{
    return (i % 2 == 0);
}

int sqr (int i)
{
    return (i * i);
}

int genint (void)
{
    static int counter = 0;
    return (counter++);
}

// In its own function because compilers differ in selecting const/nonconst
// members where no choice is needed.
//
void TestEqualRange (const vector<int>& v)
{
    pair<vector<int>::const_iterator,vector<int>::const_iterator> rv;
    rv = equal_range (v, 10);
    cout << "Range of 10 is { " << rv.first - v.begin()
	 << ", " << rv.second - v.begin() << " }" << endl;
    rv = equal_range (v, 0);
    cout << "Range of 0 is { " << rv.first - v.begin()
	 << ", " << rv.second - v.begin() << " }" << endl;
    rv = equal_range (v, 100);
    cout << "Range of 100 is { " << rv.first - v.begin()
    	 << ", " << rv.second - v.begin() << " }" << endl;
}

template <typename T>
void TestBigFill (const size_t size, const T magic)
{
    vector<T> vbig (size);
    fill (vbig.begin() + 1, vbig.end(), magic);		// offset to test prealignment loop
    typename vector<T>::const_iterator iMismatch;
    iMismatch = find_if (vbig.begin() + 1, vbig.end(), bind1st (not_equal_to<T>(), magic));
    if (iMismatch == vbig.end())
	cout << "works";
    else {
	cout << "does not work: mismatch at " << distance(vbig.begin(), iMismatch);
	cout << ", value = 0x" << ios::hex << uintptr_t(*iMismatch) << ios::dec;
    }
    cout << endl;
}

template <typename T>
void TestBigCopy (const size_t size, const T magic)
{
    vector<T> vbig1 (size), vbig2 (size);
    fill (vbig1, magic);
    copy (vbig1.begin() + 1, vbig1.end(), vbig2.begin() + 1);	// offset to test prealignment loop
    typedef typename vector<T>::const_iterator iter_t;
    pair<iter_t, iter_t> iMismatch;
    iMismatch = mismatch (vbig1.begin() + 1, vbig1.end(), vbig2.begin() + 1);
    if (iMismatch.first == vbig1.end())
	cout << "works";
    else {
	cout << "does not work: mismatch at " << distance(vbig1.begin(), iMismatch.first);
	cout << ios::hex << ", 0x" << uintptr_t(*iMismatch.first);
	assert (iMismatch.second < vbig2.end());
	cout << " != 0x" << uintptr_t(*iMismatch.second) << ios::dec;
    }
    cout << endl;
}

int main (void)
{
    const int c_TestNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 13, 14, 15, 16, 17, 18 };
    const int* first = c_TestNumbers;
    const int* last = first + VectorSize(c_TestNumbers);
    vector<int> v;
    v.assign (first, last);
    PrintVector (v);

    cout << "swap(1,2)" << endl;
    swap (v[0], v[1]);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy(0,8,9)" << endl;
    copy (v.begin(), v.begin() + 8, v.begin() + 9);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy with back_inserter" << endl;
    v.clear();
    copy (first, last, back_inserter(v));
    PrintVector (v);
    v.assign (first, last);

    cout << "copy with inserter" << endl;
    v.clear();
    copy (first, first + 5, inserter(v, v.begin()));
    copy (first, first + 5, inserter(v, v.begin() + 3));
    PrintVector (v);
    v.assign (first, last);

    cout << "copy_n(0,8,9)" << endl;
    copy_n (v.begin(), 8, v.begin() + 9);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy_if(is_even)" << endl;
    vector<int> v_even;
    copy_if (v, back_inserter(v_even), &is_even);
    PrintVector (v_even);
    v.assign (first, last);

    cout << "for_each(printint)" << endl;
    cout << "{ ";
    for_each (v, &printint);
    cout << "}" << endl;

    cout << "for_each(reverse_iterator, printint)" << endl;
    cout << "{ ";
    for_each (v.rbegin(), v.rend(), &printint);
    cout << "}" << endl;

    cout << "find(10)" << endl;
    cout << "10 found at offset " << find (v, 10) - v.begin() << endl;

    cout << "count(13)" << endl;
    cout << count(v,13) << " values of 13, ";
    cout << count(v,18) << " values of 18" << endl;

    cout << "transform(sqr)" << endl;
    transform (v, &sqr);
    PrintVector (v);
    v.assign (first, last);

    cout << "replace(13,666)" << endl;
    replace (v, 13, 666);
    PrintVector (v);
    v.assign (first, last);

    cout << "fill(13)" << endl;
    fill (v, 13);
    PrintVector (v);
    v.assign (first, last);

    cout << "fill_n(5, 13)" << endl;
    fill_n (v.begin(), 5, 13);
    PrintVector (v);
    v.assign (first, last);

    cout << "fill 64083 uint8_t(0x41) ";
    TestBigFill<uint8_t> (64083, 0x41);
    cout << "fill 64083 uint16_t(0x4142) ";
    TestBigFill<uint16_t> (64083, 0x4142);
    cout << "fill 64083 uint32_t(0x41424344) ";
    TestBigFill<uint32_t> (64083, 0x41424344);
#ifdef HAVE_INT64_T
    cout << "fill 64083 uint64_t(0x4142434445464748) ";
    TestBigFill<uint64_t> (64083, UINT64_C(0x4142434445464748));
#else
    cout << "No 64bit types available on this platform" << endl;
#endif

    cout << "copy 64083 uint8_t(0x41) ";
    TestBigCopy<uint8_t> (64083, 0x41);
    cout << "copy 64083 uint16_t(0x4142) ";
    TestBigCopy<uint16_t> (64083, 0x4142);
    cout << "copy 64083 uint32_t(0x41424344) ";
    TestBigCopy<uint32_t> (64083, 0x41424344);
#ifdef HAVE_INT64_T
    cout << "copy 64083 uint64_t(0x4142434445464748) ";
    TestBigCopy<uint64_t> (64083, UINT64_C(0x4142434445464748));
#else
    cout << "No 64bit types available on this platform" << endl;
#endif

    cout << "generate(genint)" << endl;
    generate (v, &genint);
    PrintVector (v);
    v.assign (first, last);

    cout << "rotate(4)" << endl;
    rotate (v, 7);
    rotate (v, -3);
    PrintVector (v);
    v.assign (first, last);

    cout << "merge with (3,5,10,11,11,14)" << endl;
    const int c_MergeWith[] = { 3,5,10,11,11,14 };
    vector<int> vmerged;
    merge (v.begin(), v.end(), c_MergeWith, c_MergeWith + VectorSize(c_MergeWith), back_inserter(vmerged));
    PrintVector (vmerged);
    v.assign (first, last);

    cout << "inplace_merge with (3,5,10,11,11,14)" << endl;
    v.insert (v.end(), c_MergeWith, c_MergeWith + VectorSize(c_MergeWith));
    inplace_merge (v.begin(), v.end() - VectorSize(c_MergeWith), v.end());
    PrintVector (v);
    v.assign (first, last);

    cout << "remove(13)" << endl;
    remove (v, 13);
    PrintVector (v);
    v.assign (first, last);

    cout << "remove (elements 3, 4, 6, 15, and 45)" << endl;
    vector<uoff_t> toRemove;
    toRemove.push_back (3);
    toRemove.push_back (4);
    toRemove.push_back (6);
    toRemove.push_back (15);
    toRemove.push_back (45);
    typedef index_iterate<vector<int>::iterator, vector<uoff_t>::iterator> riiter_t;
    riiter_t rfirst = index_iterator (v.begin(), toRemove.begin());
    riiter_t rlast = index_iterator (v.begin(), toRemove.end());
    remove (v, rfirst, rlast);
    PrintVector (v);
    v.assign (first, last);

    cout << "unique" << endl;
    unique (v);
    PrintVector (v);
    v.assign (first, last);

    cout << "reverse" << endl;
    reverse (v);
    PrintVector (v);
    v.assign (first, last);

    cout << "lower_bound(10)" << endl;
    PrintVector (v);
    cout << "10 begins at position " << lower_bound (v, 10) - v.begin() << endl;
    v.assign (first, last);

    cout << "upper_bound(10)" << endl;
    PrintVector (v);
    cout << "10 ends at position " << upper_bound (v, 10) - v.begin() << endl;
    v.assign (first, last);

    cout << "equal_range(10)" << endl;
    PrintVector (v);
    TestEqualRange (v);
    v.assign (first, last);

    cout << "sort" << endl;
    reverse (v);
    PrintVector (v);
    random_shuffle (v);
    sort (v);
    PrintVector (v);
    v.assign (first, last);

    return (EXIT_SUCCESS);
}

