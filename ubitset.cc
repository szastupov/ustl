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
// ubitset.cc
//

#include "ubitset.h"

namespace ustl {

/// Copies bits from \p v of size \p n into \p buf as MSB "1011001..." LSB
/// If \p buf is too small, MSB bits will be truncated.
void convert_to_bitstring (const u_int* v, size_t n, string& buf)
{
    string::iterator stri = buf.end();
    for (size_t i = 0; i < n && stri > buf.begin(); ++ i)
	for (size_t b = 1; b != 0 && stri > buf.begin(); b <<= 1)
	    *--stri = (v[i] & b) ? '1' : '0';
}

/// Copies bits from \p buf as MSB "1011001..." LSB into \p v of size \p n.
void convert_from_bitstring (const string& buf, u_int* v, size_t n)
{
    string::const_iterator stri = buf.end();
    for (size_t i = 0; i < n; ++ i) {
	for (size_t b = 1; b != 0; b <<= 1) {
	    if (stri == buf.begin() || *--stri == '0')
		v[i] &= ~b;
	    else
		v[i] |= b;
	}
    }
}

}; // namespace ustl

