// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// file.cc
//

#include "file.h"
#include "uexception.h"
#include "uutility.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

namespace ustl {

/// Default constructor.
file::file (void)
: m_Filename (""),
  m_fd (-1)
{
}

/// Destructor. Closes if still open, but without throwing.
file::~file (void) throw()
{
    try { close(); } catch (...) { assert (!"close failed"); }
}

/// Attaches to the given \p fd.
void file::attach (int fd, const char* filename)
{
    assert (filename && "Don't do that");
    m_Filename = filename;
    m_fd = fd;
}

/// \brief Opens \p filename in the given mode.
/// \warning The string at \p filename must exist until the object is closed.
void file::open (const char* filename, EOpenMode mode, mode_t perms)
{
    static const int flags[for_Last] = {
	/* for_Reading */   O_RDONLY,
	/* for_Writing */   O_WRONLY | O_CREAT | O_TRUNC,
	/* for_Appending */ O_WRONLY | O_CREAT | O_APPEND
    };
    int fd = ::open (filename, flags[mode], perms);
    if (fd < 0)
	throw file_exception ("open", filename);
    attach (fd, filename);
}

/// Closes the fd and throws on error.
void file::close (void)
{
    if (m_fd >= 0 && ::close(m_fd))
	throw file_exception ("close", m_Filename);
    attach (-1, "");
}

/// Reads \p n bytes into \p p.
void file::read (void* p, off_t n)
{
    assert (m_fd >= 0 && "open before reading");
    off_t btr (n);
    while (btr) {
	const off_t br (n - btr);
	ssize_t brn = ::read (m_fd, advance(p,br), btr);
	if (!brn)
	    throw stream_bounds_exception ("read", "ustl::file", lseek(m_fd,0,SEEK_CUR) - br, n, br);
	else if (brn > 0)
	    btr -= brn;
	else if (errno != EINTR)
	    throw file_exception ("read", m_Filename);
    }
}

/// Writes \p n bytes from \p p.
void file::write (const void* p, off_t n)
{
    assert (m_fd >= 0 && "open before writing");
    off_t btw (n);
    while (btw) {
	const off_t bw (n - btw);
	ssize_t bwn = ::write (m_fd, advance(p,bw), btw);
	if (!bwn)
	    throw stream_bounds_exception ("write", "ustl::file", lseek(m_fd,0,SEEK_CUR) - bw, n, bw);
	else if (bwn > 0)
	    btw -= bwn;
	else if (errno != EINTR)
	    throw file_exception ("read", m_Filename);
    }
}

/// Returns the current file position.
off_t file::pos (void) const
{
    off_t p = lseek (m_fd, 0, SEEK_CUR);
    if (p < 0) {
	if (errno != ESPIPE)
	    throw file_exception ("lseek", m_Filename);
	return (0);
    }
    return (p);
}

/// Returns the file size.
off_t file::size (void) const
{
    struct stat st;
    if (fstat (m_fd, &st))
	throw file_exception ("stat", m_Filename);
    return (st.st_size);
}

} // namespace ustl

