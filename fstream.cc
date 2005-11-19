// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// file.cc
//

#include "fstream.h"
#include "uexception.h"
#include "uutility.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

namespace ustl {

/// Default constructor.
fstream::fstream (void)
: ios_base (),
  m_fd (-1),
  m_Filename ()
{
}

/// Opens \p filename in \p mode.
fstream::fstream (const char* filename, openmode mode)
: ios_base (),
  m_fd (-1),
  m_Filename ()
{
    open (filename, mode);
}

/// Attaches to \p nfd of \p filename.
fstream::fstream (int nfd, const char* filename)
: ios_base (),
  m_fd (-1),
  m_Filename ()
{
    attach (nfd, filename);
}

/// Destructor. Closes if still open, but without throwing.
fstream::~fstream (void) throw()
{
    clear (goodbit);
    exceptions (goodbit);
    close();
    assert (!(rdstate() & badbit) && "close failed in the destructor! This may lead to loss of user data. Please call close() manually and either enable exceptions or check the badbit.");
}

/// Attaches to the given \p nfd.
void fstream::attach (int nfd, const char* filename)
{
    assert (filename && "Don't do that");
    clear (goodbit);
    if (nfd < 0 && set_and_throw (badbit))
	throw file_exception ("attach", filename);
    close();
    m_fd = nfd;
    m_Filename = filename;
}

/// Detaches from the current fd.
void fstream::detach (void)
{
    m_fd = -1;
    m_Filename.clear();
}

/// Converts openmode bits into libc open flags.
/*static*/ int fstream::om_to_flags (openmode m)
{
    static const int s_OMFlags [nombits] = {
	0,		// in
	O_CREAT,	// out
	O_APPEND,	// app
	O_APPEND,	// ate
	0,		// binary
	O_TRUNC,	// trunc
	O_NONBLOCK,	// nonblock
	0,		// nocreate
	O_NOCTTY	// noctty
    };
    int flags = (m - 1) & O_ACCMODE;	// in and out
    for (uoff_t i = 0; i < VectorSize(s_OMFlags); ++ i)
	if (m & (1 << i))
	    flags |= s_OMFlags[i];
    if (m & nocreate)
	flags &= ~O_CREAT;
    return (flags);
}

/// \brief Opens \p filename in the given mode.
/// \warning The string at \p filename must exist until the object is closed.
void fstream::open (const char* filename, openmode mode, mode_t perms)
{
    clear (goodbit);
    int nfd = ::open (filename, om_to_flags(mode), perms);
    if (nfd < 0 && set_and_throw (badbit))
	throw file_exception ("open", filename);
    close();
    m_fd = nfd;
    m_Filename = filename;
}

/// Closes the file and throws on error.
void fstream::close (void)
{
    if (m_fd >= 0 && ::close(m_fd) && set_and_throw (badbit | failbit))
	throw file_exception ("close", name());
    detach();
}

/// Moves the current file position to \p n.
off_t fstream::seek (off_t n, seekdir whence)
{
    off_t p = lseek (m_fd, n, whence);
    if (p < 0 && set_and_throw (failbit))
	throw file_exception ("seek", name());
    return (p);
}

/// Returns the current file position.
off_t fstream::pos (void) const
{
    return (lseek (m_fd, 0, SEEK_CUR));
}

/// Reads \p n bytes into \p p.
off_t fstream::read (void* p, off_t n)
{
    off_t btr (n);
    while (btr) {
	const off_t br (n - btr);
	ssize_t brn = ::read (m_fd, advance(p,br), btr);
	if (brn > 0)
	    btr -= brn;
	else if (!brn) {
	    if (set_and_throw (eofbit | failbit))
		throw stream_bounds_exception ("read", name(), pos() - br, n, br);
	    break;
	} else if (errno != EINTR) {
	    if (errno != EAGAIN && set_and_throw (failbit))
		throw file_exception ("read", name());
	    break;
	}
    }
    return (n - btr);
}

/// Writes \p n bytes from \p p.
off_t fstream::write (const void* p, off_t n)
{
    off_t btw (n);
    while (btw) {
	const off_t bw (n - btw);
	ssize_t bwn = ::write (m_fd, advance(p,bw), btw);
	if (bwn > 0)
	    btw -= bwn;
	else if (!bwn) {
	    if (set_and_throw (eofbit | failbit))
		throw stream_bounds_exception ("write", name(), pos() - bw, n, bw);
	    break;
	} else if (errno != EINTR) {
	    if (errno != EAGAIN && set_and_throw (failbit))
		throw file_exception ("write", name());
	    break;
	}
    }
    return (n - btw);
}

/// Returns the file size.
off_t fstream::size (void) const
{
    struct stat st;
    st.st_size = 0;
    fstat (m_fd, &st);
    return (st.st_size);
}

} // namespace ustl

