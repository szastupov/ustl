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
// memlink.cc
//
//	A pointer to a sized block of memory.
//

#include "mistream.h"
#include "memlink.h"
#include "ualgo.h"
#include <stdlib.h>

namespace ustl {

/// Initializes both links to point to NULL,0
memlink::memlink (void)
: cmemlink (),
  m_Data (NULL)
{
}

/// Initializes the const link to point to \p p, \p n. Non-const link is NULL.
memlink::memlink (const void* p, size_t n)
: cmemlink (p, n),
  m_Data (NULL)
{
}

/// Initializes both links to point to \p p, \p n
memlink::memlink (void* p, size_t n)
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

/// Reads the object from stream \p s
void memlink::read (istream& is)
{
    size_t n;
    is >> n;
    assert (n % elementSize() == 0 && "You are trying to read a block with different element type.");
    const size_t btr = min (n, size());
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (is.remaining() < btr)
	throw stream_bounds_exception ("read", "ustl::memlink", is.pos(), btr, is.remaining());
#else
    assert (btr <= is.remaining() && "Not enough data in the input stream");
#endif
    if (btr <= is.remaining()) {
	is.read (data(), btr);
	resize (btr);
	is.skip (n - btr);
    }
    is.align();
}

/// Copies data from \p p, \p n to the linked block starting at \p start.
void memlink::copy (iterator start, const void* p, size_t n)
{
    assert (m_Data || !n);
    assert (p || !n);
    assert (start >= begin() && start + n <= end());
    assert (n % elementSize() == 0 && "You are trying to write an incompatible element type");
    if (p && n && p != m_Data)
	copy_n (const_iterator(p), n, start);
}

/// Fills the linked block with the given pattern.
/// \arg start   Offset at which to start filling the linked block
/// \arg p       Pointer to the pattern.
/// \arg elSize  Size of the pattern.
/// \arg elCount Number of times to write the pattern.
/// Total number of bytes written is \p elSize * \p elCount.
///
void memlink::fill (iterator start, const void* p, size_t elSize, size_t elCount)
{
    assert (m_Data || !elCount || !elSize);
    assert (start >= begin() && start + elSize * elCount <= end());
    assert (elSize % elementSize() == 0 && "You are trying to write an incompatible element type");
    if (elSize == 1)
	fill_n (start, elCount, *reinterpret_cast<const u_char*>(p));
    else {
	while (elCount--)
	    start = copy_n (const_iterator(p), elSize, start);
    }
}

/// Shifts the data in the linked block from \p start to \p start + \p n.
/// The contents of the uncovered bytes is undefined.
void memlink::insert (iterator start, size_t n)
{
    assert (m_Data || !n);
    assert (cmemlink::begin() || !n);
    assert (start >= begin() && start + n <= end());
    assert (n % elementSize() == 0 && "You are trying to write an incompatible element type");
    assert (distance(begin(), start) % elementSize() == 0 && "You are trying to write in the middle of an element");
    rotate (start, end() - n, end());
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
/// The contents of the uncovered bytes is undefined.
void memlink::erase (iterator start, size_t n)
{
    assert (m_Data || !n);
    assert (cmemlink::begin() || !n);
    assert (start >= begin() && start + n <= end());
    assert (n % elementSize() == 0 && "You are trying to write an incompatible element type");
    assert (distance(begin(), start) % elementSize() == 0 && "You are trying to write in the middle of an element");
    rotate (start, start + n, end());
}

/// Override to initialize malloc'ed space, like calling constructors, for example.
void memlink::constructBlock (void* p, size_t n) const
{
    assert (n % elementSize() == 0 && "You are trying to write an incompatible element type");
    fill_n (p, n, u_char(0));
}

#ifndef NDEBUG
/// Override to deinitialize malloc'ed space, like calling destructors, for example.
void memlink::destructBlock (void* p, size_t n) const
{
    assert (n % elementSize() == 0 && "You are trying to write an incompatible element type");
    fill_n (p, n, u_char(0xCD));
}
#else
void memlink::destructBlock (void*, size_t) const
{
}
#endif

} // namespace ustl

