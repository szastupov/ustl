// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//
// sostream.h
//

#include "mistream.h"	// for istream_iterator, referenced in utf8.h
#include "sostream.h"
#include "ustring.h"
#include "utf8.h"
#include "ulimits.h"
#include <stdio.h>

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
ostringstream::ostringstream (void* p, size_type n)
: ostream (),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
    link (p, n);
}

/// Creates a stream for writing into string \p dest.
///
/// dest may be resized by the stream if insufficient space is available.
///
ostringstream::ostringstream (string& dest)
: ostream (),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
    link (dest);
}

/// Creates a stream for writing into fixed block \p dest.
ostringstream::ostringstream (memlink& dest)
: ostream (),
  m_pResizable (NULL),
  m_Flags (0),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (',')
{
    link (dest);
}

/// Writes a single character into the stream.
void ostringstream::iwrite (uint8_t v)
{
    if (remaining() >= sizeof(uint8_t) || overflow() >= sizeof(uint8_t))
	ostream::iwrite (v);
}

/// Writes \p buf of size \p bufSize through the internal buffer.
void ostringstream::write_buffer (const char* buf, size_type bufSize)
{
    size_type btw = 0, written = 0;
    while ((written += btw) < bufSize && (remaining() || overflow(bufSize - written)))
	write (buf + written, btw = min (remaining(), bufSize - written));
}

void ostringstream::fmtstring (char* fmt, const char* typestr, bool bInteger) const
{
    #define encode_dec(p,n)	{ *p++ = '0'+n/10; *p++ = '0'+n%10; }
    *fmt++ = '%';
    if (m_Width)
	encode_dec (fmt, m_Width);
    if (m_Flags & ios::left)
	*fmt++ = '-';
    if (!bInteger) {
	*fmt++ = '.';
	encode_dec (fmt, m_Precision);
    }
    while (*typestr)
	*fmt++ = *typestr++;
    if (bInteger) {
	if (m_Base == 16)
	    fmt[-1] = 'X';
	else if (m_Base == 8)
	    fmt[-1] = 'o';
    } else {
	if (m_Flags & ios::scientific)
	    fmt[-1] = 'E';
    }
    *fmt = 0;
}

template <typename T>
inline void ostringstream::sprintf_iwrite (T v, const char* typestr)
{
    const size_type c_BufSize = 64, c_FmtStrSize = 16;
    char fmt [c_FmtStrSize], buffer [c_BufSize];
    fmtstring (fmt, typestr, numeric_limits<T>::is_integer);
    size_type i = snprintf (buffer, c_BufSize, fmt, v);
    i = min (i, c_BufSize - 1);
    write_buffer (buffer, i);
}

void ostringstream::iwrite (int v)		{ sprintf_iwrite (v, "d"); }
void ostringstream::iwrite (unsigned int v)	{ sprintf_iwrite (v, "u"); }
void ostringstream::iwrite (long v)		{ sprintf_iwrite (v, "ld"); }
void ostringstream::iwrite (unsigned long v)	{ sprintf_iwrite (v, "lu"); }
void ostringstream::iwrite (float v)		{ simd::reset_mmx(); sprintf_iwrite (v, "f"); }
void ostringstream::iwrite (double v)		{ simd::reset_mmx(); sprintf_iwrite (v, "lf"); }
#if HAVE_LONG_LONG
void ostringstream::iwrite (long long v)	{ sprintf_iwrite (v, "lld"); }
void ostringstream::iwrite (unsigned long long v) { sprintf_iwrite (v, "llu"); }
#endif

/// Writes \p v into the stream as utf8
void ostringstream::iwrite (wchar_t v)
{
    char buffer [9];
    *utf8out(buffer) = v;
    write_buffer (buffer, Utf8Bytes(v));
}

/// Writes value \p v into the stream as text.
void ostringstream::iwrite (bool v)
{
    static const char* c_Names[2] = { "false", "true" };
    write_buffer (c_Names[v], 5 - v);
}

/// Writes string \p s into the stream.
void ostringstream::iwrite (const char* s)
{
    write_buffer (s, strlen(s));
}

/// Writes string \p v into the stream.
void ostringstream::iwrite (const string& v)
{
    write_buffer (v.begin(), v.size());
}

/// Equivalent to a sprintf on the string.
int ostringstream::format (const char* fmt, ...)
{
    simd::reset_mmx();
    va_list args;
    va_start (args, fmt);
    const bool bIsString = m_pResizable;
    assert (ipos());
    int rv = vsnprintf (ipos(), remaining() + bIsString, fmt, args);
    if (rv > 0) {
	const size_t urv (rv);
	if (urv > remaining() + bIsString)
	    rv = (urv < overflow(urv) ? 0 : vsnprintf (ipos(), remaining() + bIsString, fmt, args));
	skip (rv);
    }
    va_end (args);
    return (rv);
}

/// Sets the flag \p f in the stream.
void ostringstream::iwrite (ios::fmtflags f)
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
	default:
	    m_Flags |= f;
	    break;
    }
}

/// Links to string \p l as resizable.
void ostringstream::link (string& l)
{
    if (l.is_linked())
	l.reserve (l.capacity());
    assert (l.data() && "The output string buffer must not be read-only");
    ostream::link (l);
    m_pResizable = &l;
}

/// Unlinks the stream from its bound buffer.
void ostringstream::unlink (void)
{
    ostream::unlink();
    m_pResizable = NULL;
}

/// Writes the contents of \p buffer of \p size into the stream.
void ostringstream::write (const void* buffer, size_type sz)
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
ostringstream::size_type ostringstream::overflow (size_type n)
{
    assert (n > remaining() && "Don't call overflow if you don't need to");
    if (m_pResizable) {
	const uoff_t oldPos (pos());
	m_pResizable->reserve (oldPos + n, false);
	m_pResizable->resize (oldPos + n);
	link (*m_pResizable);
	seek (oldPos);
    }
    if (remaining() < n)
#ifdef WANT_STREAM_BOUNDS_CHECKING
	throw stream_bounds_exception ("write", "text", pos(), n, remaining());
#else
	assert (remaining() >= n && "Buffer overrun. Check your stream size calculations.");
#endif
    return (remaining());
}

} // namespace ustl


