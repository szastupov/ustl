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
// uheap.h
//
// Implementation of STL heap algorithms.
//
// The function prototypes are copied
// exactly from the SGI version of STL documentation along with comments about
// their use. The code is NOT the same, though the functionality usually is.
//

#ifndef UHEAP_H_574B9EAF271A1C107190B4D575A356C5
#define UHEAP_H_574B9EAF271A1C107190B4D575A356C5

#include "ualgobase.h"

namespace ustl {

/// \brief make_heap turns the range [first, last) into a heap
/// At completion, is_heap (first, last, comp) is true.
/// The algorithm is adapted from "Classic Data Structures in C++" by Timothy Budd.
///
template <typename RandomAccessIterator, typename Compare>
void make_heap (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    const value_type v (*first);
    RandomAccessIterator i (first);
    while (i < last) {
	// replace position with the smaller of the two children, or the last element
	RandomAccessIterator iChild = i + 2 * distance (first, i) + 1;
	if (iChild < last) {
	    if (iChild + 1 < last && comp (*(iChild + 1), *iChild))
		++ iChild;
	    if (comp (v, *iChild)) {
		*i = v;
		break;
	    } else {
		*i = *iChild;
		i = iChild;
	    }
	} else {
	    *i = v;
	    break;
	}
    }
}

template <typename RandomAccessIterator, typename Compare>
void push_heap (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    if (last <= first)
	return;
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    const value_type v (*--last);
    while (first < last) {
	RandomAccessIterator iParent = first + distance(first, last) / 2;
	if (comp (*iParent, v))
	    break;
	else {
	    *last = *iParent;
	    last = iParent;
	}
    }
    *last = v;
}

/// \brief Make_heap turns the range [first, last) into a heap
/// At completion, is_heap (first, last) is true.
template <typename RandomAccessIterator>
inline void make_heap (RandomAccessIterator first, RandomAccessIterator last)
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    make_heap (first, last, less<value_type>());
}

template <typename RandomAccessIterator>
inline void push_heap (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    push_heap (first, last, less<value_type>());
}

}; // namespace ustl

#endif

