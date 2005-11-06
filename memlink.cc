// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// memlink.cc
//
//	A pointer to a sized block of memory.
//

#include "mistream.h"
#include "memlink.h"
#include "ualgo.h"

namespace ustl {

/// Initializes both links to point to NULL,0
memlink::memlink (void)
: cmemlink (),
  m_Data (NULL)
{
}

/// Initializes the const link to point to \p p, \p n. Non-const link is NULL.
memlink::memlink (const void* p, size_type n)
: cmemlink (p, n),
  m_Data (NULL)
{
}

/// Initializes both links to point to \p p, \p n
memlink::memlink (void* p, size_type n)
: cmemlink (p, n),
  m_Data (reinterpret_cast<pointer>(p))
{
}

/// Copies information from \p l
memlink::memlink (const cmemlink& l)
: cmemlink (l),
  m_Data (NULL)
{
}

/// Copies information from \p l
memlink::memlink (const memlink& l)
: cmemlink (l),
  m_Data (l.m_Data)
{
}

/// Copies information from \p l
const memlink& memlink::operator= (const cmemlink& l)
{
    cmemlink::operator= (l);
    m_Data = NULL;
    return (*this);
}

/// Copies information from \p l
const memlink& memlink::operator= (const memlink& l)
{
    cmemlink::operator= (l);
    m_Data = l.m_Data;
    return (*this);
}

/// Exchanges the contents with \p l
void memlink::swap (memlink& l)
{
    cmemlink::swap (l);
    ::ustl::swap (m_Data, l.m_Data);
}

/// Returns the size of the writable area
memlink::size_type memlink::writable_size (void) const
{
    return (m_Data ? size() : 0);
}

/// Initializes both links to point to \p p, \p n
void memlink::link (void* p, size_type n)
{
    cmemlink::link (p, n);
    m_Data = reinterpret_cast<pointer>(p);
}

/// Resets all members to 0
void memlink::unlink (void)
{
    cmemlink::unlink();
    m_Data = NULL;
}

/// Reads the object from stream \p s
void memlink::read (istream& is)
{
    size_type n;
    is >> n;
    const size_type btr = min (n, size());
    if (is.remaining() < btr)
	throw stream_bounds_exception ("read", "ustl::memlink", is.pos(), btr, is.remaining());
    is.read (data(), btr);
    resize (btr);
    is.skip (n - btr);
    is.align();
}

/// Copies data from \p p, \p n to the linked block starting at \p start.
void memlink::copy (iterator start, const void* p, size_type n)
{
    assert (data() || !n);
    assert (p || !n);
    assert (start >= begin() && start + n <= end());
    if (p && n && p != data())
	copy_n (const_iterator(p), n, start);
}

/// Fills the linked block with the given pattern.
/// \arg start   Offset at which to start filling the linked block
/// \arg p       Pointer to the pattern.
/// \arg elSize  Size of the pattern.
/// \arg elCount Number of times to write the pattern.
/// Total number of bytes written is \p elSize * \p elCount.
///
void memlink::fill (iterator start, const void* p, size_type elSize, size_type elCount)
{
    assert (data() || !elCount || !elSize);
    assert (start >= begin() && start + elSize * elCount <= end());
    if (elSize == 1)
	fill_n (start, elCount, *reinterpret_cast<const uint8_t*>(p));
    else {
	while (elCount--)
	    start = copy_n (const_iterator(p), elSize, start);
    }
}

/// Shifts the data in the linked block from \p start to \p start + \p n.
/// The contents of the uncovered bytes is undefined.
void memlink::insert (iterator start, size_type n)
{
    assert (data() || !n);
    assert (cmemlink::begin() || !n);
    assert (start >= begin() && start + n <= end());
    rotate (start, end() - n, end());
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
/// The contents of the uncovered bytes is undefined.
void memlink::erase (iterator start, size_type n)
{
    assert (data() || !n);
    assert (cmemlink::begin() || !n);
    assert (start >= begin() && start + n <= end());
    rotate (start, start + n, end());
}

} // namespace ustl

