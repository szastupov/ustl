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

namespace ustl {

template <typename K>
class multiset : public vector<K> {
public:
    typedef typename vector<K>::value_type	value_type;
    typedef typename vector<K>::pointer		pointer;
    typedef typename vector<K>::const_pointer	const_pointer;
    typedef typename vector<K>::reference	reference;
    typedef typename vector<K>::const_reference	const_reference;
    typedef typename vector<K>::const_iterator	const_iterator;
    typedef typename vector<K>::iterator	iterator;
    typedef typename vector<K>::reverse_iterator	reverse_iterator;
    typedef typename vector<K>::const_reverse_iterator	const_reverse_iterator;
public:
    			multiset (void);
    explicit		multiset (size_t n);
    			multiset (const multiset<K>& v);
			multiset (const_iterator i1, const_iterator i2);
    inline const multiset<K>&	operator= (const multiset<K>& v);
    inline void		assign (const_iterator i1, const_iterator i2);
    size_t		count (const_reference v) const;
    inline void		push_back (const_reference v);
    iterator		insert (const_reference v);
    inline void		insert (const_iterator i1, const_iterator i2);
    void		erase (const_reference v);
    inline iterator	erase (iterator ep);
    inline iterator	erase (iterator ep1, iterator ep2);
};

template <typename K>
multiset<K>::multiset (void)
: vector<K> ()
{
}

template <typename K>
multiset<K>::multiset (size_t n)
: vector<K> (n)
{
}

template <typename K>
multiset<K>::multiset (const multiset<K>& v)
: vector<K> (v)
{
}

template <typename K>
multiset<K>::multiset (const_iterator i1, const_iterator i2)
: vector<K> ()
{
    insert (i1, i2);
}

template <typename K>
inline const multiset<K>& multiset<K>::operator= (const multiset<K>& v)
{
    vector<K>::operator= (v);
    return (*this);
}

template <typename K>
inline void multiset<K>::assign (const_iterator i1, const_iterator i2)
{
    clear();
    insert (i1, i2);
}

template <typename K>
size_t multiset<K>::count (const_reference v) const
{
    const pair<const_iterator,const_iterator> fr = equal_range (begin(), end(), v);
    return (distance (fr.first, fr.second));
}

template <typename K>
inline void multiset<K>::push_back (const_reference v)
{
    insert (v);
}

template <typename K>
typename multiset<K>::iterator multiset<K>::insert (const_reference v)
{
    iterator ip = lower_bound (begin(), end(), v);
    return (vector<K>::insert (ip, v));
}

template <typename K>
inline void multiset<K>::insert (const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    reserve (size() + distance (i1, i2));
    for_each (i1, i2, mem_fun (this, &multiset<K>::push_back));
}

template <typename K>
void multiset<K>::erase (const_reference v)
{
    pair<iterator,iterator> epr = equal_range (begin(), end(), v);
    erase (epr.first, epr.second);
}

template <typename K>
inline typename multiset<K>::iterator multiset<K>::erase (iterator ep)
{
    return (vector<K>::erase (ep));
}

template <typename K>
inline typename multiset<K>::iterator multiset<K>::erase (iterator ep1, iterator ep2)
{
    return (vector<K>::erase (ep1, ep2));
}

} // namespace ustl

#endif

