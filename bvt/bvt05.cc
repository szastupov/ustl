// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

typedef vector<int>			intvec_t;
typedef const intvec_t&			rcintvec_t;
typedef intvec_t::const_iterator	intiter_t;

static void printint (int i)
{
    cout.format ("%d ", i);
}

static void PrintVector (rcintvec_t v)
{
    cout << "{ ";
    foreach (intiter_t, i, v)
	printint (*i);
    cout << "}\n";
}

static bool is_even (int i)
{
    return (i % 2 == 0);
}

static int sqr (int i)
{
    return (i * i);
}

static int genint (void)
{
    static int counter = 0;
    return (counter++);
}

// In its own function because compilers differ in selecting const/nonconst
// members where no choice is needed.
//
static void TestEqualRange (rcintvec_t v)
{
    pair<intiter_t,intiter_t> rv;
    rv = equal_range (v, 10);
    cout.format ("Range of  10 is { %2zd, %2zd }\n", rv.first - v.begin(), rv.second - v.begin());
    rv = equal_range (v, 0);
    cout.format ("Range of   0 is { %2zd, %2zd }\n", rv.first - v.begin(), rv.second - v.begin());
    rv = equal_range (v, 100);
    cout.format ("Range of 100 is { %2zd, %2zd }\n", rv.first - v.begin(), rv.second - v.begin());
}

template <typename T>
void TestBigFill (const size_t size, const T magic)
{
    vector<T> vbig (size);
    fill (vbig.begin() + 1, vbig.end(), magic);		// offset to test prealignment loop
    typename vector<T>::const_iterator iMismatch;
    iMismatch = find_if (vbig.begin() + 1, vbig.end(), bind1st (not_equal_to<T>(), magic));
    if (iMismatch == vbig.end())
	cout << "works\n";
    else
	cout.format ("does not work: mismatch at %zu, =0x%lX\n", distance(vbig.begin(), iMismatch), (unsigned long)(*iMismatch));
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
    assert (iMismatch.second <= vbig2.end());
    if (iMismatch.first == vbig1.end())
	cout << "works\n";
    else
	cout.format ("does not work: mismatch at %zu, 0x%lX != 0x%lX\n", distance(vbig1.begin(), iMismatch.first), (unsigned long)(*iMismatch.first), (unsigned long)(*iMismatch.second));
}

static void TestAlgorithms (void)
{
    const int c_TestNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 13, 14, 15, 16, 17, 18 };
    const int* first = c_TestNumbers;
    const int* last = first + VectorSize(c_TestNumbers);
    intvec_t v;
    v.assign (first, last);
    PrintVector (v);

    cout << "swap(1,2)\n";
    swap (v[0], v[1]);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy(0,8,9)\n";
    copy (v.begin(), v.begin() + 8, v.begin() + 9);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy with back_inserter\n";
    v.clear();
    copy (first, last, back_inserter(v));
    PrintVector (v);
    v.assign (first, last);

    cout << "copy with inserter\n";
    v.clear();
    copy (first, first + 5, inserter(v, v.begin()));
    copy (first, first + 5, inserter(v, v.begin() + 3));
    PrintVector (v);
    v.assign (first, last);

    cout << "copy_n(0,8,9)\n";
    copy_n (v.begin(), 8, v.begin() + 9);
    PrintVector (v);
    v.assign (first, last);

    cout << "copy_if(is_even)\n";
    intvec_t v_even;
    copy_if (v, back_inserter(v_even), &is_even);
    PrintVector (v_even);
    v.assign (first, last);

    cout << "for_each(printint)\n{ ";
    for_each (v, &printint);
    cout << "}\n";

    cout << "for_each(reverse_iterator, printint)\n{ ";
    for_each (v.rbegin(), v.rend(), &printint);
    cout << "}\n";

    cout << "find(10)\n";
    cout.format ("10 found at offset %zu\n", find (v, 10) - v.begin());

    cout << "count(13)\n";
    cout.format ("%zu values of 13, %zu values of 18\n", count(v,13), count(v,18));

    cout << "transform(sqr)\n";
    transform (v, &sqr);
    PrintVector (v);
    v.assign (first, last);

    cout << "replace(13,666)\n";
    replace (v, 13, 666);
    PrintVector (v);
    v.assign (first, last);

    cout << "fill(13)\n";
    fill (v, 13);
    PrintVector (v);
    v.assign (first, last);

    cout << "fill_n(5, 13)\n";
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
    cout << "No 64bit types available on this platform\n";
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
    cout << "No 64bit types available on this platform\n";
#endif

    cout << "generate(genint)\n";
    generate (v, &genint);
    PrintVector (v);
    v.assign (first, last);

    cout << "rotate(4)\n";
    rotate (v, 7);
    rotate (v, -3);
    PrintVector (v);
    v.assign (first, last);

    cout << "merge with (3,5,10,11,11,14)\n";
    const int c_MergeWith[] = { 3,5,10,11,11,14 };
    intvec_t vmerged;
    merge (v.begin(), v.end(), VectorRange(c_MergeWith), back_inserter(vmerged));
    PrintVector (vmerged);
    v.assign (first, last);

    cout << "inplace_merge with (3,5,10,11,11,14)\n";
    v.insert (v.end(), VectorRange(c_MergeWith));
    inplace_merge (v.begin(), v.end() - VectorSize(c_MergeWith), v.end());
    PrintVector (v);
    v.assign (first, last);

    cout << "remove(13)\n";
    remove (v, 13);
    PrintVector (v);
    v.assign (first, last);

    cout << "remove (elements 3, 4, 6, 15, and 45)\n";
    vector<uoff_t> toRemove;
    toRemove.push_back (3);
    toRemove.push_back (4);
    toRemove.push_back (6);
    toRemove.push_back (15);
    toRemove.push_back (45);
    typedef index_iterate<intvec_t::iterator, vector<uoff_t>::iterator> riiter_t;
    riiter_t rfirst = index_iterator (v.begin(), toRemove.begin());
    riiter_t rlast = index_iterator (v.begin(), toRemove.end());
    remove (v, rfirst, rlast);
    PrintVector (v);
    v.assign (first, last);

    cout << "unique\n";
    unique (v);
    PrintVector (v);
    v.assign (first, last);

    cout << "reverse\n";
    reverse (v);
    PrintVector (v);
    v.assign (first, last);

    cout << "lower_bound(10)\n";
    PrintVector (v);
    cout.format ("10 begins at position %zu\n", lower_bound (v, 10) - v.begin());
    v.assign (first, last);

    cout << "upper_bound(10)\n";
    PrintVector (v);
    cout.format ("10 ends at position %zu\n", upper_bound (v, 10) - v.begin());
    v.assign (first, last);

    cout << "equal_range(10)\n";
    PrintVector (v);
    TestEqualRange (v);
    v.assign (first, last);

    cout << "sort\n";
    reverse (v);
    PrintVector (v);
    random_shuffle (v);
    sort (v);
    PrintVector (v);
    v.assign (first, last);

    cout << "stable_sort\n";
    reverse (v);
    PrintVector (v);
    random_shuffle (v);
    stable_sort (v);
    PrintVector (v);
    v.assign (first, last);
}

StdBvtMain (TestAlgorithms)

