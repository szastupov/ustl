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
/// \file utypes.h
///
/// \brief Types used by this library.
///

#ifndef UTYPES_H
#define UTYPES_H

#include <sys/types.h>
#include <stddef.h>	// For ptrdiff_t, size_t
#include <stdlib.h>	// for NULL
#include <limits.h>
#include <float.h>
#ifndef unix
    typedef unsigned char   u_char;
    typedef unsigned short  u_short;
    typedef unsigned int    u_int;
    typedef unsigned long   u_long;
#endif
#include "ulimits.h"

typedef size_t	uoff_t; ///< A type for storing offsets into blocks measured by size_t.

#endif

