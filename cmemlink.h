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
// cmemlink.h
//

#ifndef CMEMLINK_H
#define CMEMLINK_H

#include "uutility.h"

/// The ustl namespace contains all ustl classes and algorithms.
namespace ustl {

class istream;
class ostream;

///
/// A read-only pointer to a sized block of memory.
///
/// Use this class the way you would a const pointer to an allocated unstructured block.
/// The pointer and block size are available through member functions and cast operator.
///
/// Example usage:
///
/// \code
///     void* p = malloc (46721);
///     cmemlink a, b;
///     a.link (p, 46721);
///     assert (a.size() == 46721));
///     b = a;
///     assert (b.size() == 46721));
///     assert (b.DataAt(34) == a.DataAt(34));
///     assert (0 == memcmp (a, b, 12));
/// \endcode
///
class cmemlink {
public:
    typedef void		value_type;
    typedef const value_type*	pointer;
    typedef const value_type*	const_pointer;
    typedef value_type		reference;
    typedef value_type		const_reference;
    typedef size_t		size_type;
    typedef ptrdiff_t		difference_type;
    /// A wrapper for void pointers to allow iterator functionality.
    class iterator {
    public:
	typedef void		value_type;
	typedef ptrdiff_t	difference_type;
	typedef const void*	pointer;
	typedef void		reference;
    public:
				iterator (pointer p = NULL) : m_p (p) {}
				iterator (const iterator& i) : m_p (i.m_p) {}
	inline const iterator&	operator= (const iterator& i) { m_p = i.m_p; return (*this); }
	inline 			operator const void* (void) const { return (m_p); }
	inline pointer		base (void) const { return (m_p); }
	inline const iterator&	operator++ (void) { m_p = advance (m_p, 1); return (*this); }
	inline const iterator&	operator-- (void) { m_p = advance (m_p, -1); return (*this); }
	inline const iterator&	operator+= (off_t n) { m_p = advance (m_p, n); return (*this); }
	inline const iterator&	operator-= (off_t n) { m_p = advance (m_p, -n); return (*this); }
	inline iterator		operator++ (int) { iterator old (*this); m_p = advance (m_p, 1); return (old); }
	inline iterator		operator-- (int) { iterator old (*this); m_p = advance (m_p, -1); return (old); }
	inline iterator		operator+ (off_t n) const { return (iterator (advance (m_p, n))); }
	inline iterator		operator- (off_t n) const { return (iterator (advance (m_p, -n))); }
	inline difference_type	operator- (const iterator& i) const { return (distance (m_p, i.m_p)); }
	inline bool		operator== (const iterator& i) const { return (m_p == i.m_p); }
	inline bool		operator< (const iterator& i) const { return (m_p < i.m_p); }
    private:
	pointer			m_p;
    };
    typedef iterator		const_iterator;
public:
			cmemlink (void);
			cmemlink (const void* p, size_t n);
			cmemlink (const cmemlink& l);
    virtual	       ~cmemlink (void);
    void		link (const void* p, size_t n);
    inline void		link (const cmemlink& l);
    virtual void	unlink (void);
    inline		operator const void* (void) const;
    const cmemlink&	operator= (const cmemlink& l);
    bool		operator== (const cmemlink& l) const;
    void		swap (cmemlink& l);
    inline const void*	cdata (void) const;
    inline iterator	begin (void) const;
    inline iterator	end (void) const;
    inline size_t	size (void) const;
    inline size_t	max_size (void) const;
    inline bool		empty (void) const;
    void		resize (size_t n);
    virtual void	read (istream&);
    virtual void	write (ostream& os) const;
    virtual size_t	stream_size (void) const;
    void		write_file (const char* filename, int mode = 0644) const;
private:
    const void*		m_CData;	///< Pointer to the data block (const)
    size_t		m_Size;		///< size of the data block
};

/// Links to \p l
inline void cmemlink::link (const cmemlink& l)
{
    link (l.begin(), l.size());
}

/// Returns the pointer to the internal data
inline const void* cmemlink::cdata (void) const
{
    return (m_CData);
}

/// Returns the pointer to the internal data
inline cmemlink::operator const void* (void) const
{
    return (m_CData);
}

/// Returns the pointer to the internal data
inline cmemlink::iterator cmemlink::begin (void) const
{
    return (iterator (m_CData));
}

/// Returns the pointer to the end of the internal data
inline cmemlink::iterator cmemlink::end (void) const
{
    return (begin() + size());
}

/// Returns the size of the block
inline size_t cmemlink::size (void) const
{
    return (m_Size);
}

/// Returns the maximum size of the block (non-resizable, so always == size())
inline size_t cmemlink::max_size (void) const
{
    return (m_Size);
}

/// true if size() == 0
inline bool cmemlink::empty (void) const
{
    return (size() == 0);
}

} // namespace ustl

#endif

