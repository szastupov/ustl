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
// varsize.h
//

#ifndef VARSIZE_H_5F2DD7855F1CD23A031BCCF643F57965
#define VARSIZE_H_5F2DD7855F1CD23A031BCCF643F57965

#include "utypes.h"
#include "mistream.h"
#include "mostream.h"

namespace ustl {

class ostream;

///
/// A size_t wrapper for writing to binary streams in minimum-byte form.
///
/// The minimum-byte form is defined as follows:
/// \li values in [0..UCHAR_MAX] are written using 1 byte
/// \li [UCHAR_MAX..USHRT_MAX] are written using 4 bytes
/// \li [USHRT_MAX..ULONG_MAX] are written using 8 bytes
///
/// The above numbers are for u_long aligned writes only. If your writes are
/// not, the worst case scenarios will write 5 bytes for shorts and up to 10
/// bytes for longs. varsize exists because most collections have less than
/// 256 elements; this is especially true of strings.
///
class varsize {
public:
    			varsize (void);
			varsize (size_t s);
    inline		operator size_t (void) const;
    inline varsize&	operator= (size_t v);
    inline varsize&	operator+= (size_t v);
    inline varsize&	operator-= (size_t v);
    size_t		stream_size (void) const;
    void		read (istream& is);
    void		write (ostream& os) const;
private:
    size_t		m_Size;
};

/// Returns the size_t value of the object
inline varsize::operator size_t (void) const
{
    return (m_Size);
}

inline varsize& varsize::operator= (size_t v)
{
    m_Size = v;
    return (*this);
}

inline varsize& varsize::operator+= (size_t v)
{
    m_Size += v;
    return (*this);
}

inline varsize& varsize::operator-= (size_t v)
{
    m_Size -= v;
    return (*this);
}

} // namespace ustl

STD_STREAMABLE(ustl::varsize)

#endif

