/// \file uspecial.h
///
/// \brief Template specializations for uSTL classes.
///

#ifndef USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0
#define USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0

#include "uvector.h"
#include "ustring.h"
#include "uset.h"
#include "umultiset.h"
#include "fdostream.h"

namespace ustl {

template <>
inline void swap (cmemlink& a, cmemlink& b)
{
    a.swap (b);
}

template <> 
inline void swap (memlink& a, memlink& b)
{
    a.swap (b);
}

template <> 
inline void swap (memblock& a, memblock& b)
{
    a.swap (b);
}

template <> 
inline void swap (string& a, string& b)
{
    a.swap (b);
}

template <typename T> 
inline void swap (vector<T>& a, vector<T>& b)
{
    a.swap (b);
}

template <typename T>
inline void swap (set<T>& a, set<T>& b)
{
    a.swap (b);
}

template <typename T>
inline void swap (multiset<T>& a, multiset<T>& b)
{
    a.swap (b);
}

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

} // namespace ustl

// This is here because there really is no other place to put it.
STD_STREAMABLE(ustl::cmemlink)
STD_STREAMABLE(ustl::istream)
STD_STREAMABLE(ustl::ostream)
STD_STREAMABLE(ustl::exception)
TEXT_STREAMABLE(ustl::exception)

#endif

