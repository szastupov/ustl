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
#include <string.h>
#include <stdio.h>	// for vsnprintf in ostringstream::format
#include <stdarg.h>

namespace ustl {

ostringstream::ostringstream (void)
: ostream (),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Precision (2)
{
}

ostringstream::ostringstream (void* p, size_t n)
: ostream (p, n),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Precision (2)
{
}

ostringstream::ostringstream (const memlink& source)
: ostream (source),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator (','),
  m_Precision (2)
{
}

const char c_Digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

ostringstream& ostringstream::operator<< (long v)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    bool negative = (v < 0);
    if (negative)
	v = -v;
    if (v == 0)
	buffer[i--] = c_Digits[0];
    while (v) {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    }
    if (negative)
	buffer[i--] = '-';
    ++ i;
    write (buffer + i, c_BufSize - i - 1);
    return (*this);
}

ostringstream& ostringstream::operator<< (u_long v)
{
    assert (m_Base < VectorSize(c_Digits));
    const size_t c_BufSize = BitsInType(v) + 1;
    char buffer [c_BufSize];
    uoff_t i = c_BufSize - 1;
    buffer [i--] = 0;
    if (v == 0)
	buffer[i--] = c_Digits[0];
    while (v) {
	buffer[i--] = c_Digits[v % m_Base];
	v /= m_Base;
    }
    ++ i;
    write (buffer + i, c_BufSize - i - 1);
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
    write (buffer + i, c_BufSize - i - 1);
    return (*this);
}

ostringstream& ostringstream::operator<< (bool v)
{
    if (v)
	write ("true", 4);
    else
	write ("false", 5);
    return (*this);
}

ostringstream& ostringstream::operator<< (const char* s)
{
    write (s, strlen(s));
    return (*this);
}

ostringstream& ostringstream::operator<< (const string& v)
{
    write (v.begin(), v.length());
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

} // namespace ustl


