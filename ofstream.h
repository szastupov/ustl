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
/** \class ustl::fdostringstream
 *  \brief A string stream that writes to an fd. Implements cout and cerr.
 */

#ifndef FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC
#define FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC

#include "sostream.h"
#include "memblock.h"

namespace ustl {

#define USTL_COUT_BUFFER_SIZE		1024

class string;
class fdostringstream : public ostringstream {
public:
    				fdostringstream (int fd, size_t bufSize = USTL_COUT_BUFFER_SIZE);
			       ~fdostringstream (void);
    void			flush (void);
    fdostringstream&		operator<< (char c);
    inline fdostringstream&	operator<< (char* s);
    fdostringstream&		operator<< (const char* s);
    fdostringstream&		operator<< (const string& s);
    inline ostringstream&	operator<< (short v);
    inline ostringstream&	operator<< (int v);
    inline ostringstream&	operator<< (long v);
    inline ostringstream&	operator<< (u_char v);
    inline ostringstream&	operator<< (u_short v);
    inline ostringstream&	operator<< (u_int v);
    inline ostringstream&	operator<< (u_long v);
    inline ostringstream&	operator<< (float v);
    inline ostringstream&	operator<< (double v);
    inline ostringstream&	operator<< (bool v);
    inline ostringstream&	operator<< (const u_char* s);
    inline ostringstream&	operator<< (u_char* s);
    inline ostringstream&	operator<< (const void* s);
    inline ostringstream&	operator<< (void* s);
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

inline ostringstream& fdostringstream::operator<< (const u_char* s)
{
    return (operator<< (reinterpret_cast<const char*>(s)));
}

inline ostringstream& fdostringstream::operator<< (u_char* s)
{
    return (operator<< (const_cast<const u_char*>(s)));
}

inline ostringstream& fdostringstream::operator<< (short v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (int v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (long v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (u_char v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (u_short v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (u_int v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (u_long v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (float v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (double v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (bool v)	{ ostringstream::operator<< (v); return (*this); }
inline ostringstream& fdostringstream::operator<< (void* s)	{ ostringstream::operator<< (s); return (*this); }
inline ostringstream& fdostringstream::operator<< (const void* s)	{ ostringstream::operator<< (s); return (*this); }

extern fdostringstream cout, cerr;

} // namespace ustl

#endif

