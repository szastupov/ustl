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
// sostream.h
//

#include "sostream.h"
#include "ustring.h"
#include <stdio.h>	// for vsnprintf in ostringstream::format
#include <stdarg.h>

namespace ustl {

/// Default constructor.
ostringstream::ostringstream (void)
: ostream (),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
}

/// Creates a stream for writing into \p p of size \p n.
ostringstream::ostringstream (void* p, size_t n)
: ostream (p, n),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
}

/// Creates a stream for writing into string \p dest.
///
/// dest may be resized by the stream if insufficient space is available.
///
ostringstream::ostringstream (string& dest)
: ostream (dest.begin(), dest.size()),
  m_pResizable (&dest),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
}

/// Creates a stream for writing into fixed block \p dest.
ostringstream::ostringstream (const memlink& dest)
: ostream (dest),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
}

const char c_Digits[] = "0123456789ABCDEF";

/// Writes \p buf of size \p bufSize through the internal buffer.
void ostringstream::write_buffer (const char* buf, size_t bufSize)
{
    size_t btw = 0, written = 0;
    while ((written += btw) < bufSize && (remaining() || overflow(bufSize - written)))
	write (buf + written, btw = min (remaining(), bufSize - written));
}

/// Writes a single character into the stream.
ostringstream& ostringstream::operator<< (u_char v)
{
    if (remaining() >= sizeof(u_char) || overflow() >= sizeof(u_char))
	ostream::iwrite (v);
    return (*this);
}

/// Writes long value \p sv into the stream.
ostringstream& ostringstream::operator<< (long sv)
{
    assert (m_Base < VectorSize(c_Digits));
    const bool negative = (sv < 0);
    u_long v (negative ? -sv : sv);
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    do {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    } while (v);
    if (negative)
	buffer[i--] = '-';
    ++ i;
    write_buffer (buffer + i, c_BufSize - i - 1);
    return (*this);
}

/// Writes number \p v into the stream as text.
ostringstream& ostringstream::operator<< (u_long v)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    do {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    } while (v);
    ++ i;
    write_buffer (buffer + i, c_BufSize - i - 1);
    return (*this);
}

#ifdef __GNUC__
/// Writes number \p v into the stream as text.
ostringstream& ostringstream::operator<< (long long sv)
{
    assert (m_Base < VectorSize(c_Digits));
    const bool negative = (sv < 0);
    u_long v (negative ? -sv : sv);
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    do {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    } while (v);
    if (negative)
	buffer[i--] = '-';
    ++ i;
    write_buffer (buffer + i, c_BufSize - i - 1);
    return (*this);
}

/// Writes number \p v into the stream as text.
ostringstream& ostringstream::operator<< (unsigned long long v)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    do {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    } while (v);
    ++ i;
    write_buffer (buffer + i, c_BufSize - i - 1);
    return (*this);
}
#endif

/// Writes number \p iv into the stream as text.
ostringstream& ostringstream::operator<< (double iv)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(double) + 1;
    char buffer [c_BufSize];
    char fmt [16];
    assert (m_Precision < c_BufSize / 2);
    snprintf (fmt, 16, "%%.%dlf", m_Precision);
    size_t i = snprintf (buffer, c_BufSize, fmt, iv);
    if (i > c_BufSize)
	i = c_BufSize - 1;
    buffer [c_BufSize - 1] = 0;
    assert (i < c_BufSize);
    write_buffer (buffer, i);
    return (*this);
}

/// Writes value \p v into the stream as text.
ostringstream& ostringstream::operator<< (bool v)
{
    if (v)
	write_buffer ("true", 4);
    else
	write_buffer ("false", 5);
    return (*this);
}

/// Writes string \p s into the stream.
ostringstream& ostringstream::operator<< (const char* s)
{
    write_buffer (s, strlen(s));
    return (*this);
}

/// Writes string \p v into the stream.
ostringstream& ostringstream::operator<< (const string& v)
{
    write_buffer (v.begin(), v.size());
    return (*this);
}

/// Equivalent to a sprintf on the string.
int ostringstream::format (const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    int rv = vsnprintf (ipos(), remaining(), fmt, args);
    if (rv > 0) {
	if (rv > int(remaining()))
	    rv = (rv > int(overflow (rv)) ? 0 : vsnprintf (ipos(), remaining(), fmt, args));
	skip (rv);
    }
    va_end (args);
    return (rv);
}

/// Sets the flag \p f in the stream.
ostringstream& ostringstream::operator<< (ios::fmtflags f)
{
    switch (f) {
	case ios::oct:		set_base (8); break;
	case ios::dec:		set_base (10); break;
	case ios::hex:		set_base (16); break;
	case ios::left:
	    m_Flags |= ios::left;
	    m_Flags &= ~ios::right;
	    break;
	case ios::right:
	    m_Flags |= ios::right;
	    m_Flags &= ~ios::left;
	    break;
	case ios::boolalpha:
	case ios::fixed:
	case ios::internal:
	case ios::scientific:
	case ios::showbase:
	case ios::showpoint:
	case ios::showpos:
	case ios::skipws:
	case ios::unitbuf:
	case ios::uppercase:
	case ios::adjustfield:
	case ios::basefield:
	case ios::floatfield:
	    m_Flags |= f;
	    break;
    }
    return (*this);
}

/// Unlinks the stream from its bound buffer.
void ostringstream::unlink (void)
{
    ostream::unlink();
    m_pResizable = NULL;
}

/// Writes the contents of \p buffer of \p size into the stream.
void ostringstream::write (const void* buffer, size_t sz)
{
    if (remaining() < sz && overflow(sz) < sz)
	return;
    ostream::write (buffer, sz);
}

/// Writes the contents of \p buf into the stream.
void ostringstream::write (const cmemlink& buf)
{
    if (remaining() < buf.size() && overflow(buf.size()) < buf.size())
	return;
    ostream::write (buf);
}

/// Attempts to create more output space. Returns remaining().
size_t ostringstream::overflow (size_t n)
{
    if (!m_pResizable)
	return (0);
    assert (n > remaining() && "Don't call overflow if you don't need to");
    const uoff_t oldPos (pos());
    string* pResizable (m_pResizable);	// reset in unlink
    pResizable->resize (pos() + n);
    link (*pResizable);
    m_pResizable = pResizable;
    seek (oldPos);
    if (remaining() < n)
#ifdef WANT_STREAM_BOUNDS_CHECKING
	throw stream_bounds_exception ("write", "binary data", pos(), n, remaining());
#else
	assert (remaining() >= n && "Buffer overrun. Check your stream size calculations.");
#endif
    return (remaining());
}

}; // namespace ustl


