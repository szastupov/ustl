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
// uiosfunc.h
//

#ifndef UIOSFUNC_H_730C16E316F7650E3A02E1C6611B789A
#define UIOSFUNC_H_730C16E316F7650E3A02E1C6611B789A

#include "uios.h"

namespace ustl {
namespace ios {
    /// Stream functor to allow inline align() calls. os << align(sizeof(u_short));
    class align {
    public:
				align (size_t grain = c_DefaultAlignment) : m_Grain(grain) {}
	inline istream&		apply (istream& is) const { is.align (m_Grain); return (is); }
	inline ostream&		apply (ostream& os) const { os.align (m_Grain); return (os); }
	friend inline size_t	stream_size_of (const align& op);
    private:
	size_t			m_Grain;
    };
    inline istream& operator>> (istream& is, const align& op) { return (op.apply (is)); }
    inline ostream& operator<< (ostream& os, const align& op) { return (op.apply (os)); }
    inline size_t stream_size_of (const align& op) { return (op.m_Grain - 1); }

    /// Stream functor to allow inline skip() calls. os << skip(sizeof(u_short));
    class skip {
    public:
				skip (size_t nBytes) : m_nBytes(nBytes) {}
	inline istream&		apply (istream& is) const { is.skip (m_nBytes); return (is); }
	inline ostream&		apply (ostream& os) const { os.skip (m_nBytes); return (os); }
	friend inline size_t	stream_size_of (const skip& op);
    private:
	size_t			m_nBytes;
    };
    inline istream& operator>> (istream& is, const skip& op) { return (op.apply (is)); }
    inline ostream& operator<< (ostream& os, const skip& op) { return (op.apply (os)); }
    inline size_t stream_size_of (const skip& op) { return (op.m_nBytes); }
}; // namespace ios
}; // namespace ustl

CAST_STREAMABLE(ustl::ios::fmtflags, u_int)
CAST_STREAMABLE(ustl::ios::openmode, u_int)
CAST_STREAMABLE(ustl::ios::seekdir, u_int)

#endif

