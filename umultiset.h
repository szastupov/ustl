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
// umultiset.h
//

#ifndef UMULTISET_H_446AEDBB7F61C6994DC228C25D5FA3A1
#define UMULTISET_H_446AEDBB7F61C6994DC228C25D5FA3A1

#include "uvector.h"
#include "ualgo.h"

namespace ustl {

/// Multiple sorted container. Unlike set, it may contain multiple copies of each element.
template <typename T>
class multiset : public vector<T> {
public:
    typedef typename vector<T>::value_type	value_type;
    typedef typename vector<T>::pointer		pointer;
    typedef typename vector<T>::const_pointer	const_pointer;
    typedef typename vector<T>::reference	reference;
    typedef typename vector<T>::const_reference	const_reference;
    typedef typename vector<T>::const_iterator	const_iterator;
    typedef typename vector<T>::iterator	iterator;
    typedef typename vector<T>::reverse_iterator	reverse_iterator;
    typedef typename vector<T>::const_reverse_iterator	const_reverse_iterator;
public:
    			multiset (void);
    explicit		multiset (size_t n);
    			multiset (const multiset<T>& v);
			multiset (const_iterator i1, const_iterator i2);
    inline const multiset<T>&	operator= (const multiset<T>& v);
    inline void		assign (const_iterator i1, const_iterator i2);
    size_t		count (const_reference v) const;
    inline void		push_back (const_reference v);
    iterator		insert (const_reference v);
    inline void		insert (const_iterator i1, const_iterator i2);
    void		erase (const_reference v);
    inline iterator	erase (iterator ep);
    inline iterator	erase (iterator ep1, iterator ep2);
};

/// Default constructor.
template <typename T>
multiset<T>::multiset (void)
: vector<T> ()
{
}

/// Creates the container with space enough to hold \p n elements.
template <typename T>
multiset<T>::multiset (size_t n)
: vector<T> (n)
{
}

/// Creates a copy of \p v.
template <typename T>
multiset<T>::multiset (const multiset<T>& v)
: vector<T> (v)
{
}

/// Copies range [i1,i2)
template <typename T>
multiset<T>::multiset (const_iterator i1, const_iterator i2)
: vector<T> ()
{
    insert (i1, i2);
}

/// Copies contents of \p v.
template <typename T>
inline const multiset<T>& multiset<T>::operator= (const multiset<T>& v)
{
    vector<T>::operator= (v);
    return (*this);
}

/// Copies contents of range [i1,i2)
template <typename T>
inline void multiset<T>::assign (const_iterator i1, const_iterator i2)
{
    clear();
    insert (i1, i2);
}

/// Returns the number of elements of value \p v.
template <typename T>
size_t multiset<T>::count (const_reference v) const
{
    const pair<const_iterator,const_iterator> fr = equal_range (begin(), end(), v);
    return (distance (fr.first, fr.second));
}

/// Inserts \p v.
template <typename T>
inline void multiset<T>::push_back (const_reference v)
{
    insert (v);
}

/// Inserts \p v.
template <typename T>
typename multiset<T>::iterator multiset<T>::insert (const_reference v)
{
    iterator ip = lower_bound (begin(), end(), v);
    return (vector<T>::insert (ip, v));
}

/// Inserts all elements from range [i1,i2).
template <typename T>
inline void multiset<T>::insert (const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    reserve (size() + distance (i1, i2));
    while (i1 < i2)
	push_back (*i1++);
}

/// Erases all elements with value \p v.
template <typename T>
void multiset<T>::erase (const_reference v)
{
    pair<iterator,iterator> epr = equal_range (begin(), end(), v);
    erase (epr.first, epr.second);
}

/// Erases the element at \p ep.
template <typename T>
inline typename multiset<T>::iterator multiset<T>::erase (iterator ep)
{
    return (vector<T>::erase (ep));
}

/// Erases range [ep1,ep2).
template <typename T>
inline typename multiset<T>::iterator multiset<T>::erase (iterator ep1, iterator ep2)
{
    return (vector<T>::erase (ep1, ep2));
}

}; // namespace ustl

#endif

