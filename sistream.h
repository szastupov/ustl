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

#ifndef SISTREAM_H_0CCA102229A49F5D65EE852E62B27CE2
#define SISTREAM_H_0CCA102229A49F5D65EE852E62B27CE2

#include "mistream.h"
#ifdef WITHOUT_LIBSTDCPP
    #include "ufacets.h"
#else
    #include <locale>
#endif

namespace ustl {

class string;

/// \class istringstream sistream.h ustl.h
/// \ingroup TextStreams
///
/// \brief A stream that reads textual data from a memory block.
///
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
#ifdef __GNUC__
    istringstream&		operator>> (long long& v);
    inline istringstream&	operator>> (unsigned long long& v);
#endif
    void			set_delimiters (const char* delimiters);
    inline void			set_base (short base);
    inline void			set_decimal_separator (char s);
    inline void			set_thousand_separator (char s);
    void			read (void* buffer, size_t size);
    void			read (memlink& buf);
    inline void			read_strz (string& str);
protected:
    inline virtual size_t	underflow (size_t = 1)	{ return (0); }
    inline void			ungetc (void)		{ seek (pos() - sizeof(char)); }
    char			skip_delimiters (void);
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

#ifdef __GNUC__
/// Reads a number into \p v.
inline istringstream& istringstream::operator>> (unsigned long long& v)
{
    long long vl; operator>> (vl); v = vl; return (*this);
}
#endif

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

/// Reads a null-terminated character stream. This is not allowed in this class.
inline void istringstream::read_strz (string& str)
{
    assert (false && "Reading nul characters is not allowed from text streams");
}

}; // namespace ustl

#endif

