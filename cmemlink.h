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

#ifndef CMEMLINK_H_7CFAB32C5C6732ED29B34EF00EA40A12
#define CMEMLINK_H_7CFAB32C5C6732ED29B34EF00EA40A12

#include "ualgobase.h"

/// The ustl namespace contains all ustl classes and algorithms.
namespace ustl {

class istream;
class ostream;

/// \class cmemlink cmemlink.h ustl.h
/// \ingroup MemoryManagement
///
/// \brief A read-only pointer to a sized block of memory.
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
    typedef char		value_type;
    typedef const value_type*	pointer;
    typedef const value_type*	const_pointer;
    typedef value_type		reference;
    typedef value_type		const_reference;
    typedef size_t		size_type;
    typedef ptrdiff_t		difference_type;
    typedef const_pointer	const_iterator;
    typedef const_iterator	iterator;
public:
			cmemlink (void);
			cmemlink (const void* p, size_type n);
			cmemlink (const cmemlink& l);
    inline virtual     ~cmemlink (void) {}
    void		link (const void* p, size_type n);
    inline void		link (const cmemlink& l);
    virtual void	unlink (void);
    const cmemlink&	operator= (const cmemlink& l);
    bool		operator== (const cmemlink& l) const;
    void		swap (cmemlink& l);
   inline const_pointer	cdata (void) const;
    inline iterator	begin (void) const;
    inline iterator	end (void) const;
    inline size_type	size (void) const;
    inline size_type	max_size (void) const;
    virtual size_type	readable_size (void) const;
    inline bool		empty (void) const;
    inline void		resize (size_type n);
    inline void		read (istream&);
    void		write (ostream& os) const;
    size_type		stream_size (void) const;
    void		write_file (const char* filename, int mode = 0644) const;
protected:
    virtual size_type	elementSize (void) const;
    inline size_type	elementBytes (size_type n) const	{ return (n * elementSize()); }
private:
    const_pointer	m_CData;	///< Pointer to the data block (const)
    size_type		m_Size;		///< size of the data block
};

/// Returns the pointer to the internal data
inline cmemlink::const_pointer cmemlink::cdata (void) const
{
    return (m_CData);
}

/// Returns the size of the block
inline cmemlink::size_type cmemlink::size (void) const
{
    return (m_Size);
}

/// Returns the maximum size of the block (non-resizable, so always == size())
inline cmemlink::size_type cmemlink::max_size (void) const
{
    return (m_Size);
}

/// true if size() == 0
inline bool cmemlink::empty (void) const
{
    return (size() == 0);
}

/// Resizes the block as seen by users of the class (no memory allocation)
inline void cmemlink::resize (size_type n)
{
    m_Size = n;
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

/// Links to \p l
inline void cmemlink::link (const cmemlink& l)
{
    link (l.begin(), l.size());
}

/// Reads the object from stream \p os
inline void cmemlink::read (istream&)
{
    assert (false && "ustl::cmemlink is a read-only object.");
}

} // namespace ustl

#endif

