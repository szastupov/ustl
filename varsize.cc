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
// varsize.cc
//

#include "mistream.h"
#include "mostream.h"
#include "strmsize.h"
#include "varsize.h"

namespace ustl {

/// Constructs with size 0
varsize::varsize (void)
: m_Size (0)
{
}

/// Constructs with size \p s
varsize::varsize (size_t s)
: m_Size (s)
{
}

/// Returns the number of bytes required to write the object (u_long aligned)
size_t varsize::stream_size (void) const
{
    // Assumes that write starts on a u_long boundary.
    return (m_Size < numeric_limits<u_char>::max() ? sizeof(u_char) :
	    (m_Size < numeric_limits<u_short>::max() ? sizeof(u_long) : sizeof(u_long) * 2));
}

/// Reads the object from stream \p is
void varsize::read (istream& is)
{
    u_char bs;
    is >> bs;
    if (bs < numeric_limits<u_char>::max())
	m_Size = bs;
    else {
	u_short ss;
	is.align (sizeof(u_short));
	is >> ss;
	if (ss < numeric_limits<u_short>::max())
	    m_Size = ss;
	else {
	    u_long ls;
	    is.align (sizeof(u_long));
	    is >> ls;
	    m_Size = ls;
	}
    }
}

/// Writes the object into stream \p os
void varsize::write (ostream& os) const
{
    if (m_Size < numeric_limits<u_char>::max())
	os << u_char(m_Size);
    else {
	os << numeric_limits<u_char>::max();
	os.align (sizeof(u_short));
	if (m_Size < numeric_limits<u_short>::max())
	    os << u_short(m_Size);
	else {
	    os << numeric_limits<u_short>::max();
	    os.align (sizeof(u_long));
	    os << u_long(m_Size);
	}
    }
}

} // namespace ustl

