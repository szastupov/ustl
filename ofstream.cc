// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
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

#ifndef WITHOUT_CIN_COUT_CERR
namespace ustl {

//----------------------------------------------------------------------

fdistringstream cin  (STDIN_FILENO);
fdostringstream cout (STDOUT_FILENO);
fdostringstream cerr (STDERR_FILENO);

//----------------------------------------------------------------------

/// Constructs a stream for writing to \p fd.
fdostringstream::fdostringstream (int fd)
: ostringstream (),
  m_Buffer (256),
  m_Fd (fd)
{
    link (m_Buffer);
}

/// Default destructor.
fdostringstream::~fdostringstream (void)
{
    try { flush(); } catch (...) {}
}

/// Flushes the buffer to the file.
void fdostringstream::flush (void)
{
    while (pos() && overflow());
}

/// Called when more buffer space (\p n bytes) is needed.
fdostringstream::size_type fdostringstream::overflow (size_type n)
{
    size_type bw = 0;
    while (!bw) {
	errno = 0;
	ssize_t bwn = ::write (m_Fd, cdata() + bw, pos() - bw);
	if (bwn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("write");
	} else if (bwn == 0)
	    break;
	else
	    bw += bwn;
    }
    erase (begin(), bw);
    if (remaining() < n)
	ostringstream::overflow (n);
    return (remaining());
}

/// Equivalent to a sprintf on the string.
int fdostringstream::format (const char* fmt, ...)
{
    if (remaining() < strlen(fmt) * 5)
	flush();
    va_list args;
    va_start (args, fmt);
    char* dest = reinterpret_cast<char*>(ipos());
    int rv = vsnprintf (dest, remaining(), fmt, args);
    if (rv > 0)
	skip (rv);
    va_end (args);
    return (rv);
}

//----------------------------------------------------------------------

/// Constructs a stream to read from \p fd.
fdistringstream::fdistringstream (int fd)
: istringstream (),
  m_Buffer (256),
  m_Fd (fd)
{
    link (m_Buffer.data(), 0U);
}

fdistringstream::size_type fdistringstream::underflow (size_type n)
{
    m_Buffer.erase (m_Buffer.begin(), pos());
    size_type br = 0;
    if (m_Buffer.size() - pos() < n) {
	m_Buffer.resize (pos() + n);
	link (m_Buffer.data(), 0U);
    }
    while (br < n) {
	errno = 0;
	ssize_t brn = ::read (m_Fd, m_Buffer.begin() + br, m_Buffer.size() - br);
	if (brn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("read");
	} else if (brn == 0)
	    break;
	else
	    br += brn;
    }
    m_Buffer[br] = string::c_Terminator;
    link (m_Buffer.data(), br);
    return (remaining());
}

//----------------------------------------------------------------------

} // namespace ustl
#endif // WITHOUT_CIN_COUT_CERR

