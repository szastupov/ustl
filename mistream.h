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
// mistream.h
//
#ifndef MISTREAM_H
#define MISTREAM_H

#include "cmemlink.h"

namespace ustl {

class ostream;
class memlink;
class string;

///
/// \brief Helper class to read packed binary streams.
/// 
/// This class contains a set of functions to read integral types from an
/// unstructured memory block. Unpacking binary file data can be done this
/// way, for instance. aligning the data is your responsibility, and can
/// be accomplished by proper ordering of reads and by calling the align()
/// function. Unaligned access is usually slower by orders of magnitude and,
/// on some architectures, such as PowerPC, can cause your program to crash.
/// Therefore, all read functions have asserts to check alignment.
/// Overreading the end of the stream will also cause a crash (an assert in
/// debug builds). Oh, and don't be intimidated by the size of the inlines
/// here. In the assembly code the compiler will usually chop everything down
/// to five instructions each.
/// 
/// Alignment rules for your objects:
///	\li Assume your writes start off u_long aligned.
///	\li After completion, align the stream to u_long.
///	\li Non-u_long alignment is allowed if you plan to frequently write this
///	object in array form and alignment would be costly. For example, an
///	array of u_short-sized objects may leave the stream u_short aligned
///	as long as you know about it and will u_long-align the stream after
///	writing the array (note: vector<T> will already do this for you)
/// 
/// Example code:
/// \code
///	CMemoryBlock b;
///	int br = read (fd, b, b.size());
///	b.SetSize (br);
///	COStream is (b);
///	is >> boolVar;
///	is.align (sizeof(int));
///	is >> intVar >> floatVar;
///	is.read (binaryData, binaryDataSize);
///	is.align (sizeof(u_long));
///	// Assuming the input is written by code in mostream.h
///	assert (is.pos() == b.size()); 
/// \endcode
///
class istream : public cmemlink {
public:
			istream (void);
			istream (const void* p, size_t n);
    explicit		istream (const cmemlink& source);
    virtual void	unlink (void);
    inline void		seek (uoff_t newPos);
    inline void		seek (const_iterator newPos);
    inline void		skip (size_t nBytes);
    inline uoff_t	pos (void) const;
    inline const_iterator ipos (void) const;
    inline size_t	remaining (void) const;
    inline bool		aligned (size_t grain = c_DefaultAlignment) const;
    inline void		align (size_t grain = c_DefaultAlignment);
    void		swap (istream& is);
    void		read (void* buffer, size_t size);
    void		read (memlink& buf);
    void		read_strz (string& str);
    void		read (istream& is);
    void		write (ostream& is) const;
    inline size_t	stream_size (void) const;
    template <typename T>
    inline void		iread (T& v);
private:
    uoff_t		m_Pos;		///< The current read position.
};

//----------------------------------------------------------------------

/// An iterator over an istream to use with uSTL algorithms.
template <class T>
class istream_iterator {
public:
    typedef T			value_type;
    typedef ptrdiff_t		difference_type;
    typedef const value_type*	pointer;
    typedef const value_type&	reference;
public:
    explicit			istream_iterator (istream& is)
				    : m_Is (is), m_v (T()), m_vPos (is.size()) {}
 				istream_iterator (const istream_iterator& i)
				    : m_Is (i.m_Is), m_v (i.m_v), m_vPos (i.m_vPos) {} 
    inline const istream_iterator& operator= (const istream_iterator& i)
				    { m_Is = i.m_Is; m_v = i.m_v; m_vPos = i.m_vPos; return (*this); }
    /// Reads and returns the next value.
    inline const T&		operator* (void)
    				{
				    if (m_vPos != m_Is.pos()) {
					m_Is >> m_v;
					m_vPos = m_Is.pos();
				    }
				    return (m_v);
				}
    inline istream_iterator&	operator++ (void) { ++ m_vPos; return (*this); }
    inline istream_iterator	operator++ (int) { istream_iterator old (*this); ++ m_vPos; return (old); }
    inline istream_iterator&	operator+= (size_t n) { m_vPos += n; return (*this); }
    inline bool			operator== (const istream_iterator& i) const
				    { return (m_Is.pos() == i.m_Is.pos()); }
    inline bool			operator< (const istream_iterator& i) const
				    { return (m_Is.pos() < i.m_Is.pos()); }
private:
    istream&	m_Is;
    T		m_v;
    uoff_t	m_vPos;
};

//----------------------------------------------------------------------

/// Returns the current read position
inline uoff_t istream::pos (void) const
{
    return (m_Pos);
}

/// Returns the current read position
inline istream::const_iterator istream::ipos (void) const
{
    return (begin() + m_Pos);
}

/// Sets the current read position to \p newPos
inline void istream::seek (uoff_t newPos)
{
    assert (newPos <= size());
    m_Pos = newPos;
}

/// Sets the current read position to \p newPos
inline void istream::seek (const_iterator newPos)
{
    assert (newPos >= begin() && newPos <= end());
    m_Pos = distance (begin(), newPos);
}

/// skips \p nBytes without reading anything.
inline void istream::skip (size_t nBytes)
{
    seek (pos() + nBytes);
}

/// Returns the number of bytes remaining in the input buffer.
inline size_t istream::remaining (void) const
{
    return (size() - pos());
}

/// Returns \c true if the read position is aligned on \p grain
inline bool istream::aligned (size_t grain) const
{
    return (pos() % grain == 0);
}

/// aligns the read position on \p grain
inline void istream::align (size_t grain)
{
    seek (Align (pos(), grain));
}

/// Returns number of unread bytes.
inline size_t istream::stream_size (void) const
{
    return (remaining());
}

/// Reads type T from the stream via a direct pointer cast.
template <typename T>
inline void istream::iread (T& v)
{
    assert (aligned (sizeof(T) < c_DefaultAlignment ? sizeof(T) : c_DefaultAlignment));
    assert (remaining() >= sizeof(T));
    const void* pv = begin() + pos();
    v = *reinterpret_cast<const T*>(pv);
    skip (sizeof(T));
}

template <typename T>
inline istream& operator>> (istream& is, T*& v)		{ is.iread(v); return (is); }
template <typename T>
inline istream& operator>> (istream& is, const T*& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, signed char& v){ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, char& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, short& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, int& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, long& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, u_char& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, u_short& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, u_int& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, u_long& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, float& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, double& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, bool& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, wchar_t& v)	{ is.iread(v); return (is); }

}; // namespace ustl

#endif

