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
// sistream.h
//

#ifndef SISTREAM_H
#define SISTREAM_H

#include "mistream.h"

namespace ustl {

class string;
/// A stream that reads textual data from a memory block.
class istringstream : public istream {
public:
    static const size_t c_MaxDelimiters = 16;	///< Maximum number of word delimiters.
public:
    				istringstream (void);
				istringstream (const void* p, size_t n);
    explicit			istringstream (const cmemlink& source);
    inline istringstream&	operator>> (signed char& v);
    inline istringstream&	operator>> (char& v);
    inline istringstream&	operator>> (short& v);
    inline istringstream&	operator>> (int& v);
    istringstream&		operator>> (long& v);
    istringstream&		operator>> (u_char& v);
    inline istringstream&	operator>> (u_short& v);
    inline istringstream&	operator>> (u_int& v);
    inline istringstream&	operator>> (u_long& v);
    inline istringstream&	operator>> (wchar_t& v);
    inline istringstream&	operator>> (float& v);
    istringstream&		operator>> (double& v);
    istringstream&		operator>> (bool& v);
    istringstream&		operator>> (string& v);
    void			set_delimiters (const char* delimiters);
    inline void			set_base (short base);
    inline void			set_decimal_separator (char s);
    inline void			set_thousand_separator (char s);
private:
    inline bool			is_delimiter (char c) const;
private:
    char			m_Delimiters [c_MaxDelimiters];
    u_short			m_Base;
    char			m_DecimalSeparator;
    char			m_ThousandSeparator;
};

/// Sets the numeric base used to read numbers.
inline void istringstream::set_base (short base)
{
    m_Base = base;
}

/// Sets the decimal separator to recognize in numbers. Period by default.
inline void istringstream::set_decimal_separator (char s)
{
    m_DecimalSeparator = s;
}

/// Sets the thousand separator to recognize in numbers. Comma by default.
inline void istringstream::set_thousand_separator (char s)
{
    m_ThousandSeparator = s;
}

/// Reads a single character into \p v.
inline istringstream& istringstream::operator>> (signed char& v)
{
    u_char vl; operator>> (vl); v = vl; return (*this);
}

/// Reads a single character into \p v.
inline istringstream& istringstream::operator>> (char& v)
{
    u_char vl; operator>> (vl); v = vl; return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (short& v)
{
    long vl; operator>> (vl); v = static_cast<short>(vl); return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (int& v)
{
    long vl; operator>> (vl); v = static_cast<int>(vl); return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (u_short& v)
{
    short vl; operator>> (vl); v = static_cast<u_short>(vl); return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (u_int& v)
{
    int vl; operator>> (vl); v = static_cast<u_int>(vl); return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (u_long& v)
{
    long vl; operator>> (vl); v = vl; return (*this);
}

/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (wchar_t& v)
{
    long vl; operator>> (vl); v = static_cast<wchar_t>(vl); return (*this);
}

/// Reads a floating point number into \p v.
inline istringstream& istringstream::operator>> (float& v)
{
    double vl; operator>> (vl); v = static_cast<float>(vl); return (*this);
}

}; // namespace ustl

#endif

