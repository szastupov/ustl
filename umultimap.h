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
// umultimap.h
//

#ifndef UMULTIMAP_H_45743F516E02A87A3FCEA5024052A6F5
#define UMULTIMAP_H_45743F516E02A87A3FCEA5024052A6F5

#include "uvector.h"
#include "ufunction.h"

namespace ustl {

/// \class multimap umultimap.h ustl.h
/// \ingroup AssociativeContainers
///
/// \brief A sorted associative container that may container multiple entries for each key.
///
template <typename K, typename V>
class multimap : public vector<pair<K,V> > {
public:
    typedef K						key_type;
    typedef V						data_type;
    typedef const K&					const_key_ref;
    typedef const V&					const_data_ref;
    typedef typename vector<pair<K,V> >::value_type	value_type;
    typedef typename vector<pair<K,V> >::pointer	pointer;
    typedef typename vector<pair<K,V> >::const_pointer	const_pointer;
    typedef typename vector<pair<K,V> >::reference	reference;
    typedef typename vector<pair<K,V> >::const_reference	const_reference;
    typedef typename vector<pair<K,V> >::const_iterator		const_iterator;
    typedef typename vector<pair<K,V> >::iterator		iterator;
    typedef typename vector<pair<K,V> >::reverse_iterator	reverse_iterator;
    typedef typename vector<pair<K,V> >::const_reverse_iterator	const_reverse_iterator;
    typedef pair<const_iterator,const_iterator>		const_range_t;
    typedef pair<iterator,iterator>			range_t;
public:
    				multimap (void);
    explicit			multimap (size_t n);
    				multimap (const multimap<K,V>& v);
				multimap (const_iterator i1, const_iterator i2);
    inline const multimap<K,V>&	operator= (const multimap<K,V>& v);
    inline void			assign (const_iterator i1, const_iterator i2);
    size_t			count (const_key_ref k) const;
    inline void			push_back (const_reference v);
    inline const_range_t	equal_range (const_key_ref k) const;
    inline range_t		equal_range (const_key_ref k);
    inline const_iterator	lower_bound (const_key_ref k) const;
    inline const_iterator	upper_bound (const_key_ref k) const;
    iterator			insert (const_reference v);
    inline void			insert (const_iterator i1, const_iterator i2);
    void			erase (const_key_ref k);
    inline iterator		erase (iterator ep);
    inline iterator		erase (iterator ep1, iterator ep2);
    inline void			clear (void)		{ vector<pair<K,V> >::clear(); }
    inline size_t		size (void) const	{ return (vector<pair<K,V> >::size()); }
    inline iterator		begin (void)		{ return (vector<pair<K,V> >::begin()); }
    inline const_iterator	begin (void) const	{ return (vector<pair<K,V> >::begin()); }
    inline iterator		end (void)		{ return (vector<pair<K,V> >::end()); }
    inline const_iterator	end (void) const	{ return (vector<pair<K,V> >::end()); }
};

/// Default constructor.
template <typename K, typename V>
multimap<K,V>::multimap (void)
: vector<pair<K,V> > ()
{
}

/// Creates container with enough space for \p n elements.
template <typename K, typename V>
multimap<K,V>::multimap (size_t n)
: vector<pair<K,V> > (n)
{
}

/// Creates a copy of \p v.
template <typename K, typename V>
multimap<K,V>::multimap (const multimap<K,V>& v)
: vector<pair<K,V> > (v)
{
}

/// Inserts elements from range [i1,i2).
template <typename K, typename V>
multimap<K,V>::multimap (const_iterator i1, const_iterator i2)
: vector<pair<K,V> > ()
{
    insert (i1, i2);
}

/// Copies contents of \p v.
template <typename K, typename V>
inline const multimap<K,V>& multimap<K,V>::operator= (const multimap<K,V>& v)
{
    vector<pair<K,V> >::operator= (v);
    return (*this);
}

/// Copies contents of [i1,i2).
template <typename K, typename V>
inline void multimap<K,V>::assign (const_iterator i1, const_iterator i2)
{
    clear();
    insert (i1, i2);
}

/// Returns the range of all elements with key value \p k.
template <typename K, typename V>
inline pair<typename multimap<K,V>::const_iterator,typename multimap<K,V>::const_iterator>
multimap<K,V>::equal_range (const_key_ref k) const
{
    return (::ustl::equal_range (begin(), end(), make_pair(k, V()), mem_var_less(&value_type::first)));
}

/// Returns the range of all elements with key value \p k.
template <typename K, typename V>
inline pair<typename multimap<K,V>::iterator,typename multimap<K,V>::iterator>
multimap<K,V>::equal_range (const_key_ref k)
{
    return (::ustl::equal_range (begin(), end(), make_pair(k, V()), mem_var_less(&value_type::first)));
}

/// Returns the number of elements with key value \p k.
template <typename K, typename V>
size_t multimap<K,V>::count (const_key_ref k) const
{
    const pair<const_iterator,const_iterator> fr = equal_range (k);
    return (distance (fr.first, fr.second));
}

/// Returns an iterator to the first element with key value \p k.
template <typename K, typename V>
inline typename multimap<K,V>::const_iterator multimap<K,V>::lower_bound (const_key_ref k) const
{
    return (::ustl::lower_bound (begin(), end(), make_pair(k, V()), mem_var_less(&value_type::first)));
}

/// Returns an iterator to the first element with key value \p k.
template <typename K, typename V>
inline typename multimap<K,V>::const_iterator multimap<K,V>::upper_bound (const_key_ref k) const
{
    return (::ustl::upper_bound (begin(), end(), make_pair(k, V()), mem_var_less(&value_type::first)));
}

/// Inserts the pair into the container.
template <typename K, typename V>
inline void multimap<K,V>::push_back (const_reference v)
{
    insert (v);
}

/// Inserts the pair into the container.
template <typename K, typename V>
typename multimap<K,V>::iterator multimap<K,V>::insert (const_reference v)
{
    iterator ip = ::ustl::lower_bound (begin(), end(), v, mem_var_less(&value_type::first));
    return (vector<pair<K,V> >::insert (ip, v));
}

/// Inserts elements from range [i1,i2) into the container.
template <typename K, typename V>
inline void multimap<K,V>::insert (const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    reserve (size() + distance (i1, i2));
    for_each (i1, i2, mem_fun (this, &multimap::push_back));
}

/// Erases all elements with key value \p k.
template <typename K, typename V>
void multimap<K,V>::erase (const_key_ref k)
{
    pair<iterator,iterator> epr = equal_range (k);
    erase (epr.first, epr.second);
}

/// Erases element at \p ep.
template <typename K, typename V>
inline typename multimap<K,V>::iterator multimap<K,V>::erase (iterator ep)
{
    return (vector<pair<K,V> >::erase (ep));
}

/// Erases range [ep1,ep2).
template <typename K, typename V>
inline typename multimap<K,V>::iterator multimap<K,V>::erase (iterator ep1, iterator ep2)
{
    return (vector<pair<K,V> >::erase (ep1, ep2));
}

} // namespace ustl

#endif

