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

#include "sistream.h"
#include "sostream.h"
#include "memblock.h"

namespace ustl {

#define USTL_COUT_BUFFER_SIZE		2048
#define USTL_CIN_BUFFER_SIZE		2048

class string;

/// A string stream that writes to an fd. Implements cout and cerr.
class fdostringstream : public ostringstream {
public:
    explicit			fdostringstream (int fd, size_t bufSize = USTL_COUT_BUFFER_SIZE);
    virtual		       ~fdostringstream (void);
    void			flush (void);
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
protected:
    virtual size_t		overflow (void);
private:
    memblock			m_Buffer;
    int				m_Fd;
};

/// A string stream that reads from an fd. Implements cin.
class fdistringstream : public istringstream {
public:
    explicit			fdistringstream (int fd, size_t bufSize = USTL_CIN_BUFFER_SIZE);
    virtual		       ~fdistringstream (void);
protected:
    virtual size_t		underflow (void);
private:
    memblock			m_Buffer;
    int				m_Fd;
};

extern fdostringstream cout, cerr;
extern fdistringstream cin;

}; // namespace ustl

#endif

