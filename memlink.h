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

#ifndef MEMLINK_H_798D25827C8E322D2D7E734B169FF5FC
#define MEMLINK_H_798D25827C8E322D2D7E734B169FF5FC

#include "cmemlink.h"

namespace ustl {

/// \class memlink memlink.h ustl.h
/// \ingroup MemoryManagement
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
    typedef pointer			iterator;
public:
			memlink (void);
			memlink (void* p, size_type n);
			memlink (const void* p, size_type n);
			memlink (const memlink& l);
    explicit		memlink (const cmemlink& l);
    inline void		link (const void* p, size_type n)	{ cmemlink::link (p, n); }
    void		link (void* p, size_type n);
    inline void		link (const cmemlink& l);
    inline void		link (memlink& l);
    inline void		link (const void* first, const void* last);
    inline void		link (void* first, void* last);
			OVERLOAD_POINTER_AND_SIZE_T_V2(link, void*)
			OVERLOAD_POINTER_AND_SIZE_T_V2(link, const void*)
    virtual void	unlink (void);
    inline void		copy (const cmemlink& l);
    inline void		copy (const void* p, size_type n);
    void		copy (iterator offset, const void* p, size_type n);
    inline pointer	data (void);
    size_type		writable_size (void) const;
    const memlink&	operator= (const cmemlink& l);
    const memlink&	operator= (const memlink& l);
    void		swap (memlink& l);
    inline iterator	begin (void);
    inline iterator	end (void);
    inline const_iterator	begin (void) const;
    inline const_iterator	end (void) const;
    void		fill (iterator start, const void* p, size_type elsize, size_type elCount = 1);
    void		insert (iterator start, size_type size);
    void		erase (iterator start, size_type size);
    void		read (istream& is);
protected:
    virtual void	constructBlock (void*, size_type) const;
    virtual void	destructBlock (void*, size_type) const;
private:
    pointer		m_Data;	///< Pointer to the begin block (non-const)
};

/// Returns a modifiable pointer to the block
inline memlink::pointer memlink::data (void)
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
    assert ((begin() || !l.size()) && "Can't copy into a constant link.");
    copy (begin(), l.cdata(), l.size());
}

/// Copies begin from \p p, \p n to the linked block.
inline void memlink::copy (const void* p, size_type n)
{
    assert ((begin() || !n) && "Can't copy into a constant link.");
    copy (begin(), p, n);
}

/// Links to \p l
inline void memlink::link (const cmemlink& l)
{
    cmemlink::link (l);
}

/// Links to \p l
inline void memlink::link (memlink& l)
{
    cmemlink::link (l);
    m_Data = l.data();
}

/// Links to iterator range \p first - \p last
inline void memlink::link (const void* first, const void* last)
{
    link (first, distance (first, last));
}

/// Links to iterator range \p first - \p last
inline void memlink::link (void* first, void* last)
{
    link (first, distance (first, last));
}

/// Reads object \p l from stream \p is
inline istream& operator>> (istream& is, memlink& l)
{
    l.read (is);
    return (is);
}

/// Use with memlink-derived classes to allocate and link to stack space. \p n is in elements, not bytes!
#define alloca_link(m,n)	(m).memlink::link (alloca ((m).elementBytes (n)), (n))

} // namespace ustl

#endif

