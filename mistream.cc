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
// mstream.cpp
//
//	Helper classes to read and write packed binary streams.
//

#include "mistream.h"
#include "mostream.h"
#include "ulimits.h"
#include "ualgo.h"
#include <memory.h>

namespace ustl {

//--------------------------------------------------------------------

/// \brief Constructs a stream attached to nothing.
/// A stream attached to nothing is not usable. Call Link() functions
/// inherited from cmemlink to attach to some memory block.
///
istream::istream (void)
: cmemlink (),
  m_Pos (0)
{
}

/// Attaches the stream to a block at \p p of size \p n.
istream::istream (const void* p, size_t n)
: cmemlink (p, n),
  m_Pos (0)
{
}

/// Attaches to the block pointed to by \p source.
istream::istream (const cmemlink& source)
: cmemlink (source),
  m_Pos (0)
{
}

/// Swaps contents with \p is
void istream::swap (istream& is)
{
    cmemlink::swap (is);
    ::ustl::swap (m_Pos, is.m_Pos);
}

/// Reads \p n bytes into \p buffer.
void istream::read (void* buffer, size_t n)
{
    assert (remaining() >= n);
    memcpy (buffer, begin() + pos(), n);
    skip (n);
}

/// No reading into an input stream allowed, so this is a NOP.
void istream::read (istream&)
{
}

/// Writes all unread bytes into \p os.
void istream::write (ostream& os) const
{
    os.write (begin() + pos(), remaining());
}

/// Returns number of unread bytes.
size_t istream::stream_size (void) const
{
    return (remaining());
}

/// Links to \p p of size \p n
void istream::unlink (void)
{
    cmemlink::unlink();
    m_Pos = 0;
}

//--------------------------------------------------------------------

/// \brief Constructs a stream attached to nothing.
/// A stream attached to nothing is not usable. Call Link() functions
/// inherited from memlink to attach to some memory block.
///
ostream::ostream (void)
: memlink (),
  m_Pos (0)
{
}

/// Attaches the stream to a block at \p p of size \p n.
ostream::ostream (void* p, size_t n)
: memlink (p, n),
  m_Pos (0)
{
}

/// Attaches to the block pointed to by \p source.
ostream::ostream (const memlink& source)
: memlink (source),
  m_Pos (0)
{
}

/// Links to \p p of size \p n
void ostream::unlink (void)
{
    memlink::unlink();
    m_Pos = 0;
}

/// Writes \p n bytes from \p buffer.
void ostream::write (const void* buffer, size_t n)
{
    assert (remaining() >= n);
    memcpy (begin() + pos(), buffer, n);
    skip (n);
}

/// Equivalent to istream::write(os)
void ostream::read (istream& is)
{
    is.write (*this);
    is.seek (is.size());
}

/// Copy currently written bytes into \p os.
void ostream::write (ostream& os) const
{
    os.write (begin(), pos());
}

/// Inserts an empty area of \p size, at \p start.
void ostream::insert (iterator start, size_t s)
{
    memlink::insert (start, s);
    m_Pos += s;
}

/// Erases an area of \p size, at \p start.
void ostream::erase (iterator start, size_t s)
{
    m_Pos -= s;
    memlink::erase (start, s);
}

/// Swaps with \p os
void ostream::swap (ostream& os)
{
    memlink::swap (os);
    ::ustl::swap (m_Pos, os.m_Pos);
}

/// Returns number of bytes written.
size_t ostream::stream_size (void) const
{
    return (pos());
}

//--------------------------------------------------------------------

} // namespace ustl

