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
// uexception.cc
//

#include "uexception.h"
#include "ustring.h"
#include "mistream.h"
#include "mostream.h"
#include "sostream.h"
#include "uiosfunc.h"
#include <errno.h>

namespace ustl {

//----------------------------------------------------------------------

/// Initializes an empty object
exception::exception (void)
: m_Format (xfmt_Exception)
{
}

/// Destroys the object
exception::~exception (void)
{
}

/// Returns the name of the exception
const char* exception::what (void) const
{
    return ("Generic exception");
}

/// \brief Returns a descriptive error message. fmt="%s"
/// Overloads of this functions must set NULL as the default fmt
/// argument and handle that case to provide a default format string
/// in case the user does not have a localized one. The format
/// string should be shown in the documentation to not require
/// translators to look through code. Also, this function must
/// not throw anything, so you must wrap memory allocation routines
/// (like string::format, for instance) in a try{}catch(...){} block.
///
void exception::info (string& msgbuf, const char*) const
{
    try { msgbuf.format ("%s", what()); } catch (...) { /* Ignore all exceptions */ }
}

/// Reads the exception from stream \p is.
void exception::read (istream& is)
{
    size_t stmSize;
    xfmt_t fmt;
    is >> fmt >> stmSize;
    assert (fmt == m_Format && "The saved exception is of a different type.");
    assert (stmSize - exception::stream_size() <= is.remaining() && "The saved exception data is corrupt.");
    m_Format = fmt;
}

/// Writes the exception into stream \p os as an IFF chunk.
void exception::write (ostream& os) const
{
    os << m_Format << stream_size();
}

/// Returns the size of the written exception.
size_t exception::stream_size (void) const
{
    return (stream_size_of (m_Format) + stream_size_of(size_t()));
}

/// Writes the exception as text into stream \p os.
void exception::text_write (ostringstream& os) const
{
    try {
	string buf;
	info (buf);
	os << buf;
    } catch (...) {}
}

//----------------------------------------------------------------------

/// Initializes the empty object. \p nBytes is the size of the attempted allocation.
bad_alloc::bad_alloc (size_t nBytes)
: exception(),
  m_nBytesRequested (nBytes)
{
    set_format (xfmt_BadAlloc);
}

/// Returns the name of the exception.
const char* bad_alloc::what (void) const
{
    return ("memory allocation failed");
}

/// Returns a descriptive error message. fmt="Failed to allocate %d bytes"
void bad_alloc::info (string& msgbuf, const char* fmt) const
{
    if (!fmt) fmt = "Failed to allocate %d bytes";
    try { msgbuf.format (fmt, m_nBytesRequested); } catch (...) {}
}

/// Reads the exception from stream \p is.
void bad_alloc::read (istream& is)
{
    exception::read (is);
    is >> m_nBytesRequested;
}

/// Writes the exception into stream \p os.
void bad_alloc::write (ostream& os) const
{
    exception::write (os);
    os << m_nBytesRequested;
}

/// Returns the size of the written exception.
size_t bad_alloc::stream_size (void) const
{
    return (exception::stream_size() + stream_size_of(m_nBytesRequested));
}

//----------------------------------------------------------------------

/// Initializes the empty object. \p operation is the function that returned the error code.
libc_exception::libc_exception (const char* operation)
: exception(),
  m_Errno (errno),
  m_Operation (operation)
{
    set_format (xfmt_LibcException);
}

/// Copies object \p v.
libc_exception::libc_exception (const libc_exception& v)
: exception (v),
  m_Errno (v.m_Errno),
  m_Operation (v.m_Operation)
{
}

/// Copies object \p v.
const libc_exception& libc_exception::operator= (const libc_exception& v)
{
    m_Errno = v.m_Errno;
    m_Operation = v.m_Operation;
    return (*this);
}

/// Returns the name of the exception.
const char* libc_exception::what (void) const
{
    return ("libc function failed");
}

/// Returns a descriptive error message. fmt="%s: %m (%d)"
void libc_exception::info (string& msgbuf, const char* fmt) const
{
    if (!fmt) fmt = "%s: %m (%d)";
    try { msgbuf.format (fmt, m_Operation, m_Errno, m_Errno); } catch (...) {}
}

/// Reads the exception from stream \p is.
void libc_exception::read (istream& is)
{
    exception::read (is);
    is >> m_Errno >> m_Operation;
}

/// Writes the exception into stream \p os.
void libc_exception::write (ostream& os) const
{
    exception::write (os);
    os << m_Errno << m_Operation;
}

/// Returns the size of the written exception.
size_t libc_exception::stream_size (void) const
{
    return (exception::stream_size() +
	    stream_size_of(m_Errno) +
	    stream_size_of(m_Operation));
}

//----------------------------------------------------------------------

/// Initializes the empty object. \p operation is the function that returned the error code.
file_exception::file_exception (const char* operation, const char* filename)
: libc_exception (operation),
  m_Filename()
{
    set_format (xfmt_FileException);
    strncpy (m_Filename, filename, PATH_MAX - 1);
}

/// Returns the name of the libc_exception.
const char* file_exception::what (void) const
{
    return ("file exception");
}

/// Returns a descriptive error message. fmt="%s %s: %m (%d)"
void file_exception::info (string& msgbuf, const char* fmt) const
{
    if (!fmt) fmt = "%s %s: %m (%d)";
    try { msgbuf.format (fmt, m_Operation, m_Filename, m_Errno, m_Errno); } catch (...) {}
}

/// Reads the exception from stream \p is.
void file_exception::read (istream& is)
{
    libc_exception::read (is);
    string filename;
    is >> filename >> ios::align();
    strncpy (m_Filename, filename, PATH_MAX - 1);
}

/// Writes the exception into stream \p os.
void file_exception::write (ostream& os) const
{
    libc_exception::write (os);
    os << string(m_Filename) << ios::align();
}

/// Returns the size of the written exception.
size_t file_exception::stream_size (void) const
{
    return (libc_exception::stream_size() +
	    stream_size_of(string (m_Filename)));
}

//----------------------------------------------------------------------

}; // namespace ustl

