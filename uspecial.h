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
#include "ufunction.h"
#include "mistream.h"
#include "mostream.h"
#include "strmsize.h"

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

//----------------------------------------------------------------------
// Streamable definitions. Not used in the library and require streams.
//----------------------------------------------------------------------

//----{ pair }----------------------------------------------------------
///
/// \brief Reads pair \p p from stream \p is.
///
/// To allow compile-time resolution of these conditionals, sizeof is
/// used instead of stream_size_of. This is ok as long as you don't create
/// some strange class that writes only a short. If you do that you'll
/// have to resolve the alignment issues yourself. It is not efficient to
/// do it here as a general case since stream_size methods are not always
/// constant.
///
template <typename T1, typename T2>
istream& operator>> (istream& is, pair<T1,T2>& p)
{
    is >> p.first;
    if (sizeof(T1) < sizeof(T2) && sizeof(T1) % c_DefaultAlignment)
	is.align (min (sizeof(T2), c_DefaultAlignment));
    is >> p.second;
    if (sizeof(T2) < sizeof(T1) && sizeof(T2) % c_DefaultAlignment)
	is.align (min (sizeof(T1), c_DefaultAlignment));
    return (is);
}

/// Writes pair \p p to stream \p os.
template <typename T1, typename T2>
ostream& operator<< (ostream& os, const pair<T1,T2>& p)
{
    os << p.first;
    if (sizeof(T1) < sizeof(T2) && sizeof(T1) % c_DefaultAlignment)
	os.align (min (sizeof(T2), c_DefaultAlignment));
    os << p.second;
    if (sizeof(T2) < sizeof(T1) && sizeof(T2) % c_DefaultAlignment)
	os.align (min (sizeof(T1), c_DefaultAlignment));
    return (os);
}

/// Returns the written size of the object.
template <typename T1, typename T2>
inline size_t stream_size_of (const pair<T1,T2>&)
{
    return (2 * max(sizeof(T1), sizeof(T2)));
}

/// Takes a pair and returns pair.first
/// This is an extension, available in uSTL and the SGI STL.
template <typename Pair> struct select1st : public unary_function<Pair,typename Pair::first_type> {
    typedef typename Pair::first_type result_type;
    inline const result_type&	operator()(const Pair& a) const { return (a.first); }
    inline result_type&		operator()(Pair& a) const { return (a.first); }
};

/// Takes a pair and returns pair.second
/// This is an extension, available in uSTL and the SGI STL.
template <typename Pair> struct select2nd : public unary_function<Pair,typename Pair::second_type> {
    typedef typename Pair::second_type result_type;
    inline const result_type&	operator()(const Pair& a) const { return (a.second); }
    inline result_type&		operator()(Pair& a) const { return (a.second); }
};

//----{ vector }--------------------------------------------------------

/// Reads the vector from stream \p is.
template <typename T>
istream& operator>> (istream& is, vector<T>& v)
{
    size_t n;
    is >> n;
    assert (n * stream_size_of(T()) <= is.remaining() && "This does not look like a written vector.");
    v.resize (n);
    copy_n (istream_iterator<T>(is), n, v.begin());
    is.align();
    return (is);
}

/// Writes the vector to stream \p os.
template <typename T>
ostream& operator<< (ostream& os, const vector<T>& v)
{
    os << v.size();
    copy_n (v.begin(), v.size(), ostream_iterator<T>(os));
    os.align();
    return (os);
}

/// Returns the number of bytes necessary to write this object to a stream
template <typename T>
size_t stream_size_of (const vector<T>& v)
{
    typedef typename vector<T>::const_iterator viter_t;
    size_t s = sizeof(size_t);
    for (viter_t first = v.begin(); first < v.end(); ++ first)
	s += stream_size_of(*first);
    return (Align (s));
}

//----{ bitset }--------------------------------------------------------

/// Reads bitset \p v from stream \p is.
template <size_t Size>
inline istream& operator>> (istream& is, bitset<Size>& v)
{
    for (typename bitset<Size>::iterator i = v.begin(); i < v.end(); ++ i)
	is >> *i;
    return (is);
}

/// Writes bitset \p v into stream \p os.
template <size_t Size>
inline ostream& operator<< (ostream& os, const bitset<Size>& v)
{
    for (typename bitset<Size>::const_iterator i = v.begin(); i < v.end(); ++ i)
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

//----------------------------------------------------------------------

class fdostringstream; // for TEXT_STREAMABLE below

}; // namespace ustl

// This is here because there really is no other place to put it.
STD_STREAMABLE(ustl::cmemlink);
STD_STREAMABLE(ustl::istream);
STD_STREAMABLE(ustl::ostream);
STD_STREAMABLE(ustl::exception);
STD_STREAMABLE(ustl::string)

TEXT_STREAMABLE(ustl::exception);

#endif

