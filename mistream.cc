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
#include "ustring.h"
#include "ualgo.h"

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

/// Attaches to the block pointed to by source of size source.pos()
istream::istream (const ostream& source)
: cmemlink (source.begin(), source.pos()),
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
    assert (remaining() >= n && "Reading past end of buffer. Make sure you are reading the right format.");
    copy_n (ipos(), n, reinterpret_cast<value_type*>(buffer));
    skip (n);
}

/// Reads a null-terminated string into \p str.
void istream::read_strz (string& str)
{
    const_iterator zp = find (ipos(), end(), string::c_Terminator);
    if (zp == end())
	zp = ipos();
    const size_t strl = distance (ipos(), zp);
    str.resize (strl);
    copy (ipos(), zp, str.begin());
    skip (strl + 1);
}

/// Reads \p buf.size() bytes into \p buf.
void istream::read (memlink& buf)
{
    read (buf.begin(), buf.size());
}

/// No reading into an input stream allowed, so this is a NOP.
void istream::read (istream&)
{
}

/// Writes all unread bytes into \p os.
void istream::write (ostream& os) const
{
    os.write (ipos(), remaining());
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
    assert (remaining() >= n && "Buffer overrun. Check your stream size calculations.");
    copy_n (const_iterator(buffer), n, ipos());
    skip (n);
}

/// Writes \p str as a null-terminated string.
void ostream::write_strz (const char* str)
{
    write (str, strlen(str));
    iwrite (string::c_Terminator);
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

//--------------------------------------------------------------------

}; // namespace ustl

