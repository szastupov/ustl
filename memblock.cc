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
// memblock.cc
//
//	Allocated memory block.
//

#include "mistream.h"
#include "memblock.h"
#include "ualgo.h"
#include "uexception.h"
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

namespace ustl {

/// Allocates 0 bytes for the internal block.
memblock::memblock (void)
: memlink (),
  m_AllocatedSize (0),
  m_PageSize (c_DefaultPageSize)
{
}

/// Allocates \p n bytes for the internal block.
memblock::memblock (size_t n)
: memlink (),
  m_AllocatedSize (0),
  m_PageSize (c_DefaultPageSize)
{
    resize (n);
}

/// links to \p p, \p n. Data can be modified but will not be freed.
memblock::memblock (void* p, size_t n)
: memlink (p, n),
  m_AllocatedSize (0),
  m_PageSize (c_DefaultPageSize)
{
}

/// links to \p p, \p n. Data can not be modified and will not be freed.
memblock::memblock (const void* p, size_t n)
: memlink (p, n),
  m_AllocatedSize (0),
  m_PageSize (c_DefaultPageSize)
{
}

/// Links to what \p b is linked to.
memblock::memblock (const cmemlink& b)
: memlink (b),
  m_AllocatedSize (0),
  m_PageSize (c_DefaultPageSize)
{
}

/// Allocates enough space and copies the contents of \p b.
memblock::memblock (const memblock& b)
: memlink (),
  m_AllocatedSize (0),
  m_PageSize (b.m_PageSize)
{
    resize (b.size());
    copy (b.begin(), b.size());
}

/// Frees internal data, if appropriate
/// Only if the block was allocated using resize, or linked to using Manage,
/// will it be freed. Also, Derived classes should call DestructBlock from
/// their destructor, because upstream virtual functions are unavailable at
/// this point and will not be called automatically.
///
memblock::~memblock (void)
{
    deallocate();
}

/// Links to what \p b links to.
const memblock& memblock::operator= (const cmemlink& b)
{
    deallocate();
    memlink::operator= (b);
    return (*this);
}

/// Links to what \p b links to.
const memblock& memblock::operator= (const memlink& b)
{
    deallocate();
    memlink::operator= (b);
    return (*this);
}

/// Allocates enough space and copies the contents of \p b.
const memblock& memblock::operator= (const memblock& b)
{
    if (is_linked())
	unlink();
    m_PageSize = b.m_PageSize;
    resize (b.size());
    copy (b.begin(), b.size());
    return (*this);
}

/// Frees internal data.
void memblock::deallocate (void)
{
    if (m_AllocatedSize) {
	assert (data() && cdata());
	if (m_PageSize > c_DefaultPageSize)
	    m_PageSize /= 2;
	destructBlock (data(), m_AllocatedSize);
	free (data());
    }
    unlink();
}

/// Assumes control of the memory block \p p of size \p n.
/// The block assigned using this function will be freed in the destructor.
void memblock::manage (void* p, size_t n)
{
    assert (p || !n);
    assert (!data() || !m_AllocatedSize);	// Can't link to an allocated block.
    link (p, n);
    m_AllocatedSize = n;
}

/// Unlinks object.
void memblock::unlink (void)
{
    memlink::unlink();
    m_AllocatedSize = 0;
}

/// Copies data from \p p, \p n to the linked block starting at \p start.
void memblock::assign (const cmemlink& l)
{
    if (is_linked())
	unlink();
    resize (l.size());
    copy (l.cdata(), l.size());
}

/// Swaps the contents with \p l
void memblock::swap (memblock& l)
{
    memlink::swap (l);
    ::ustl::swap (m_AllocatedSize, l.m_AllocatedSize);
    ::ustl::swap (m_PageSize, l.m_PageSize);
}

/// Reallocates internal block to hold at least \p newSize bytes
/// Paged allocation is performed with minimum allocation unit
/// defined by variable m_PageSize. The block size as returned by
/// size() is not altered.
///
void memblock::reserve (size_t newSize)
{
    if ((newSize < m_AllocatedSize && newSize + c_MinimumShrinkSize > m_AllocatedSize) ||
	is_linked() || !newSize)
	return;
    size_t alignedSize = Align (newSize, m_PageSize);
    if (alignedSize > m_AllocatedSize)
	m_PageSize *= 2;
    else {
	if (m_PageSize > c_DefaultPageSize)
	    m_PageSize /= 2;
	destructBlock (begin() + alignedSize, m_AllocatedSize - alignedSize);
	m_AllocatedSize = alignedSize; // To retain sanity in case of failure.
    }
    void* newBlock = realloc (data(), alignedSize);
    if (!newBlock)
	throw bad_alloc(alignedSize);
    if (alignedSize > m_AllocatedSize)
	constructBlock (advance (newBlock, m_AllocatedSize), alignedSize - m_AllocatedSize);
    link (newBlock, size());
    m_AllocatedSize = alignedSize;
}

/// resizes the block to \p newSize bytes, reallocating if necessary.
void memblock::resize (size_t newSize)
{
    reserve (newSize);
    memlink::resize (newSize);
}

/// Shifts the data in the linked block from \p start to \p start + \p n.
memblock::iterator memblock::insert (iterator start, size_t n)
{
    const uoff_t ip = start - begin();
    assert (ip <= size());
    resize (size() + n);
    memlink::insert (begin() + ip, n);
    return (begin() + ip);
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
memblock::iterator memblock::erase (iterator start, size_t n)
{
    const uoff_t ep = start - begin();
    assert (ep <= size());
    memlink::erase (begin() + ep, n);
    resize (size() - n);
    return (begin() + ep);
}

/// Reads the object from stream \p s
void memblock::read (istream& is)
{
    size_t n;
    is >> n;
    resize (n);
    is.read (data(), size());
    is.align();
}

/// Reads the entire file \p "filename".
void memblock::read_file (const char* filename)
{
    struct stat st;
    if (stat (filename, &st))
	throw file_exception ("stat", filename);
    resize (st.st_size);
    int fd = open (filename, O_RDONLY);
    if (fd < 0)
	throw file_exception ("open", filename);
    ssize_t br = ::read (fd, data(), size());
    if (size_t(br) != size()) {
	close (fd);
	throw file_exception ("read", filename);
    }
    close (fd);
}

} // namespace ustl

