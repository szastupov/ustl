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
/// \file uiterator.h
//

#ifndef UITERATOR_H
#define UITERATOR_H

namespace ustl {

//----------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <typename Iterator>
struct iterator_traits {
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template <typename T>
struct iterator_traits<T*> {
    typedef T		value_type;
    typedef ptrdiff_t	difference_type;
    typedef T*		pointer;
    typedef T&		reference;
};

template <typename T>
struct iterator_traits<const T*> {
    typedef T		value_type;
    typedef ptrdiff_t	difference_type;
    typedef const T*	pointer;
    typedef const T&	reference;
};

#endif

//----------------------------------------------------------------------

/// Wraps \p Iterator to behave in an exactly opposite manner.
template <class Iterator>
class reverse_iterator {
public:
    typedef typename iterator_traits<Iterator>::value_type	value_type;
    typedef typename iterator_traits<Iterator>::difference_type	difference_type;
    typedef typename iterator_traits<Iterator>::pointer		pointer;
    typedef typename iterator_traits<Iterator>::reference	reference;
public:
    				reverse_iterator (void) : m_i() {}
    explicit			reverse_iterator (Iterator i) : m_i (i) {}
    				reverse_iterator (const reverse_iterator& i) : m_i (i.m_i) {}
    reverse_iterator&		operator= (const reverse_iterator& i) { m_i = i.m_i; return (*this); }
    inline bool			operator== (const reverse_iterator& i) const { return (m_i == i.m_i); }
    inline bool			operator< (const reverse_iterator& i) const { return (i.m_i < m_i); }
    inline Iterator		base (void) const { return (m_i); }
    inline reference		operator* (void) const { Iterator prev (m_i); --prev; return (*prev); }
    inline pointer		operator-> (void) const { return (&(operator*())); }
    inline reverse_iterator&	operator++ (void) { -- m_i; return (*this); }
    inline reverse_iterator&	operator-- (void) { ++ m_i; return (*this); }
    inline reverse_iterator	operator++ (int) { reverse_iterator prev (*this); -- m_i; return (prev); }
    inline reverse_iterator	operator-- (int) { reverse_iterator prev (*this); ++ m_i; return (prev); }
    inline reverse_iterator&	operator+= (size_t n) { m_i -= n; return (*this); }
    inline reverse_iterator&	operator-= (size_t n) { m_i += n; return (*this); }
    inline reverse_iterator	operator+ (size_t n) const { return (reverse_iterator (m_i - n)); }
    inline reverse_iterator	operator- (size_t n) const { return (reverse_iterator (m_i + n)); }
    inline reference		operator[] (uoff_t n) const { return (*(*this + n)); }
    inline difference_type	operator- (const reverse_iterator& i) const { return (i.m_i - m_i); }
protected:
    Iterator		m_i;
};

//----------------------------------------------------------------------

/// Calls push_back on bound container for each assignment.
template <class Container>
class back_insert_iterator {
public:
    					back_insert_iterator (Container& ctr) : m_rCtr (ctr) {}
    inline back_insert_iterator&	operator= (typename Container::const_reference v)
    					    { m_rCtr.push_back (v); return (*this); }
    inline back_insert_iterator&	operator* (void)  { return (*this); }
    inline back_insert_iterator&	operator++ (void) { return (*this); }
    inline back_insert_iterator		operator++ (int)  { return (*this); }
protected:
    Container&		m_rCtr;
};

/// Returns the back_insert_iterator for \p ctr.
template <class Container>
inline back_insert_iterator<Container> back_inserter (Container& ctr)
{
    return (back_insert_iterator<Container> (ctr));
}

//----------------------------------------------------------------------

} // namespace ustl

#endif

