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
// cmemlink.cc
//
// See cmemlink.h for documentation.
//

#include "cmemlink.h"
#include <stdlib.h>
#include <memory.h>
#include "mostream.h"
#include "strmsize.h"
#include "ualgo.h"
#include "uexception.h"
#include <fcntl.h>
#include <unistd.h>

namespace ustl {

/// Default constructor initializes to point to NULL,0
cmemlink::cmemlink (void)
: m_CData (NULL),
  m_Size (0)
{
}

/// Attaches the object to pointer \p p of size \p n.
cmemlink::cmemlink (const void* p, size_t n)
: m_CData (p),
  m_Size (n)
{
    assert (p || !n);
}

/// Copies values from l
cmemlink::cmemlink (const cmemlink& l)
: m_CData (l.m_CData),
  m_Size (l.m_Size)
{
}

/// Destructor does nothing
cmemlink::~cmemlink (void)
{
}

/// Attaches the object to pointer \p p of size \p n.
void cmemlink::link (const void* p, size_t n)
{
    unlink();
    assert (p || !n);
    m_CData = p;
    m_Size = n;
}

/// Resets all to 0
void cmemlink::unlink (void)
{
    m_CData = NULL;
    m_Size = 0;
}

/// Writes the object to stream \p os
void cmemlink::write (ostream& os) const
{
    os << size();
    os.write (cdata(), size());
    os.align();
}

/// Returns the number of bytes required to write this object to a stream.
size_t cmemlink::stream_size (void) const
{
    return (Align (stream_size_of(m_Size) + size()));
}

/// Writes the data to file \p "filename".
void cmemlink::write_file (const char* filename, int mode) const
{
    int fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fd < 0)
	throw file_exception ("open", filename);
    ssize_t bw = ::write (fd, cdata(), size());
    if (size_t(bw) != size()) {
	close (fd);
	throw file_exception ("write", filename);
    }
    if (0 != close (fd))
	throw file_exception ("close", filename);
}

/// Copies values from l
const cmemlink& cmemlink::operator= (const cmemlink& l)
{
    m_CData = l.m_CData;
    m_Size = l.m_Size;
    return (*this);
}

/// swaps the contents with \p l
void cmemlink::swap (cmemlink& l)
{
    ::ustl::swap (m_CData, l.m_CData);
    ::ustl::swap (m_Size, l.m_Size);
}

/// Compares to memory block pointed by l. Size is compared first.
bool cmemlink::operator== (const cmemlink& l) const
{
    return (l.m_Size == m_Size &&
	    (l.m_CData == m_CData || 0 == memcmp (l.m_CData, m_CData, m_Size)));
}

} // namespace ustl

