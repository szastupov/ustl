// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005-2009 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "mistream.h"
#include "sostream.h"
#include "ustring.h"
#include "ualgo.h"

namespace ustl {

//--------------------------------------------------------------------

/// Checks that \p n bytes are available in the stream, or else throws.
void ios_base::overrun (const char* op, const char* type, uint32_t n, uint32_t pos, uint32_t rem)
{
    if (set_and_throw (rem ? failbit : (failbit | eofbit)))
	throw stream_bounds_exception (op, type, pos, n, rem);
}

//--------------------------------------------------------------------

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
void istream::read (void* buffer, size_type n)
{
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (!verify_remaining ("read", "binary data", n))
	return;
#else
    assert (remaining() >= n && "Reading past end of buffer. Make sure you are reading the right format.");
#endif
    copy_n (ipos(), n, reinterpret_cast<value_type*>(buffer));
    m_Pos += n;
}

/// Reads a null-terminated string into \p str.
void istream::read_strz (string& str)
{
    const_iterator zp = find (ipos(), end(), string::c_Terminator);
    if (zp == end())
	zp = ipos();
    const size_type strl = distance (ipos(), zp);
    str.resize (strl);
    copy (ipos(), zp, str.begin());
    m_Pos += strl + 1;
}

/// Reads at most \p n bytes into \p s.
istream::size_type istream::readsome (void* s, size_type n)
{
    if (remaining() < n)
	underflow (n);
    const size_type ntr (min (n, remaining()));
    read (s, ntr);
    return (ntr);
}

/// Writes all unread bytes into \p os.
void istream::write (ostream& os) const
{
    os.write (ipos(), remaining());
}

/// Writes the object to stream \p os.
void istream::text_write (ostringstream& os) const
{
    os.write (ipos(), remaining());
}

/// Links to \p p of size \p n
void istream::unlink (void) throw()
{
    cmemlink::unlink();
    m_Pos = 0;
}

//--------------------------------------------------------------------

/// Links to \p p of size \p n
void ostream::unlink (void) throw()
{
    memlink::unlink();
    m_Pos = 0;
}

/// Aligns the write pointer on \p grain. The skipped bytes are zeroed.
void ostream::align (size_type grain)
{
    const size_t r = pos() % grain;
    size_t nb = grain - r;
#if WANT_STREAM_BOUNDS_CHECKING
    if (!r || !verify_remaining ("align", "padding", nb))
	return;
#else
    if (!r) nb = 0;
    assert (remaining() >= nb && "Buffer overrun. Check your stream size calculations.");
#endif
    memset (ipos(), '\x0', nb);
    m_Pos += nb;
}

/// Writes \p n bytes from \p buffer.
void ostream::write (const void* buffer, size_type n)
{
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (!verify_remaining ("write", "binary data", n))
	return;
#else
    assert (remaining() >= n && "Buffer overrun. Check your stream size calculations.");
#endif
    copy_n (const_iterator(buffer), n, ipos());
    m_Pos += n;
}

/// Writes \p str as a null-terminated string.
void ostream::write_strz (const char* str)
{
    write (str, strlen(str));
    iwrite (string::c_Terminator);
}

/// Writes all available data from \p is.
void ostream::read (istream& is)
{
    is.write (*this);
    is.seek (is.size());
}

/// Writes all written data to \p os.
void ostream::text_write (ostringstream& os) const
{
    os.write (begin(), pos());
}

/// Inserts an empty area of \p size, at \p start.
void ostream::insert (iterator start, size_type s)
{
    memlink::insert (start, s);
    m_Pos += s;
}

/// Erases an area of \p size, at \p start.
void ostream::erase (iterator start, size_type s)
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

} // namespace ustl
