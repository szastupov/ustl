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
// utypes.h
//
// Types used by this library.
//

#ifndef UTYPES_H_118BBB3B50B7DBF22F5460C52E515C83
#define UTYPES_H_118BBB3B50B7DBF22F5460C52E515C83

#include "config.h"
#define __STDC_LIMIT_MACROS	// For WCHAR_MIN and WCHAR_MAX in stdint.
#define __STDC_CONSTANT_MACROS	// For UINT??_C macros to avoid using L and UL suffixes on constants.
#ifdef HAVE_STDINT_H
    #include <stdint.h>
#endif
#include <stddef.h>		// For ptrdiff_t, size_t
#include <limits.h>
#include <float.h>
#ifdef HAVE_SYS_TYPES_H
    #include <sys/types.h>
#else
    typedef unsigned char	u_char;
    typedef unsigned short	u_short;
    typedef unsigned int	u_int;
    typedef unsigned long	u_long;
#endif

typedef size_t	uoff_t;		///< A type for storing offsets into blocks measured by size_t.
typedef u_long	hashvalue_t;	///< Value type returned by the hash functions.

#endif

