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

#ifndef SOSTREAM_H_5323DC8C26E181D43278F2F53FDCF19F
#define SOSTREAM_H_5323DC8C26E181D43278F2F53FDCF19F

#include "mostream.h"
#ifdef WITHOUT_LIBSTDCPP
    #include "ufacets.h"
#endif
#include "uios.h"

namespace ustl {

class string;

/// \class ostringstream sostream.h ustl.h
/// \ingroup TextStreams
///
/// \brief This stream writes textual data into a memory block.
///
class ostringstream : public ostream {
public:
				ostringstream (void);
				ostringstream (void* p, size_type n);
    explicit			ostringstream (string& dest);
    explicit			ostringstream (memlink& dest);
    void			iwrite (uint8_t v);
    void			iwrite (int32_t v);
    void			iwrite (uint32_t v);
    void			iwrite (wchar_t v);
    void			iwrite (float v);
    void			iwrite (double v);
    void			iwrite (bool v);
    void			iwrite (const char* s);
    void			iwrite (const string& v);
    void			iwrite (ios::fmtflags f);
#if HAVE_INT64_T
    void			iwrite (int64_t v);
    void			iwrite (uint64_t v);
#endif
#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
    void			iwrite (long long v);
    void			iwrite (unsigned long long v);
#endif
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
    inline void			set_base (uint16_t b);
    inline void			set_width (uint16_t w);
    inline void			set_decimal_separator (char s);
    inline void			set_thousand_separator (char s);
    inline void			set_precision (uint16_t precision);
    inline void			link (void* p, size_type n)	{ ostream::link (p, n); }
    inline void			link (memlink& l)		{ ostream::link (l); }
    void			link (string& l);
    virtual void		unlink (void);
    void			write (const void* buffer, size_type size);
    void			write (const cmemlink& buf);
    inline void			write_strz (const char* str);
protected:
    virtual size_type		overflow (size_type n = 1);
    void			write_buffer (const char* buf, size_type bufSize);
private:
    template <typename T>
    inline void			iwrite_integer (T v);
    template <typename T>
    inline void			iwrite_uinteger (T v);
private:
    string*			m_pResizable;		///< Pointer to the buffer, if resizable.
    uint32_t			m_Flags;		///< See ios::fmtflags.
    uint16_t			m_Base;			///< Numeric base for writing numbers.
    uint16_t			m_Precision;		///< Number of digits after the decimal separator.
    uint16_t			m_Width;		///< Field width.
    char			m_DecimalSeparator;	///< Period by default.
    char			m_ThousandSeparator;	///< Comma by default.
};

/// Sets the numeric base for writing numbers.
inline void ostringstream::set_base (uint16_t b)
{
    m_Base = b;
}

/// Sets the numeric base for writing numbers.
inline void ostringstream::set_width (uint16_t w)
{
    m_Width = w;
}

/// Sets the decimal separator in numbers. Period by default.
inline void ostringstream::set_decimal_separator (char s)
{
    m_DecimalSeparator = s;
}

/// Sets the thousand separator in numbers. Comma by default.
inline void ostringstream::set_thousand_separator (char s)
{
    m_ThousandSeparator = s;
}

/// Sets the number of digits after the decimal separator.
inline void ostringstream::set_precision (uint16_t precision)
{
    m_Precision = precision;
}

/// Disallows writing nul characters into the stream.
inline void ostringstream::write_strz (const char*)
{
    assert (false && "Writing nul characters into a text stream is not allowed");
}

#define OSTRSTREAM_OPERATOR(RealT, CastT)			\
inline ostringstream& operator<< (ostringstream& os, RealT v)	\
{ os.iwrite ((CastT) v); return (os); }

#if SIZE_OF_POINTER == SIZE_OF_INT
template <typename T>
OSTRSTREAM_OPERATOR (T*,		uint32_t)
OSTRSTREAM_OPERATOR (const void*,	uint32_t)
OSTRSTREAM_OPERATOR (void*,		uint32_t)
#else
template <typename T>
OSTRSTREAM_OPERATOR (T*,		uint64_t)
OSTRSTREAM_OPERATOR (const void*,	uint64_t)
OSTRSTREAM_OPERATOR (void*,		uint64_t)
#endif
OSTRSTREAM_OPERATOR (const char*,	const char*)
OSTRSTREAM_OPERATOR (char*,		const char*)
OSTRSTREAM_OPERATOR (uint8_t*,		const char*)
OSTRSTREAM_OPERATOR (const uint8_t*,	const char*)
OSTRSTREAM_OPERATOR (const string&,	const string&)
OSTRSTREAM_OPERATOR (ios::fmtflags,	ios::fmtflags)
OSTRSTREAM_OPERATOR (int8_t,		uint8_t)
OSTRSTREAM_OPERATOR (uint8_t,		uint8_t)
OSTRSTREAM_OPERATOR (int16_t,		int32_t)
OSTRSTREAM_OPERATOR (uint16_t,		uint32_t)
OSTRSTREAM_OPERATOR (int32_t,		int32_t)
OSTRSTREAM_OPERATOR (uint32_t,		uint32_t)
OSTRSTREAM_OPERATOR (float,		float)
OSTRSTREAM_OPERATOR (double,		double)
OSTRSTREAM_OPERATOR (bool,		bool)
OSTRSTREAM_OPERATOR (wchar_t,		wchar_t)
#if HAVE_THREE_CHAR_TYPES
OSTRSTREAM_OPERATOR (char,		uint8_t)
#endif
#if SIZE_OF_LONG == SIZE_OF_INT
OSTRSTREAM_OPERATOR (long,		int32_t)
OSTRSTREAM_OPERATOR (unsigned long,	uint32_t)
#endif
#if HAVE_INT64_T
OSTRSTREAM_OPERATOR (int64_t,		int64_t)
OSTRSTREAM_OPERATOR (uint64_t,		uint64_t)
#endif
#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
OSTRSTREAM_OPERATOR (long long,		long long)
OSTRSTREAM_OPERATOR (unsigned long long, unsigned long long)
#endif

} // namespace ustl

#endif

