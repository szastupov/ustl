// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//
// uspecial.h
//
// Template specializations for uSTL classes.
//

#ifndef USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0
#define USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0

#include "uvector.h"
#include "ustring.h"
#include "uset.h"
#include "umultiset.h"
#include "ubitset.h"
#include "ulaalgo.h"
#include "uctralgo.h"
#include "ufunction.h"
#include "mistream.h"
#include "mostream.h"
#include "sostream.h"
#include "strmsize.h"
#if !defined(WITHOUT_LIBSTDCPP) && defined(__GNUC__) && (__GNUC__ >= 3)
    #include <locale>
#else
    #include <ctype.h>
#endif

namespace ustl {

//----------------------------------------------------------------------
// Alogrithm specializations not in use by the library code.
//----------------------------------------------------------------------

template <> inline void swap (cmemlink& a, cmemlink& b)			{ a.swap (b); }
template <> inline void swap (memlink& a, memlink& b)			{ a.swap (b); }
template <> inline void swap (memblock& a, memblock& b)			{ a.swap (b); }
template <> inline void swap (string& a, string& b)			{ a.swap (b); }
template <typename T> inline void swap (vector<T>& a, vector<T>& b)	{ a.swap (b); }
template <typename T> inline void swap (set<T>& a, set<T>& b)		{ a.swap (b); }
template <typename T> inline void swap (multiset<T>& a, multiset<T>& b)	{ a.swap (b); }
template <size_t N, typename T> inline void swap (tuple<N,T>& a, tuple<N,T>& b)	{ a.swap (b); }

//----------------------------------------------------------------------
// Streamable definitions. Not used in the library and require streams.
//----------------------------------------------------------------------

/// Computes the stream size of a standard container.
template <typename Container>
size_t container_stream_size (const Container& v)
{
    typedef typename Container::const_iterator viter_t;
    typedef typename iterator_traits<viter_t>::value_type value_type;
    size_t s = 0;
    if (numeric_limits<value_type>::is_integral)
	s += v.size() * stream_size_of(value_type());
    else {
	foreach (viter_t, i, v)
	    s += stream_size_of(*i);
    }
    return (s);
}

//----{ pair }----------------------------------------------------------

/// \brief Reads pair \p p from stream \p is.
template <typename T1, typename T2>
istream& operator>> (istream& is, pair<T1,T2>& p)
{
    is >> p.first;
    is.align (alignof(T2()));
    is >> p.second;
    is.align (alignof(T1()));
    return (is);
}

/// Writes pair \p p to stream \p os.
template <typename T1, typename T2>
ostream& operator<< (ostream& os, const pair<T1,T2>& p)
{
    os << p.first;
    os.align (alignof(T2()));
    os << p.second;
    os.align (alignof(T1()));
    return (os);
}

/// Writes pair \p p to stream \p os.
template <typename T1, typename T2>
ostringstream& operator<< (ostringstream& os, const pair<T1,T2>& p)
{
    os << '(' << p.first << ',' << p.second << ')';
    return (os);
}

/// Returns the written size of the object.
template <typename T1, typename T2>
inline size_t stream_size_of (const pair<T1,T2>& v)
{
    return (Align (stream_size_of(v.first), alignof(T2())) +
	    Align (stream_size_of(v.second), alignof(T1())));
}

/// \brief Takes a pair and returns pair.first
/// This is an extension, available in uSTL and the SGI STL.
template <typename Pair> struct select1st : public unary_function<Pair,typename Pair::first_type> {
    typedef typename Pair::first_type result_type;
    inline const result_type&	operator()(const Pair& a) const { return (a.first); }
    inline result_type&		operator()(Pair& a) const { return (a.first); }
};

/// \brief Takes a pair and returns pair.second
/// This is an extension, available in uSTL and the SGI STL.
template <typename Pair> struct select2nd : public unary_function<Pair,typename Pair::second_type> {
    typedef typename Pair::second_type result_type;
    inline const result_type&	operator()(const Pair& a) const { return (a.second); }
    inline result_type&		operator()(Pair& a) const { return (a.second); }
};

/// \brief Converts a const_iterator pair into an iterator pair
/// Useful for converting pair ranges returned by equal_range, for instance.
/// This is an extension, available in uSTL.
template <typename Container>
inline pair<typename Container::iterator, typename Container::iterator>
unconst (const pair<typename Container::const_iterator, typename Container::const_iterator>& i, Container& ctr)
{
    assert (i.first >= ctr.begin() && i.first <= ctr.end() && "unconst algorithm must be given iterators from the argument container");
    pair<typename Container::iterator, typename Container::iterator> result;
    result.first = ctr.begin() + (i.first - ctr.begin());
    result.second = ctr.begin() + (i.second - ctr.begin());
    return (result);
}

//----{ vector }--------------------------------------------------------

/// Reads the vector from stream \p is.
template <typename T>
istream& operator>> (istream& is, vector<T>& v)
{
    size_t n;
    is >> n;
    const size_t expectedSize = n * stream_size_of(T());
    if (expectedSize > is.remaining())
	throw stream_bounds_exception ("read", typeid(v).name(), is.pos(), expectedSize, is.remaining());
    v.resize (n);
    foreach (typename vector<T>::iterator, i, v)
	is >> *i;
    is.align();
    return (is);
}

/// Writes the vector to stream \p os.
template <typename T>
ostream& operator<< (ostream& os, const vector<T>& v)
{
    os << v.size();
    copy (v, ostream_iterator<T>(os));
    os.align();
    return (os);
}

/// Writes the vector to stream \p os.
template <typename T>
ostringstream& operator<< (ostringstream& os, const vector<T>& v)
{
    typename vector<T>::const_iterator i = v.begin();
    os << '(';
    if (i < v.end())
	os << *i;
    while (++i < v.end())
	os << ',' << *i;
    os << ')';
    return (os);
}

/// Returns the number of bytes necessary to write this object to a stream
template <typename T>
inline size_t stream_size_of (const vector<T>& v)
{
    return (Align (stream_size_of(v.size()) + container_stream_size (v)));
}

//----{ bitset }--------------------------------------------------------

/// Reads bitset \p v from stream \p is.
template <size_t Size>
inline istream& operator>> (istream& is, bitset<Size>& v)
{
    foreach (typename bitset<Size>::iterator, i, v)
	is >> *i;
    return (is);
}

/// Writes bitset \p v into stream \p os.
template <size_t Size>
inline ostream& operator<< (ostream& os, const bitset<Size>& v)
{
    foreach (typename bitset<Size>::const_iterator, i, v)
	os << *i;
    return (os);
}

/// Writes bitset \p v into stream \p os.
template <size_t Size>
inline ostringstream& operator<< (ostringstream& os, const bitset<Size>& v)
{
    return (os << v.to_string());
}

/// Returns the number of bytes necessary to write this object to a stream
template <size_t Size>
inline size_t stream_size_of (const bitset<Size>& v)
{
    return (v.capacity() / CHAR_BIT);
}

//----{ tuple }---------------------------------------------------------

/// Reads tuple \p v from stream \p is.
template <size_t N, typename T>
inline istream& operator>> (istream& is, tuple<N,T>& v)
{
    typedef typename tuple<N,T>::iterator iter_t;
    foreach (iter_t, i, v)
	is >> *i;
    return (is);
}

/// Writes tuple \p v into stream \p os.
template <size_t N, typename T>
inline ostream& operator<< (ostream& os, const tuple<N,T>& v)
{
    typedef typename tuple<N,T>::const_iterator iter_t;
    foreach (iter_t, i, v)
	os << *i;
    return (os);
}

/// Writes tuple \p v into stream \p os.
template <size_t N, typename T>
ostringstream& operator<< (ostringstream& os, const tuple<N,T>& v)
{
    typename tuple<N,T>::const_iterator i = v.begin();
    os << '(';
    if (i < v.end())
	os << *i;
    while (++i < v.end())
	os << ',' << *i;
    os << ')';
    return (os);
}

template <size_t N>
ostringstream& operator<< (ostringstream& os, const tuple<N,uint8_t>& v)
{
    typename tuple<N,uint8_t>::const_iterator i = v.begin();
    os << '(';
    while (i < v.end()) {
	if (isprint(*i))
	    os << '\'' << *i << '\'';
	else
	    os << (unsigned int)(*i);
	if (++i < v.end())
	    os << ',';
    }
    os << ')';
    return (os);
}

template <size_t N>
ostringstream& operator<< (ostringstream& os, const tuple<N,int8_t>& v)
{
    typename tuple<N,int8_t>::const_iterator i = v.begin();
    os << '(';
    while (i < v.end()) {
	if (isprint(*i))
	    os << '\'' << *i << '\'';
	else
	    os << (int)(*i);
	if (++i < v.end())
	    os << ',';
    }
    os << ')';
    return (os);
}

/// Returns the number of bytes necessary to write this object to a stream
template <size_t N, typename T>
inline size_t stream_size_of (const tuple<N,T>& v)
{
    return (container_stream_size (v));
}

//----{ matrix }--------------------------------------------------------

/// Writes tuple \p v into stream \p os.
template <size_t NX, size_t NY, typename T>
ostringstream& operator<< (ostringstream& os, const matrix<NX,NY,T>& v)
{
    os << '(';
    for (uoff_t row = 0; row < NY; ++ row) {
	os << '(';
        for (uoff_t column = 0; column < NX; ++ column) {
	    os << v[row][column];
	    if (column < NX - 1)
		os << ',';
	}
	os << ')';
    }
    os << ')';
    return (os);
}

//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef WITHOUT_LIBSTDCPP

/// \todo Need a better solution to getting the hash value.
inline hashvalue_t hash_value (const string::const_pointer& v)
{
    string::const_pointer first (v), last (v + strlen(v));
    hashvalue_t h = 0;
    // This has the bits flowing into each other from both sides of the number
    for (; first < last; ++ first)
	h = *first + ((h << 7) | (h >> BitsInType(hashvalue_t) - 7));
    return (h);
}

#endif
#endif

//----------------------------------------------------------------------

} // namespace ustl

// This is here because there really is no other place to put it.
STD_STREAMABLE(cmemlink)
STD_STREAMABLE(istream)
STD_STREAMABLE(ostream)
STD_STREAMABLE(string)
STD_STREAMABLE(exception)
TEXT_STREAMABLE(exception)

#endif

