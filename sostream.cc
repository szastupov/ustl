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

ostringstream::ostringstream (void)
: ostream (),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_Flags (0)
{
}

ostringstream::ostringstream (void* p, size_t n)
: ostream (p, n),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_Flags (0)
{
}

ostringstream::ostringstream (const memlink& source)
: ostream (source),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Base (10),
  m_Precision (2),
  m_Width (0),
  m_Flags (0)
{
}

const char c_Digits[] = "0123456789ABCDEF";

void ostringstream::write_buffer (const char* buf, size_t bufSize)
{
    size_t btw = 0, written = 0;
    while ((written += btw) < bufSize && (remaining() || overflow()))
	write (buf + written, btw = min (remaining(), bufSize - written));
}

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

ostringstream& ostringstream::operator<< (double iv)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(double) + 1;
    char buffer [c_BufSize];
    buffer [c_BufSize - 1] = 0;
    assert (m_Precision < c_BufSize / 2);
    uoff_t i = c_BufSize - 2 - (m_Precision + 1);
    assert (i < c_BufSize);
    double v = iv;
    bool negative = (v < 0);
    if (negative)
	v = -v;
    if (v < 1.0)
	buffer[i--] = c_Digits[0];
    while (v >= 1.0 && i) {
	buffer[i--] = c_Digits[u_long(v) % m_Base];
	v /= m_Base;
    }
    if (negative)
	buffer[i--] = '-';
    ++ i;
    if (m_Precision > 0) {
	v = iv;
	uoff_t di = c_BufSize - 2 - m_Precision;
	buffer [di] = m_DecimalSeparator;
	u_short precision = m_Precision;
	while (precision--) {
	    v *= m_Base;
	    buffer[++di] = c_Digits[u_long(v) % m_Base];
	}
    }
    write_buffer (buffer + i, c_BufSize - i - 1);
    return (*this);
}

ostringstream& ostringstream::operator<< (bool v)
{
    if (v)
	write_buffer ("true", 4);
    else
	write_buffer ("false", 5);
    return (*this);
}

ostringstream& ostringstream::operator<< (const char* s)
{
    write_buffer (s, strlen(s));
    return (*this);
}

ostringstream& ostringstream::operator<< (const string& v)
{
    write_buffer (v.begin(), v.length());
    return (*this);
}

/// Equivalent to a sprintf on the string.
int ostringstream::format (const char* fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    void* dest = begin() + pos();
    int rv = vsnprintf (reinterpret_cast<char*>(dest), remaining(), fmt, args);
    if (rv > 0)
	skip (rv);
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

}; // namespace ustl


