// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
/// \file upair.h
//

#ifndef UPAIR_H
#define UPAIR_H

#include "mistream.h"
#include "mostream.h"

namespace ustl {

/// Container for two values.
template <typename T1, typename T2>
class pair {
public:
    typedef T1		first_type;
    typedef T2		second_type;
public:
   	 		pair (void);
			pair (const T1& a, const T2& b);
			pair (const pair<T1,T2>& p);
    inline size_t	stream_size (void) const;
    const pair<T1,T2>&	operator= (const pair<T1,T2>& p);
public:
    first_type		first;
    second_type		second;
};

/// Default constructor.
template <typename T1, typename T2>
pair<T1,T2>::pair (void)
: first (T1()),
  second (T2())
{
}

/// Initializes members with \p a, and \p b.
template <typename T1, typename T2>
pair<T1,T2>::pair (const T1& a, const T2& b)
: first (a),
  second (b)
{
}

/// Copies contents of \p p.
template <typename T1, typename T2>
pair<T1,T2>::pair (const pair<T1,T2>& p)
: first (p.first),
  second (p.second)
{
}

/// Copies contents of \p p.
template <typename T1, typename T2>
const pair<T1,T2>& pair<T1,T2>::operator= (const pair<T1,T2>& p)
{
    first = p.first;
    second = p.second;
    return (*this);
}

/// Compares both values of \p p1 to those of \p p2.
template <typename T1, typename T2>
bool operator== (const pair<T1,T2>& p1, const pair<T1,T2>& p2)
{
    return (p1.first == p2.first && p1.second == p2.second);
}

/// Compares both values of \p p1 to those of \p p2.
template <typename T1, typename T2>
bool operator< (const pair<T1,T2>& p1, const pair<T1,T2>& p2)
{
    return (p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second));
}

/// Returns a pair object with (a,b)
template <typename T1, typename T2>
inline pair<T1,T2> make_pair (const T1& a, const T2& b)
{
    return (pair<T1,T2> (a, b));
}

/// Returns the written size of the object.
template <typename T1, typename T2>
inline size_t pair<T1,T2>::stream_size (void) const
{
    return (2 * max(sizeof(T1), sizeof(T2)));
}

/// Returns the written size of the object.
template <typename T1, typename T2>
inline size_t stream_size_of (const pair<T1,T2>&)
{
    return (2 * max(sizeof(T1), sizeof(T2)));
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

} // namespace ustl

#endif

