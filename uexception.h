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
/** \file uexception.h
 *
 * \brief This file contains stuff from <exception> and <new>, the standard
 * 	C++ headers, because uSTL is intended to completely replace all C++
 * 	standard library functions.
 */

#ifndef UEXCEPTION
#define UEXCEPTION

#include "utypes.h"

namespace ustl {

class string;

/** \class ustl::exception
 * \brief base class for exceptions, equivalent to std::exception.
 */
class exception {
public:
    			exception (void);
    virtual	       ~exception (void);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
};

/** \class ustl::bad_alloc
 * \brief Exception thrown on memory allocation failure by memblock::reserve.
 *
 * The standard C++ new will still throw std::bad_alloc though, so you need to
 * catch that too if you are using new.
 */
class bad_alloc : public exception {
public:
    			bad_alloc (size_t nBytes);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
protected:
    size_t		m_nBytesRequested;	///< Number of bytes requested by the failed allocation.
};

/** \class ustl::libc_exception
 * \brief Thrown when a libc function returns an error. Contains an errno and description.
*/
class libc_exception : public exception {
public:
    			libc_exception (const char* operation);
			libc_exception (const libc_exception& v);
    const libc_exception& operator= (const libc_exception& v);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
protected:
    int			m_Errno;		///< Error code returned by the failed operation.
    const char*		m_Operation;		///< Name of the failed operation.
};

/** \class ustl::file_exception
 * \brief File-related exceptions.
*/
class file_exception : public libc_exception {
public:
    			file_exception (const char* operation, const char* filename);
    virtual const char*	what (void) const;
    virtual void	info (string& msgbuf, const char* fmt = NULL) const;
protected:
    char		m_Filename [PATH_MAX];	///< Name of the file causing the error.
};

} // namespace ustl

#endif

