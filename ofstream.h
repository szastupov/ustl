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

#ifndef WITHOUT_CIN_COUT_CERR
namespace ustl {

class string;

/// \class fdostringstream fdostream.h ustl.h
/// \ingroup DeviceStreams
/// \brief A string stream that writes to an fd. Implements cout and cerr.
class fdostringstream : public ostringstream {
public:
    explicit			fdostringstream (int fd);
    virtual		       ~fdostringstream (void);
    void			flush (void);
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
protected:
    virtual size_t		overflow (size_t n = 1);
private:
    memblock			m_Buffer;
    int				m_Fd;
};

/// \class fdistringstream fdostream.h ustl.h
/// \ingroup DeviceStreams
/// \brief A string stream that reads from an fd. Implements cin.
///
/// I would discourage the use of cin in general. As a quick-n-dirty
/// hack to read a configuration file it is ok, but if you want to
/// do real user input, it becomes much less acceptable. The problem
/// is not really the implementation, but the way it is used, expecting
/// the completely unverified information coming from the user to be
/// magically converted to numbers, strings, etc. Bounds checking and
/// filter adaptors like utf8 are damn hard to do directly over cin.
/// Furthermore, you are sitting there, blocking on user input, when
/// you could be doing something useful. Instead, implement a nonblocking
/// block reading routine (I cannot provide you with one, since it
/// depends on your event framework), which reads all available data
/// and sends it through a series of validation filters. UTF8 format
/// checking should be done this way for maximum simplicity. Then later
/// you can use istringstream directly on the buffer, which allows you
/// to restart reading in case of errors, for instance, whereas cin
/// will simply drop the offending characters (this implementation will
/// keep some, but you are not supposed to know that). Anyway, this
/// class is here only because it is really very very small and does
/// next to nothing.
///
class fdistringstream : public istringstream {
public:
    explicit			fdistringstream (int fd);
    virtual		       ~fdistringstream (void);
protected:
    virtual size_t		underflow (size_t n = 1);
private:
    memblock			m_Buffer;
    int				m_Fd;
};

extern fdostringstream cout, cerr;
extern fdistringstream cin;

}; // namespace ustl

#endif
#endif

