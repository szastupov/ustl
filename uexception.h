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
// uexception.h
//
// This file contains stuff from \<exception\>.
// The standard C++ headers are duplicated because uSTL is intended
// to completely replace all C++ standard library functions.
//

#ifndef UEXCEPTION
#define UEXCEPTION

#include "utypes.h"

namespace ustl {

class string;
class istream;
class ostream;
class ostringstream;

typedef u_int		xfmt_t;

static const xfmt_t	xfmt_Exception			= 0;
static const xfmt_t	xfmt_BadAlloc			= 1;
static const xfmt_t	xfmt_LibcException		= 12;
static const xfmt_t	xfmt_FileException		= 13;
#ifdef WANT_STREAM_BOUNDS_CHECKING
static const xfmt_t	xfmt_StreamBoundsException	= 14;
#endif

/// Base class for exceptions, equivalent to std::exception.
class exception {
public:
    			exception (void);
    virtual	       ~exception (void);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    void		text_write (ostringstream& os) const;
    virtual size_t	stream_size (void) const;
    /// Format of the exception is used to lookup exception::info format string.
    /// Another common use is the instantiation of serialized exceptions, used
    /// by the error handler node chain to troubleshoot specific errors.
    inline xfmt_t	format (void) const { return (m_Format); }
protected:
    inline void		set_format (xfmt_t fmt) { m_Format = fmt; }
private:
    xfmt_t		m_Format;	///< Format of the exception's data.
};

/// Thrown to indicate a bad dynamic_cast usage.
class bad_cast : public exception {
public:
    explicit		bad_cast (void);
    virtual const char*	what (void) const;
};

/// Exception thrown on memory allocation failure by memblock::reserve.
class bad_alloc : public exception {
public:
    explicit		bad_alloc (size_t nBytes);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    virtual size_t	stream_size (void) const;
protected:
    size_t		m_nBytesRequested;	///< Number of bytes requested by the failed allocation.
};

/// Thrown when a libc function returns an error. Contains an errno and description.
class libc_exception : public exception {
public:
    explicit		libc_exception (const char* operation);
			libc_exception (const libc_exception& v);
    const libc_exception& operator= (const libc_exception& v);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    virtual size_t	stream_size (void) const;
protected:
    int			m_Errno;		///< Error code returned by the failed operation.
    const char*		m_Operation;		///< Name of the failed operation.
};

/// File-related exceptions. Contains the file name.
class file_exception : public libc_exception {
public:
			file_exception (const char* operation, const char* filename);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    virtual size_t	stream_size (void) const;
protected:
    char		m_Filename [PATH_MAX];	///< Name of the file causing the error.
};

#ifdef WANT_STREAM_BOUNDS_CHECKING
/// Stream bounds checking.
/// Only thrown in debug builds unless you say otherwise in config.h
///
class stream_bounds_exception : public libc_exception {
public:
			stream_bounds_exception (const char* operation, const char* type, uoff_t offset, size_t expected, size_t remaining);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
    virtual void	read (istream& is);
    virtual void	write (ostream& os) const;
    virtual size_t	stream_size (void) const;
protected:
    const char*		m_TypeName;
    uoff_t		m_Offset;
    size_t		m_Expected;
    size_t		m_Remaining;
};
#endif

}; // namespace ustl

#endif

