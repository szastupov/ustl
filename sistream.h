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
    void			iread (long& v);
    void			iread (u_char& v);
    void			iread (double& v);
    void			iread (bool& v);
    void			iread (string& v);
#ifdef __GNUC__
    void			iread (long long& v);
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

/// Reads a null-terminated character stream. This is not allowed in this class.
inline void istringstream::read_strz (string& str)
{
    assert (false && "Reading nul characters is not allowed from text streams");
}

/// Reads one type as another.
template <typename RealT, typename CastT>
void _cast_read (istringstream& is, RealT& v)
{
    CastT cv;
    is.iread (cv);
    v = RealT (cv);
}

inline istringstream& operator>> (istringstream& is, long& v)	{ is.iread (v); return (is); }
inline istringstream& operator>> (istringstream& is, u_char& v)	{ is.iread (v); return (is); }
inline istringstream& operator>> (istringstream& is, double& v)	{ is.iread (v); return (is); }
inline istringstream& operator>> (istringstream& is, bool& v)	{ is.iread (v); return (is); }
inline istringstream& operator>> (istringstream& is, string& v)	{ is.iread (v); return (is); }
#ifdef __GNUC__
inline istringstream& operator>> (istringstream& is, long long& v) { is.iread (v); return (is); }
#endif

#define ISTRSTREAM_CAST_OPERATOR(RealT, CastT)			\
inline istringstream& operator>> (istringstream& is, RealT& v)	\
{ _cast_read<RealT,CastT>(is, v); return (is); }

ISTRSTREAM_CAST_OPERATOR (signed char,	u_char)
ISTRSTREAM_CAST_OPERATOR (char,		u_char)
ISTRSTREAM_CAST_OPERATOR (short,	long)
ISTRSTREAM_CAST_OPERATOR (int,		long)
ISTRSTREAM_CAST_OPERATOR (u_short,	long)
ISTRSTREAM_CAST_OPERATOR (u_int,	long)
ISTRSTREAM_CAST_OPERATOR (u_long,	long)
ISTRSTREAM_CAST_OPERATOR (wchar_t,	long)
ISTRSTREAM_CAST_OPERATOR (float,	double)
#ifdef __GNUC__
ISTRSTREAM_CAST_OPERATOR (unsigned long long, long long)
#endif
#undef ISTRSTREAM_CAST_OPERATOR

}; // namespace ustl

#endif

