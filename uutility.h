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
/** \file uutility.h
 *
 * \brief Utility templates.
*/

#ifndef UUTILITY_H
#define UUTILITY_H

#include "utypes.h"
#include <stdlib.h>
#include <assert.h>

namespace ustl {

const size_t c_DefaultAlignment = sizeof(u_long);

/// Rounds \p n up to be divisible by \p grain
template <class T>
inline T Align (T n, size_t grain = c_DefaultAlignment)
{
    const T remainder = n % grain;
    return (remainder ? n + (grain - remainder) : n);
}

/// Offsets an iterator
template <typename T>
inline T advance (const T& i, off_t offset)
{
    return (i + offset);
}

/// Offsets a void pointer
inline const void* advance (const void* p, off_t offset)
{
    assert (p || !offset);
    return (reinterpret_cast<const u_char*>(p) + offset);
}

/// Offsets a void pointer
inline void* advance (void* p, off_t offset)
{
    assert (p || !offset);
    return (reinterpret_cast<u_char*>(p) + offset);
}

/// Returns the difference \p p1 - \p p2
template <typename T>
inline size_t distance (const T& i1, const T& i2)
{
    return (i2 - i1);
}

/// Returns the difference \p p1 - \p p2
inline size_t distance (void* p1, void* p2)
{
    return (reinterpret_cast<u_char*>(p2) - reinterpret_cast<u_char*>(p1));
}

/// Returns the difference \p p1 - \p p2
inline size_t distance (const void* p1, const void* p2)
{
    return (reinterpret_cast<const u_char*>(p2) -
	    reinterpret_cast<const u_char*>(p1));
}

/// Returns the number of elements in the static vector
#define VectorSize(v) (sizeof(v)/sizeof(*v))

/// Returns the number of bits in the given type
#define BitsInType(t) (sizeof(t) * CHAR_BIT)

/// Template for for_each to call delete
template <typename T>
inline void Delete (T* p)
{
    delete p;
}

/// Template for for_each to call delete
template <typename T>
inline void DeleteVector (T* p)
{
    delete [] p;
}

/// Returns the minimum of \p a and \p b
template <typename T>
inline const T& min (const T& a, const T& b)
{
    return (a < b ? a : b);
}

/// Returns the maximum of \p a and \p b
template <typename T>
inline const T& max (const T& a, const T& b)
{
    return (b < a ? a : b);
}

/// Template of making != from ! and ==
template <typename T>
inline bool operator!= (const T& x, const T& y)
{
    return (!(x == y));
}

/// Template of making > from <
template <typename T>
inline bool operator> (const T& x, const T& y)
{
    return (y < x);
}

/// Template of making <= from < and ==
template <typename T>
inline bool operator<= (const T& x, const T& y)
{
    return (x == y || x < y);
}

/// Template of making >= from < and ==
template <typename T>
inline bool operator>= (const T& x, const T& y)
{
    return (x == y || y < x);
}

} // namespace ustl

#endif

