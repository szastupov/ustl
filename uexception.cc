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
#include <errno.h>
#include <string.h>

namespace ustl {

//----------------------------------------------------------------------

/// Initializes an empty object
exception::exception (void)
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

/// Returns a descriptive error message. fmt="%s"
/** Overloads of this functions must set NULL as the default fmt
 * argument and handle that case to provide a default format string
 * in case the user does not have a localized one. The format
 * string should be shown in the documentation to not require
 * translators to look through code. Also, this function must
 * not throw anything, so you must wrap memory allocation routines
 * (like string::format, for instance) in a try{}catch(...){} block.
*/
void exception::info (string& msgbuf, const char*) const
{
    try { msgbuf.format ("%s", what()); } catch (...) { /* Ignore all exceptions */ }
}

//----------------------------------------------------------------------

/// Initializes the empty object. \p nBytes is the size of the attempted allocation.
bad_alloc::bad_alloc (size_t nBytes)
: exception(),
  m_nBytesRequested (nBytes)
{
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

//----------------------------------------------------------------------

/// Initializes the empty object. \p operation is the function that returned the error code.
libc_exception::libc_exception (const char* operation)
: exception(),
  m_Errno (errno),
  m_Operation (operation)
{
}

/// Copies object \p v.
libc_exception::libc_exception (const libc_exception& v)
: exception(),
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

//----------------------------------------------------------------------

/// Initializes the empty object. \p operation is the function that returned the error code.
file_exception::file_exception (const char* operation, const char* filename)
: libc_exception (operation),
  m_Filename()
{
    memset (m_Filename, 0, PATH_MAX);
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

//----------------------------------------------------------------------

} // namespace ustl

