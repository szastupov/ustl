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
/// \file ualgo.h
///
/// \brief Implementation of STL algorithms.
///
/// The function prototypes are copied
/// exactly from the SGI version of STL documentation along with comments about
/// their use. The code is NOT the same, though the functionality usually is.
///

#ifndef UALGOBASE_H
#define UALGOBASE_H

#include <string.h>
#include "uutility.h"

namespace ustl {

///
/// Assigns the contents of a to b and the contents of b to a.
/// This is used as a primitive operation by many other algorithms. 
///
template <class Assignable> 
inline void swap (Assignable& a, Assignable& b)
{
    Assignable tmp = a;
    a = b;
    b = tmp;
}

///
/// Copy copies elements from the range [first, last) to the range
/// [result, result + (last - first)). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
/// for every integer n from 0 to last - first, copy performs the assignment
/// *(result + n) = *(first + n). Assignments are performed in forward order,
/// i.e. in order of increasing n. 
///
template <class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
    while (first < last)
	*result++ = *first++;
    return (result);
}

///
/// Copy_n copies elements from the range [first, first + n) to the range
/// [result, result + n). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
/// for every integer i from 0 up to (but not including) n, copy_n performs
/// the assignment *(result + i) = *(first + i). Assignments are performed
/// in forward order, i.e. in order of increasing n.
///
template <class InputIterator, class OutputIterator>
inline OutputIterator copy_n (InputIterator first, size_t count, OutputIterator result)
{
    while (count--)
	*result++ = *first++;
    return (result);
}

///
/// For_each applies the function object f to each element in the range
/// [first, last); f's return value, if any, is ignored. Applications are
/// performed in forward order, i.e. from first to last. For_each returns
/// the function object after it has been applied to each element.
///
template <class InputIterator, class UnaryFunction>
UnaryFunction for_each (InputIterator first, InputIterator last, UnaryFunction f)
{
    while (first < last)
	f (*first++);
    return (f);
}

///
/// Fill assigns the value value to every element in the range [first, last).
/// That is, for every iterator i in [first, last),
/// it performs the assignment *i = value.
///
template <class ForwardIterator, class T>
void fill (ForwardIterator first, ForwardIterator last, const T& value)
{
    while (first < last)
	*first++ = value;
}

///
/// Fill_n assigns the value value to every element in the range
/// [first, first+n). That is, for every iterator i in [first, first+n),
/// it performs the assignment *i = value. The return value is first + n.
///
template <class OutputIterator, class T>
OutputIterator fill_n (OutputIterator first, size_t n, const T& value)
{
    while (n--)
	*first++ = value;
    return (first);
}

//
// Specializations for void* and char*.
//
template <>
inline void* copy (const void* first, const void* last, void* result)
{
    memcpy (result, first, distance (first, last));
    return (advance (result, distance (first, last)));
}
template <>
inline char* copy (const char* first, const char* last, char* result)
{
    memcpy (result, first, distance (first, last));
    return (advance (result, distance (first, last)));
}
template <>
inline u_char* copy (const u_char* first, const u_char* last, u_char* result)
{
    memcpy (result, first, distance (first, last));
    return (advance (result, distance (first, last)));
}
template <>
inline void* copy_n (const void* first, size_t count, void* result)
{
    memcpy (result, first, count);
    return (advance (result, count));
}
template <>
inline char* copy_n (const char* first, size_t count, char* result)
{
    memcpy (result, first, count);
    return (advance (result, count));
}
template <>
inline u_char* copy_n (const u_char* first, size_t count, u_char* result)
{
    memcpy (result, first, count);
    return (advance (result, count));
}
inline void fill (void* first, void* last, char value)
{
    memset (first, value, distance (first, last));
}
inline void fill (void* first, void* last, u_char value)
{
    memset (first, value, distance (first, last));
}
inline void fill (char* first, char* last, char value)
{
    memset (first, value, distance (first, last));
}
inline void fill (u_char* first, u_char* last, u_char value)
{
    memset (first, value, distance (first, last));
}
inline void* fill_n (void* first, size_t n, char value)
{
    memset (first, value, n);
    return (advance (first, n));
}
inline void* fill_n (void* first, size_t n, u_char value)
{
    memset (first, value, n);
    return (advance (first, n));
}
inline char* fill_n (char* first, size_t n, char value)
{
    memset (first, value, n);
    return (advance (first, n));
}
inline u_char* fill_n (u_char* first, size_t n, u_char value)
{
    memset (first, value, n);
    return (advance (first, n));
}

}; // namespace ustl

#endif

