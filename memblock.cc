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
#include "umemory.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

namespace ustl {

/// Allocates 0 bytes for the internal block.
memblock::memblock (void)
: memlink (),
  m_AllocatedSize (0)
{
}

/// Allocates \p n bytes for the internal block.
memblock::memblock (size_t n)
: memlink (),
  m_AllocatedSize (0)
{
    resize (n);
}

/// links to \p p, \p n. Data can not be modified and will not be freed.
memblock::memblock (const void* p, size_t n)
: memlink (),
  m_AllocatedSize (0)
{
    assign (p, n);
}

/// Links to what \p b is linked to.
memblock::memblock (const cmemlink& b)
: memlink (),
  m_AllocatedSize (0)
{
    assign (b);
}

/// Links to what \p b is linked to.
memblock::memblock (const memlink& b)
: memlink (),
  m_AllocatedSize (0)
{
    assign (b);
}

/// Links to what \p b is linked to.
memblock::memblock (const memblock& b)
: memlink (),
  m_AllocatedSize (0)
{
    assign (b);
}

/// Frees internal data, if appropriate
/// Only if the block was allocated using resize, or linked to using Manage,
/// will it be freed. Also, Derived classes should call DestructBlock from
/// their destructor, because upstream virtual functions are unavailable at
/// this point and will not be called automatically.
///
memblock::~memblock (void)
{
    if (!is_linked())
	deallocate();
}

/// Frees internal data.
void memblock::deallocate (void)
{
    if (m_AllocatedSize) {
	assert (data() && cdata());
	destructBlock (data(), m_AllocatedSize);
	free (data());
    }
    memblock::unlink();
}

/// Assumes control of the memory block \p p of size \p n.
/// The block assigned using this function will be freed in the destructor.
void memblock::manage (void* p, size_t n)
{
    assert (p || !n);
    assert (!data() || !m_AllocatedSize);	// Can't link to an allocated block.
    assert (n % elementSize() == 0 && "You are trying to manage a block with an incompatible element type");
    link (p, n);
    m_AllocatedSize = n;
}

/// Copies data from \p p, \p n.
void memblock::assign (const void* p, size_t n)
{
    resize (n);
    copy (p, n);
}

/// Reallocates internal block to hold at least \p newSize bytes. Some
/// additional memory may be allocated, but for efficiency it is a very
/// good idea to call reserve before doing byte-by-byte edit operations.
/// The block size as returned by size() is not altered. reserve will not
/// reduce allocated memory. If you think you are wasting space, call
/// deallocate and start over. To avoid wasting space, use the block for
/// only one purpose, and try to get that purpose to use similar amounts
/// of memory on each iteration.
///
void memblock::reserve (size_t newSize, bool bExact)
{
    if (m_AllocatedSize >= newSize)
	return;
    void* oldBlock = is_linked() ? NULL : data();
    if (!bExact)
	newSize = Align (newSize, Align (c_PageSize, elementSize()));
    assert (newSize % elementSize() == 0 && "reserve can only allocate units of elementType.");
    pointer newBlock = (pointer) realloc (oldBlock, newSize);
    if (!newBlock)
	throw bad_alloc (newSize);
    constructBlock (advance (newBlock, m_AllocatedSize), newSize - m_AllocatedSize);
    if (!oldBlock && cdata())
	copy_n (cdata(), min (size(), newSize), newBlock);
    link (newBlock, size());
    m_AllocatedSize = newSize;
}

/// Shifts the data in the linked block from \p start to \p start + \p n.
memblock::iterator memblock::insert (iterator start, size_t n)
{
    const uoff_t ip = start - begin();
    assert (ip <= size());
    reserve (size() + n, false);
    memlink::resize (size() + n);
    memlink::insert (begin() + ip, n);
    return (begin() + ip);
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
memblock::iterator memblock::erase (iterator start, size_t n)
{
    const uoff_t ep = start - begin();
    assert (ep + n <= size());
    memlink::erase (begin() + ep, n);
    reserve (size() - n, false);
    memlink::resize (size() - n);
    return (begin() + ep);
}

/// Unlinks object.
void memblock::unlink (void)
{
    memlink::unlink();
    m_AllocatedSize = 0;
}

/// Reads the object from stream \p s
void memblock::read (istream& is)
{
    size_t n;
    is >> n;
    if (is.remaining() < n)
	throw stream_bounds_exception ("read", "ustl::memblock", is.pos(), n, is.remaining());
    resize (n);
    is.read (data(), writable_size());
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
    const size_t btr = writable_size();
    ssize_t br = ::read (fd, data(), btr);
    if (size_t(br) != btr) {
	close (fd);
	throw file_exception ("read", filename);
    }
    close (fd);
}

} // namespace ustl

