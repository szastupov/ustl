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
// sistream.cc
//

#include "sistream.h"
#include "sostream.h"
#include "ustring.h"
#include <ctype.h>
#include "sostream.h"
#include "uios.h"

namespace ustl {

namespace ios {
    const char* c_DefaultDelimiters = " \t\n\r;:,.?";
}

istringstream::istringstream (void)
: istream (),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    set_delimiters (ios::c_DefaultDelimiters);
}

istringstream::istringstream (const void* p, size_t n)
: istream (p, n),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    set_delimiters (ios::c_DefaultDelimiters);
}

istringstream::istringstream (const cmemlink& source)
: istream (source),
  m_Base (10),
  m_DecimalSeparator ('.'),
  m_ThousandSeparator ('\0')
{
    set_delimiters (ios::c_DefaultDelimiters);
}

void istringstream::set_delimiters (const char* delimiters)
{
    strncpy (m_Delimiters, delimiters, c_MaxDelimiters);
}

inline bool istringstream::is_delimiter (char c) const
{
    return (NULL != memchr (m_Delimiters, c, c_MaxDelimiters));
}

istringstream& istringstream::operator>> (u_char& v)
{
    char c = m_Delimiters[0];
    while (remaining() && is_delimiter(c))
	iread (c);
    v = c;
    return (*this);
}

istringstream& istringstream::operator>> (long& v)
{
    char c = m_Delimiters[0];
    register long base = m_Base;
    v = 0;
    while (remaining() && is_delimiter(c))
	iread (c);
    bool negative = (c == '-');
    if (negative && remaining())
	iread (c);
    if (c == '0') {
	base = 8;
	iread (c);
	if (c == 'x') {
	    base = 16;
	    iread (c);
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
	    iread (c);
	    continue;
	} else
	    break;
	v *= base;
	v += digit;
	if (!remaining())
	    break;
	iread (c);
    }
    seek (pos() - sizeof(char));
    if (negative)
	v = -v;
    return (*this);
}

istringstream& istringstream::operator>> (double& v)
{
    char c = m_Delimiters[0];
    register long base = m_Base;
    v = 0;
    while (remaining() && is_delimiter(c))
	iread (c);
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
	    iread (c);
	    continue;
	} else if (c == m_ThousandSeparator) {
	    iread (c);
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
	if (!remaining())
	    break;
	iread (c);
    }
    seek (pos() - sizeof(char));
    if (negative)
	v = -v;
    return (*this);
}

istringstream& istringstream::operator>> (bool& v)
{
    char c = m_Delimiters[0];
    while (remaining() && is_delimiter(c))
	iread (c);
    v = (c == '1' || c == 't');
    if (c == 't' && remaining()) {
	iread (c);
	if (c == 'r' && remaining() >= 2 * sizeof(char))
	    skip (2 * sizeof(char));
	else
	    seek (pos() - sizeof(char));
    } else if (c == 'f' && remaining()) {
	iread (c);
	if (c == 'a' && remaining() >= 3 * sizeof(char))
	    skip (3 * sizeof(char));
	else
	    seek (pos() - sizeof(char));
    }
    return (*this);
}

istringstream& istringstream::operator>> (string& v)
{
    char c = m_Delimiters[0];
    while (remaining() && is_delimiter(c))
	iread (c);
    if (c == '\"') {
	char prevc;
	do {
	    if (!remaining())
		break;
	    prevc = c;
	    iread (c);
	    v += c;
	} while (c != '\"' || prevc != '\\');
    } else {
	do {
	    v += c;
	    if (!remaining())
		break;
	    iread (c);
	} while (!is_delimiter(c));
    }
    return (*this);
}

}; // namespace ustl

