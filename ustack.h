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
// ustack.h
//
/** \class stack
 *
 * \brief Stack adapter to uSTL containers.
 */

#ifndef USTACK_H
#define USTACK_H

namespace ustl {

template <class Sequence>
class stack {
public:
    typedef typename Sequence::value_type	value_type;
    typedef typename Sequence::size_type	size_type;
    typedef typename Sequence::difference_type	difference_type;
    typedef typename Sequence::reference	reference;
    typedef typename Sequence::const_reference	const_reference;
    typedef typename Sequence::pointer		pointer;
public:
    				stack (void);
    explicit			stack (const Sequence& s);
    inline bool			empty (void) const;
    inline size_type		size (void) const;
    inline reference		top (void);
    inline const_reference	top (void) const;
    inline void			push (const value_type& v);
    inline void			pop (void);
    inline bool			operator== (const stack& s);
    inline bool			operator< (const stack& s);
private:
    Sequence	m_Storage;
};

template <class Sequence>
stack<Sequence>::stack (void)
: m_Storage ()
{
}

template <class Sequence>
stack<Sequence>::stack (const Sequence& s)
: m_Storage (s)
{
}

template <class Sequence>
inline bool stack<Sequence>::empty (void) const
{
    return (m_Storage.empty());
}

template <class Sequence>
inline typename stack<Sequence>::size_type stack<Sequence>::size (void) const
{
    return (m_Storage.size());
}

template <class Sequence>
inline typename stack<Sequence>::reference stack<Sequence>::top (void)
{
    return (m_Storage.back());
}

template <class Sequence>
inline typename stack<Sequence>::const_reference stack<Sequence>::top (void) const
{
    return (m_Storage.back());
}

template <class Sequence>
inline void stack<Sequence>::push (const value_type& v)
{
    m_Storage.push_back (b);
}

template <class Sequence>
inline void stack<Sequence>::pop (void)
{
    m_Storage.pop_back();
}

template <class Sequence>
inline bool stack<Sequence>::operator== (const stack& s)
{
    return (m_Storage == s.m_Storage);
}

template <class Sequence>
inline bool stack<Sequence>::operator< (const stack& s)
{
    return (m_Storage < s.m_Storage);
}

} // namespace ustl

#endif

