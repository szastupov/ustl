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
// fdostringstream.cc
//

#include "fdostream.h"
#include "ustring.h"
#include "uexception.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

namespace ustl {

fdostringstream cout (STDOUT_FILENO);
fdostringstream cerr (STDERR_FILENO);

fdostringstream::fdostringstream (int fd, size_t bufSize)
: ostringstream (m_Buffer),
  m_Buffer (bufSize),
  m_Fd (fd)
{
    link (m_Buffer);
}

fdostringstream::~fdostringstream (void)
{
    flush();
}

void fdostringstream::flush (void)
{
    ssize_t bw = 0;
    while (static_cast<uoff_t>(bw) < pos()) {
	errno = 0;
	bw = ::write (m_Fd, cdata(), pos());
	if (bw < 0 && errno != EINTR)
	    throw libc_exception ("write");
    }
    seek (0U);
}

inline void fdostringstream::flush_if_full (void)
{
    if (remaining() < USTL_COUT_FLUSH_THRESHOLD)
	flush();
}

fdostringstream& fdostringstream::operator<< (char c)
{
    flush_if_full();
    ostringstream::operator<< (c);
    return (*this);
}

fdostringstream& fdostringstream::operator<< (const char* s)
{
    if (remaining() < USTL_COUT_FLUSH_THRESHOLD + strlen(s))
	flush();
    ostringstream::operator<< (s);
    flush_if_full();
    return (*this);
}

fdostringstream& fdostringstream::operator<< (const string& s)
{
    if (remaining() < USTL_COUT_FLUSH_THRESHOLD + s.length())
	flush();
    ostringstream::operator<< (s);
    flush_if_full();
    return (*this);
}

/// Equivalent to a sprintf on the string.
int fdostringstream::format (const char* fmt, ...)
{
    if (remaining() < USTL_COUT_FLUSH_THRESHOLD + strlen(fmt))
	flush();
    va_list args;
    va_start (args, fmt);
    void* dest = begin() + pos();
    int rv = vsnprintf (reinterpret_cast<char*>(dest), remaining(), fmt, args);
    if (rv > 0)
	skip (rv);
    va_end (args);
    flush_if_full();
    return (rv);
}

}; // namespace ustl

