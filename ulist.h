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
// ulist.h
//

#ifndef ULIST_H_54E3B510498982C87A0A1E1932E6729D
#define ULIST_H_54E3B510498982C87A0A1E1932E6729D

#include "uvector.h"
#include "uctralgo.h"

namespace ustl {

/// \class list ulist.h ustl.h
/// \ingroup Sequences
///
/// \brief Linked list emulator.
///
/// The list template is aliased to vector for philosophical reasons.
/// I believe the use of linked lists of any kind to be an indicator
/// of bad design. It is simply the wrong way of thinking about storage.
/// The alias is provided for compatibility with the C++ standard and
/// is mostly inline.
///
template <typename T>
class list : public vector<T> {
public:
    typedef typename vector<T>::iterator	iterator;
    typedef typename vector<T>::const_iterator	const_iterator;
    typedef typename vector<T>::reference	reference;
    typedef typename vector<T>::const_reference	const_reference;
public:
    inline			list (void)			: vector<T> () {}
    inline explicit		list (size_t n)			: vector<T> (n) {}
    inline			list (size_t n, const T& v)	: vector<T> (n, v) {}
    inline			list (const list<T>& v)		: vector<T> (v) {}
    inline			list (const_iterator i1, const_iterator i2)	: vector<T> (i1, i2) {}
    inline size_t		size (void) const		{ return (vector<T>::size()); }
    inline iterator		begin (void)			{ return (vector<T>::begin()); }
    inline const_iterator	begin (void) const		{ return (vector<T>::begin()); }
    inline iterator		end (void)			{ return (vector<T>::end()); }
    inline const_iterator	end (void) const		{ return (vector<T>::end()); }
    inline void			push_front (const T& v)		{ insert (begin(), v); }
    inline void			pop_front (void)		{ erase (begin()); }
    inline const_reference	front (void) const		{ return (*begin()); }
    inline reference		front (void)			{ return (*begin()); }
    inline void			remove (const T& v)		{ ::ustl::remove (*this, v); }
    inline void			unique (void)			{ ::ustl::unique (*this); }
    inline void			sort (void)			{ ::ustl::sort (*this); }
    void			merge (list<T>& l);
    void			splice (iterator ip, list<T>& l, iterator first = NULL, iterator last = NULL);
};

/// Merges the contents with \p l. Assumes both lists are sorted.
template <typename T>
void list<T>::merge (list& l)
{
    resize (size() + l.size());
    iterator me = merge (begin(), end(), l.begin(), l.end(), begin());
    resize (distance (begin(), me));
}

/// Moves the range [first, last) from \p l to this list at \p ip.
template <typename T>
void list<T>::splice (iterator ip, list<T>& l, iterator first, iterator last)
{
    if (!first)
	first = l.begin();
    if (!last)
	last = l.end();
    insert (ip, first, last);
    l.erase (first, last);
}

} // namespace ustl

#endif

