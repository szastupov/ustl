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
  m_Fd (fd),
  m_bEOF (false)
{
    reserve (255);
}

/// Default destructor.
fdostringstream::~fdostringstream (void)
{
    try { flush(); } catch (...) {}
}

/// Flushes the buffer to the file.
void fdostringstream::flush (void)
{
    while (pos() && overflow (remaining()));
}

/// Called when more buffer space (\p n bytes) is needed.
fdostringstream::size_type fdostringstream::overflow (size_type n)
{
    if (m_bEOF)
	return (ostringstream::overflow (n));
    size_type bw = 0;
    while (!bw) {
	errno = 0;
	ssize_t bwn = ::write (m_Fd, cdata() + bw, pos() - bw);
	if (bwn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("write");
	} else if (bwn == 0) {
	    if (pos() > bw)
		m_bEOF = true;
	    break;
	} else
	    bw += bwn;
    }
    erase (begin(), bw);
    if (remaining() < n)
	ostringstream::overflow (n);
    return (remaining());
}

//----------------------------------------------------------------------

/// Constructs a stream to read from \p fd.
fdistringstream::fdistringstream (int fd)
: istringstream (),
  m_Buffer (255),
  m_Fd (fd),
  m_bEOF (false)
{
    link (m_Buffer.data(), 0U);
}

fdistringstream::size_type fdistringstream::underflow (size_type n)
{
    if (m_bEOF)
	return (istringstream::underflow (n));

    const ssize_t freeSpace = m_Buffer.size() - pos();
    const ssize_t neededFreeSpace = max (n, m_Buffer.size() / 2);
    const size_t oughtToErase = Align (max (0, neededFreeSpace - freeSpace));
    const size_t nToErase = min (pos(), oughtToErase);
    m_Buffer.memlink::erase (m_Buffer.begin(), nToErase);
    const uoff_t oldPos (pos() - nToErase);

    size_type br = oldPos;
    if (m_Buffer.size() - br < n) {
	m_Buffer.resize (br + neededFreeSpace);
	link (m_Buffer.data(), 0U);
    }
    while (br - oldPos < n) {
	errno = 0;
	ssize_t brn = ::read (m_Fd, m_Buffer.begin() + br, m_Buffer.size() - br);
	if (brn < 0) {
	    if (errno != EAGAIN && errno != EINTR)
		throw libc_exception ("read");
	} else if (brn == 0) {
	    m_bEOF = true;
	    break;
	} else
	    br += brn;
    }
    m_Buffer[br] = string::c_Terminator;
    link (m_Buffer.data(), br);
    seek (oldPos);
    return (remaining());
}

//----------------------------------------------------------------------

} // namespace ustl
#endif // WITHOUT_CIN_COUT_CERR

