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

}; // namespace ustl

#endif

