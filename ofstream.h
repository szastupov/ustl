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
// fdostringstream.h
//

#ifndef FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC
#define FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC

#include "sostream.h"
#include "memblock.h"

namespace ustl {

#define USTL_COUT_BUFFER_SIZE		1024
#define USTL_COUT_FLUSH_THRESHOLD	(USTL_COUT_BUFFER_SIZE / 8)

class string;

/// A string stream that writes to an fd. Implements cout and cerr.
class fdostringstream : public ostringstream {
public:
    				fdostringstream (int fd, size_t bufSize = USTL_COUT_BUFFER_SIZE);
			       ~fdostringstream (void);
    void			flush (void);
    fdostringstream&		operator<< (char c);
    inline fdostringstream&	operator<< (char* s);
    fdostringstream&		operator<< (const char* s);
    fdostringstream&		operator<< (const string& s);
    inline fdostringstream&	operator<< (short v);
    inline fdostringstream&	operator<< (int v);
    inline fdostringstream&	operator<< (long v);
    inline fdostringstream&	operator<< (u_char v);
    inline fdostringstream&	operator<< (u_short v);
    inline fdostringstream&	operator<< (u_int v);
    inline fdostringstream&	operator<< (u_long v);
    inline fdostringstream&	operator<< (float v);
    inline fdostringstream&	operator<< (double v);
    inline fdostringstream&	operator<< (bool v);
    inline fdostringstream&	operator<< (wchar_t v);
    inline fdostringstream&	operator<< (const u_char* s);
    inline fdostringstream&	operator<< (u_char* s);
    inline fdostringstream&	operator<< (const void* s);
    inline fdostringstream&	operator<< (void* s);
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
private:
    inline void			flush_if_full (void);
private:
    memblock			m_Buffer;
    int				m_Fd;
};

inline fdostringstream& fdostringstream::operator<< (char* s)
{
    return (operator<< (const_cast<const char*>(s)));
}

inline fdostringstream& fdostringstream::operator<< (const u_char* s)
{
    return (operator<< (reinterpret_cast<const char*>(s)));
}

inline fdostringstream& fdostringstream::operator<< (u_char* s)
{
    return (operator<< (const_cast<const u_char*>(s)));
}

inline fdostringstream& fdostringstream::operator<< (short v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (int v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (long v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (u_char v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (u_short v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (u_int v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (u_long v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (float v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (double v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (bool v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (wchar_t v)	{ ostringstream::operator<< (v); return (*this); }
inline fdostringstream& fdostringstream::operator<< (void* s)	{ ostringstream::operator<< (s); return (*this); }
inline fdostringstream& fdostringstream::operator<< (const void* s)	{ ostringstream::operator<< (s); return (*this); }

extern fdostringstream cout, cerr;

} // namespace ustl

#endif

