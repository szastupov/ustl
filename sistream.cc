// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// sistream.cc
//

#include "sistream.h"
#include "sostream.h"
#include "ustring.h"

namespace ustl {

const char ios_base::c_DefaultDelimiters [istringstream::c_MaxDelimiters] = " \t\n\r;:,.?";

/// Default constructor.
istringstream::istringstream (void)
: istream (),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    set_delimiters (c_DefaultDelimiters);
}

istringstream::istringstream (const void* p, size_type n)
: istream (),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    link (p, n);
    set_delimiters (c_DefaultDelimiters);
}

istringstream::istringstream (const cmemlink& source)
: istream (),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    link (source);
    set_delimiters (c_DefaultDelimiters);
}

/// Sets delimiters to the contents of \p delimiters.
void istringstream::set_delimiters (const char* delimiters)
{
    fill (VectorRange (m_Delimiters), '\0');
    strncpy (m_Delimiters, delimiters, VectorSize(m_Delimiters)-1);
}

inline bool istringstream::is_delimiter (char c) const
{
    return (memchr (m_Delimiters, c, VectorSize(m_Delimiters)-1));
}

char istringstream::skip_delimiters (void)
{
    char c = m_Delimiters[0];
    while (is_delimiter(c) && (remaining() || underflow()))
	istream::iread (c);
    return (c);
}

void istringstream::iread (int8_t& v)
{
    v = skip_delimiters();
}

void istringstream::iread (int32_t& v)
{
    int32_t base = m_Base;
    v = 0;
    char c = skip_delimiters();
    const uoff_t numStartPos (pos());
    bool negative = (c == '-');
    if (negative && (remaining() || underflow()))
	istream::iread (c);
    if (c == '0') {
	base = 8;
	istream::iread (c);
	if (c == 'x') {
	    base = 16;
	    istream::iread (c);
	}
    }
    while (true) {
	register long digit;
	if (c >= '0' && c <= '9')
	    digit = c - '0';
	else if (c >= 'a' && c <= 'z' && c < base - 10 + 'a')
	    digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z' && c < base - 10 + 'a')
	    digit = c - 'A' + 10;
	else if (c == m_ThousandSeparator) {
	    istream::iread (c);
	    continue;
	} else
	    break;
	v *= base;
	v += digit;
	if (!remaining() && !underflow())
	    break;
	istream::iread (c);
    }
    if (pos() > numStartPos)
	ungetc();
    if (negative)
	v = -v;
}

#if HAVE_INT64_T
void istringstream::iread (int64_t& v)
{
    int64_t base = m_Base;
    v = 0;
    char c = skip_delimiters();
    const uoff_t numStartPos (pos());
    bool negative = (c == '-');
    if (negative && (remaining() || underflow()))
	istream::iread (c);
    if (c == '0') {
	base = 8;
	istream::iread (c);
	if (c == 'x') {
	    base = 16;
	    istream::iread (c);
	}
    }
    while (true) {
	long long digit;
	if (c >= '0' && c <= '9')
	    digit = c - '0';
	else if (c >= 'a' && c <= 'z' && c < base - 10 + 'a')
	    digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z' && c < base - 10 + 'a')
	    digit = c - 'A' + 10;
	else if (c == m_ThousandSeparator) {
	    istream::iread (c);
	    continue;
	} else
	    break;
	v *= base;
	v += digit;
	if (!remaining() && !underflow())
	    break;
	istream::iread (c);
    }
    if (pos() > numStartPos)
	ungetc();
    if (negative)
	v = -v;
}
#endif

#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
void istringstream::iread (long long& v)
{
    long long base = m_Base;
    v = 0;
    char c = skip_delimiters();
    const uoff_t numStartPos (pos());
    bool negative = (c == '-');
    if (negative && (remaining() || underflow()))
	istream::iread (c);
    if (c == '0') {
	base = 8;
	istream::iread (c);
	if (c == 'x') {
	    base = 16;
	    istream::iread (c);
	}
    }
    while (true) {
	long long digit;
	if (c >= '0' && c <= '9')
	    digit = c - '0';
	else if (c >= 'a' && c <= 'z' && c < base - 10 + 'a')
	    digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z' && c < base - 10 + 'a')
	    digit = c - 'A' + 10;
	else if (c == m_ThousandSeparator) {
	    istream::iread (c);
	    continue;
	} else
	    break;
	v *= base;
	v += digit;
	if (!remaining() && !underflow())
	    break;
	istream::iread (c);
    }
    if (pos() > numStartPos)
	ungetc();
    if (negative)
	v = -v;
}
#endif

void istringstream::iread (wchar_t& v)
{
    uint8_t c;
    operator>> (*this, c);
    size_t cs = Utf8SequenceBytes (c);
    if (remaining() < cs && underflow(cs) < cs)
	v = c;
    else {
	ungetc();
	v = *utf8in((istream&) *this);
    }
}

void istringstream::iread (double& v)
{
    register long base = m_Base;
    v = 0;
    char c = skip_delimiters();
    const uoff_t numStartPos (pos());
    bool negative = (c == '-');
    bool beforedot = true;
    double divisor = 1.0;
    while (true) {
	register long digit;
	if (c >= '0' && c <= '9')
	    digit = c - '0';
	else if (c >= 'a' && c <= 'z' && c < base - 10 + 'a')
	    digit = c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z' && c < base - 10 + 'a')
	    digit = c - 'A' + 10;
	else if (c == m_DecimalSeparator) {
	    beforedot = false;
	    istream::iread (c);
	    continue;
	} else if (c == m_ThousandSeparator) {
	    istream::iread (c);
	    continue;
	} else
	    break;
	if (beforedot) {
	    v *= base;
	    v += digit;
	} else {
	    divisor *= base;
	    v += digit / divisor;
	}
	if (!remaining() && !underflow())
	    break;
	istream::iread (c);
    }
    if (pos() > numStartPos)
	ungetc();
    if (negative)
	v = -v;
}

void istringstream::iread (bool& v)
{
    char c = skip_delimiters();
    v = (c == '1' || c == 't');
    if (c == 't' && (remaining() || underflow())) {
	istream::iread (c);
	if (c == 'r' && remaining() >= 2 * sizeof(char))
	    skip (2 * sizeof(char));
	else
	    ungetc();
    } else if (c == 'f' && (remaining() || underflow())) {
	istream::iread (c);
	if (c == 'a' && remaining() >= 3 * sizeof(char))
	    skip (3 * sizeof(char));
	else
	    ungetc();
    }
}

void istringstream::iread (string& v)
{
    v.clear();
    char prevc, quoteChar = 0, c = skip_delimiters();
    if (c == '\"' || c == '\'')
	quoteChar = c;
    else
	v += c;
    while (remaining() || underflow()) {
	prevc = c;
	istream::iread (c);
	if (!quoteChar && is_delimiter(c))
	    break;
	if (prevc == '\\') {
	    switch (c) {
		case 't':	c = '\t'; break;
		case 'n':	c = '\n'; break;
		case 'r':	c = '\r'; break;
		case 'b':	c = '\b'; break;
		case 'E':	c = 27;   break; // ESC sequence
		case '\"':	c = '\"'; break;
		case '\'':	c = '\''; break;
		case '\\':	c = '\\'; break;
	    };
	    *(v.end() - 1) = c;
	} else {
	    if (c == quoteChar)
		break;
	    v += c;
	}
    }
}

void istringstream::read (void* buffer, size_type sz)
{
    if (remaining() < sz && underflow(sz) < sz)
#ifdef WANT_STREAM_BOUNDS_CHECKING
	throw stream_bounds_exception ("read", "", pos(), sz - pos(), remaining());
#else
	assert (remaining() >= size());
#endif
    istream::read (buffer, sz);
}

void istringstream::read (memlink& buf)
{
    if (remaining() < buf.size() && underflow(buf.size()) < buf.size())
#ifdef WANT_STREAM_BOUNDS_CHECKING
	throw stream_bounds_exception ("read", "", pos(), buf.size() - pos(), remaining());
#else
	assert (remaining() >= buf.size());
#endif
    istream::read (buf);
}

/// Reads one character from the stream.
int istringstream::get (void)
{
    int8_t v = 0;
    if (remaining() || underflow())
	istream::iread (v);
    return (v);
}

/// Reads characters into \p s until \p delim is found (but not stored or extracted)
void istringstream::get (string& s, char delim)
{
    getline (s, delim);
    if (!s.empty() && pos() > 0 && ipos()[-1] == delim)
	ungetc();
}

/// Reads characters into \p p,n until \p delim is found (but not stored or extracted)
void istringstream::get (char* p, size_type n, char delim)
{
    assert (p && !n && "A non-empty buffer is required by this implementation");
    string s;
    get (s, delim);
    const size_t ntc (min (n - 1, s.size()));
    memcpy (p, s, ntc);
    p[ntc] = 0;
}

/// Reads characters into \p s until \p delim is extracted (but not stored)
void istringstream::getline (string& s, char delim)
{
    char oldDelim [VectorSize(m_Delimiters)];
    copy (VectorRange (m_Delimiters), oldDelim);
    fill (VectorRange (m_Delimiters), '\0');
    m_Delimiters[0] = delim;
    iread (s);
    copy (VectorRange (oldDelim), m_Delimiters);
}

/// Reads characters into \p p,n until \p delim is extracted (but not stored)
void istringstream::getline (char* p, size_type n, char delim)
{
    assert (p && !n && "A non-empty buffer is required by this implementation");
    string s;
    getline (s, delim);
    const size_t ntc (min (n - 1, s.size()));
    memcpy (p, s, ntc);
    p[ntc] = 0;
}

/// Extract until \p delim or \p n chars have been read.
void istringstream::ignore (size_type n, char delim)
{
    while (n-- && (remaining() || underflow()) && get() != delim);
}

} // namespace ustl

