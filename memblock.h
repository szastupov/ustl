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
// memblock.h
//

#ifndef MEMBLOCK_H_7ED63A891164CC43578E63664D52A196
#define MEMBLOCK_H_7ED63A891164CC43578E63664D52A196

#include "memlink.h"

namespace ustl {

/// \class memblock memblock.h ustl.h
/// \ingroup MemoryManagement
///
/// \brief Allocated memory block.
///
/// Adds memory management capabilities to memlink. Uses malloc and realloc to
/// maintain the internal pointer, but only if allocated using members of this class,
/// or if linked to using the Manage() member function. Managed memory is automatically
/// freed in the destructor.
///
class memblock : public memlink {
public:
    static const size_type	c_PageSize = 64;	///< The default minimum allocation unit.
public:
				memblock (void);
				memblock (const void* p, size_type n);
    explicit			memblock (size_type n);
    explicit			memblock (const cmemlink& b);
    explicit			memblock (const memlink& b);
				memblock (const memblock& b);
    virtual		       ~memblock (void);
    inline const memblock&	operator= (const cmemlink& b);
    inline const memblock&	operator= (const memlink& b);
    inline const memblock&	operator= (const memblock& b);
    void			assign (const void* p, size_type n);
    inline void			assign (const cmemlink& l);
    void			swap (memblock& l);
    void			reserve (size_type newSize, bool bExact = true);
    inline void			resize (size_type newSize, bool bExact = true);
    iterator			insert (iterator start, size_type size);
    iterator			erase (iterator start, size_type size);
    void			pop_back (void);
    inline void			clear (void);
    void			deallocate (void);
    void			manage (void* p, size_type n);
    inline void			manage (memlink& l);
    inline size_type		capacity (void) const;
    virtual void		unlink (void);
    inline size_type		max_size (void) const;
    void			read (istream& is);
    void			read_file (const char* filename);
    inline bool			is_linked (void) const;
protected:
    virtual size_type		minimumFreeCapacity (void) const;
private:
    size_type			m_AllocatedSize;	///< Number of bytes allocated by Resize.
};

/// Copies data from \p l.
inline void memblock::assign (const cmemlink& l)
{
    assign (l.cdata(), l.readable_size());
}

/// Allocates enough space and copies the contents of \p b.
inline const memblock& memblock::operator= (const cmemlink& l)
{
    assign (l);
    return (*this);
}

/// Allocates enough space and copies the contents of \p b.
inline const memblock& memblock::operator= (const memlink& l)
{
    assign (l);
    return (*this);
}

/// Allocates enough space and copies the contents of \p b.
inline const memblock& memblock::operator= (const memblock& l)
{
    assign (l);
    return (*this);
}

/// Returns the number of bytes allocated.
inline memblock::size_type memblock::capacity (void) const
{
    return (m_AllocatedSize);
}

/// resizes the block to \p newSize bytes, reallocating if necessary.
inline void memblock::resize (size_type newSize, bool bExact)
{
    if (m_AllocatedSize < newSize + 1)
	reserve (newSize, bExact);
    memlink::resize (newSize);
}

/// Assumes control over block pointed to by \p l
inline void memblock::manage (memlink& l)
{
    manage (l.begin(), l.size());
}

/// Returns the maximum possible size of the block
inline memblock::size_type memblock::max_size (void) const
{
    return (SIZE_MAX / elementSize());
}

/// Returns true if the storage is linked, false if allocated.
inline bool memblock::is_linked (void) const
{
    return (!m_AllocatedSize && cdata());
}

/// Resizes the block to 0
inline void memblock::clear (void)
{
    resize (0);
}

/// Reads object \p l from stream \p is
inline istream& operator>> (istream& is, memblock& l)
{
    l.read (is);
    return (is);
}

} // namespace ustl

#endif

