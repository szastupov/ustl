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
// memlink.h

#ifndef MEMLINK_H
#define MEMLINK_H

#include "cmemlink.h"

namespace ustl {

class istream;

///
/// \brief Wrapper for pointer to block with size.
///
/// Use this class the way you would a pointer to an allocated unstructured block.
/// The pointer and block size are available through member functions and cast operator.
/// The begin in the block may be modified, but the block is static in size.
/// \warning This class actually contains two pointers: a const pointer and a non-const
/// pointer. Both are the same when you have linked the object to a modifiable block.
/// But if you have for some reason linked the object to a const block, the non-const
/// pointer will be NULL. With proper usage, there should be no problem with this; just
/// be aware that such a thing may happen.
///
/// Example usage:
/// \code
///     void* p = malloc (46721);
///     memlink a, b;
///     a.link (p, 46721);
///     assert (a.size() == 46721));
///     b = a;
///     assert (b.size() == 46721));
///     assert (b.begin() + 34 == a.begin + 34);
///     assert (0 == memcmp (a, b, 12));
///     a.fill (673, b, 42, 67);
///     b.erase (87, 12);
/// \endcode
///
class memlink : public cmemlink {
public:
    typedef value_type*			pointer;
    typedef cmemlink::pointer		const_pointer;
    typedef cmemlink::const_iterator	const_iterator;
    /// A wrapper for void pointers to allow iterator functionality.
    class iterator {
    public:
	typedef void		value_type;
	typedef ptrdiff_t	difference_type;
	typedef void*		pointer;
	typedef void		reference;
    public:
				iterator (pointer p = NULL) : m_p (p) {}
				iterator (const iterator& i) : m_p (i.m_p) {}
	inline const iterator&	operator= (const iterator& i) { m_p = i.m_p; return (*this); }
	inline 			operator void* (void) const { return (m_p); }
	inline pointer		base (void) const { return (m_p); }
	inline const iterator&	operator++ (void) { m_p = advance (m_p, 1); return (*this); }
	inline const iterator&	operator-- (void) { m_p = advance (m_p, -1); return (*this); }
	inline const iterator&	operator+= (off_t n) { m_p = advance (m_p, n); return (*this); }
	inline const iterator&	operator-= (off_t n) { m_p = advance (m_p, -n); return (*this); }
	inline iterator		operator++ (int) { iterator old (*this); m_p = advance (m_p, 1); return (old); }
	inline iterator		operator-- (int) { iterator old (*this); m_p = advance (m_p, -1); return (old); }
	inline iterator		operator+ (off_t n) const { return (iterator (advance (m_p, n))); }
	inline iterator		operator- (off_t n) const { return (iterator (advance (m_p, -n))); }
	inline difference_type	operator- (const iterator& i) const { return (distance (i.m_p, m_p)); }
	inline bool		operator== (const iterator& i) const { return (m_p == i.m_p); }
	inline bool		operator< (const iterator& i) const { return (m_p < i.m_p); }
    private:
	pointer			m_p;
    };
public:
			memlink (void);
			memlink (void* p, size_t n);
			memlink (const void* p, size_t n);
    explicit		memlink (const cmemlink& l);
			memlink (const memlink& l);
    virtual	       ~memlink (void);
    inline void		link (const void* p, size_t n);
    inline void		link (const cmemlink& l);
    void		link (void* p, size_t n);
    inline void		link (memlink& l);
    virtual void	unlink (void);
    inline void		copy (const cmemlink& l);
    inline void		copy (const void* p, size_t n);
    void		copy (iterator offset, const void* p, size_t n);
    inline void*	data (void);
    inline		operator void* (void) const;
    const memlink&	operator= (const cmemlink& l);
    const memlink&	operator= (const memlink& l);
    void		swap (memlink& l);
    inline iterator	begin (void);
    inline iterator	end (void);
    inline const_iterator	begin (void) const;
    inline const_iterator	end (void) const;
    void		fill (iterator start, const void* p, size_t elsize, size_t elCount = 1);
    void		insert (iterator start, size_t size);
    void		erase (iterator start, size_t size);
    virtual void	read (istream& is);
protected:
    virtual void	constructBlock (void*, size_t) const;
    virtual void	destructBlock (void*, size_t) const;
private:
    mutable void*	m_Data;	///< Pointer to the begin block (non-const)
};

/// Returns a modifiable pointer to the block
inline void* memlink::data (void)
{
    return (m_Data);
}

/// Returns a modifiable pointer to the block
inline memlink::operator void* (void) const
{
    return (m_Data);
}

/// Returns a modifiable pointer to the block
inline memlink::iterator memlink::begin (void)
{
    return (iterator (m_Data));
}

/// Returns a const pointer to the block
inline memlink::const_iterator memlink::begin (void) const
{
    return (cmemlink::begin());
}

/// Returns a modifiable pointer to the end of the block
inline memlink::iterator memlink::end (void)
{
    return (begin() + size());
}

/// Returns a const pointer to the end of the block
inline memlink::const_iterator memlink::end (void) const
{
    return (cmemlink::end());
}

/// Copies from \p l.
inline void memlink::copy (const cmemlink& l)
{
    copy (begin(), l.cdata(), l.size());
}

/// Copies begin from \p p, \p n to the linked block.
inline void memlink::copy (const void* p, size_t n)
{
    copy (begin(), p, n);
}

/// Links to \p p of size \p n
inline void memlink::link (const void* p, size_t n)
{
    cmemlink::link (p, n);
}

/// Links to \p l
inline void memlink::link (const cmemlink& l)
{
    cmemlink::link (l);
}

/// Links to \p l
inline void memlink::link (memlink& l)
{
    link (l.begin(), l.size());
}

/// Reads object \p l from stream \p is
inline istream& operator>> (istream& is, memlink& l)
{
    l.read (is);
    return (is);
}

} // namespace ustl

#endif

