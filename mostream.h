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
// mostream.h

#ifndef MOSTREAM_H_24A8C5397E0848216573E5670930FC9A
#define MOSTREAM_H_24A8C5397E0848216573E5670930FC9A

#include "memlink.h"
#include "uexception.h"
#include <typeinfo>

namespace ustl {

class istream;
class string;

/// \class ostream mostream.h ustl.h
/// \ingroup BinaryStreams
///
/// \brief Helper class to write packed binary streams.
///
/// This class contains a set of functions to write integral types into an
/// unstructured memory block. Packing binary file data can be done this
/// way, for instance. aligning the data is your responsibility, and can
/// be accomplished by proper ordering of writes and by calling the align()
/// function. Unaligned access is usually slower by orders of magnitude and,
/// on some architectures, such as PowerPC, can cause your program to crash.
/// Therefore, all write functions have asserts to check alignment.
/// Overwriting the end of the stream will also cause a crash (an assert in
/// debug builds). Oh, and don't be intimidated by the size of the inlines
/// here. In the assembly code the compiler will usually chop everything down
/// to five instructions each.
///
/// Example code:
/// \code
///     memblock b;
///     ostream os (b);
///     os << boolVar;
///     os.align (sizeof(int));
///     os << intVar << floatVar;
///     os.write (binaryData, binaryDataSize);
///     os.align ();
///     b.resize (os.pos());
///     write (fd, b, b.size());
/// \endcode
///
class ostream : public memlink {
public:
			ostream (void);
			ostream (void* p, size_type n);
    explicit		ostream (const memlink& source);
    inline void		seek (uoff_t newPos);
    inline void		seek (const_iterator newPos);
    inline void		skip (size_type nBytes);
    inline uoff_t	pos (void) const;
    inline iterator	ipos (void);
    inline const_iterator ipos (void) const;
    inline size_type	remaining (void) const;
    inline bool		aligned (size_type grain = c_DefaultAlignment) const;
    inline size_type	align_size (size_type grain = c_DefaultAlignment) const;
    inline void		align (size_type grain = c_DefaultAlignment);
    void		write (const void* buffer, size_type size);
    inline void		write (const cmemlink& buf);
    void		write_strz (const char* str);
    void		read (istream& is);
    void		write (ostream& os) const;
    void		insert (iterator start, size_type size);
    void		erase (iterator start, size_type size);
    void		swap (ostream& os);
    inline size_t	stream_size (void) const;
    template <typename T>
    inline void		iwrite (const T& v);
    virtual void	unlink (void);
    inline void		link (void* p, size_type n)		{ memlink::link (p, n); }
    inline void		link (memlink& l)			{ memlink::link (l.data(), l.writable_size()); }
    inline void		link (void* f, void* l)			{ memlink::link (f, l); }
			OVERLOAD_POINTER_AND_SIZE_T_V2(link, void*)
private:
    uoff_t		m_Pos;	///< Current write position.
};

//----------------------------------------------------------------------

/// \class ostream_iterator mostream.h ustl.h
/// \ingroup BinaryStreamIterators
///
/// \brief An iterator over an ostream to use with uSTL algorithms.
///
template <class T>
class ostream_iterator {
public:
    typedef T			value_type;
    typedef ptrdiff_t		difference_type;
    typedef value_type*		pointer;
    typedef value_type&		reference;
    typedef ostream::size_type	size_type;
public:
    inline explicit		ostream_iterator (ostream& os)
				    : m_Os (os) {}
    inline			ostream_iterator (const ostream_iterator& iter)
				    : m_Os (iter.m_Os) {} 
    /// Writes \p v into the stream.
    inline ostream_iterator&	operator= (const T& v)
				    { m_Os << v; return (*this); }
    inline ostream_iterator&	operator* (void) { return (*this); }
    inline ostream_iterator&	operator++ (void) { return (*this); }
    inline ostream_iterator	operator++ (int) { return (*this); }
    inline ostream_iterator&	operator+= (size_type n) { m_Os.skip (n); return (*this); }
    inline bool			operator== (const ostream_iterator& i) const
				    { return (m_Os.pos() == i.m_Os.pos()); }
    inline bool			operator< (const ostream_iterator& i) const
				    { return (m_Os.pos() < i.m_Os.pos()); }
private:
    ostream&	m_Os;
};

//----------------------------------------------------------------------

/// Returns the current write position. Usually this is also the number of bytes written.
inline uoff_t ostream::pos (void) const
{
    return (m_Pos);
}

/// Returns the current write position
inline ostream::iterator ostream::ipos (void)
{
    return (begin() + m_Pos);
}

/// Returns the current write position
inline ostream::const_iterator ostream::ipos (void) const
{
    return (begin() + m_Pos);
}

/// Move the write pointer to \p newPos
inline void ostream::seek (uoff_t newPos)
{
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (newPos > size())
	throw stream_bounds_exception ("seek", "", pos(), newPos - pos(), size());
#else
    assert (newPos <= size());
#endif
    m_Pos = newPos;
}

/// Sets the current read position to \p newPos
inline void ostream::seek (const_iterator newPos)
{
    seek (distance (begin(), const_cast<iterator>(newPos)));
}

/// Skips \p nBytes without writing anything.
inline void ostream::skip (size_type nBytes)
{
    seek (pos() + nBytes);
}

/// Returns number of bytes remaining in the write buffer.
inline ostream::size_type ostream::remaining (void) const
{
    return (size() - pos());
}

/// Returns \c true if the write pointer is aligned on \p grain
inline bool ostream::aligned (size_type grain) const
{
    assert (uintptr_t(begin()) % grain == 0 && "Streams should be attached aligned at the maximum element grain to avoid bus errors.");
    return (pos() % grain == 0);
}

/// Returns the number of bytes to skip to be aligned on \p grain.
inline ostream::size_type ostream::align_size (size_type grain) const
{
    return (Align (pos(), grain) - pos());
}

/// Aligns the write pointer on \p grain. Nothing is written to the skipped bytes.
inline void ostream::align (size_type grain)
{
    seek (Align (pos(), grain));
}

/// Writes the contents of \p buf into the stream as a raw dump.
inline void ostream::write (const cmemlink& buf)
{
    write (buf.begin(), buf.size());
}

/// Returns number of bytes written.
inline size_t ostream::stream_size (void) const
{
    return (pos());
}

/// Writes type T into the stream via a direct pointer cast.
template <typename T>
inline void ostream::iwrite (const T& v)
{
    assert (aligned (alignof (v)));
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (remaining() < sizeof(T))
	throw stream_bounds_exception ("write", typeid(v).name(), pos(), sizeof(T), remaining());
#else
    assert (remaining() >= sizeof(T));
#endif
    void* pv = begin() + pos();
    *reinterpret_cast<T*>(pv) = v;
    skip (sizeof(T));
}

template <typename T>
inline ostream&	operator<< (ostream& os, T* v)		{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, int8_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, uint8_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, int16_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, uint16_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, int32_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, uint32_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, float v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, double v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, wchar_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, bool v)
{
    if (sizeof(bool) == sizeof(uint8_t))
	os.iwrite(v);
    else
	os.iwrite (uint8_t(v));
    return (os);
}
#if HAVE_THREE_CHAR_TYPES
inline ostream&	operator<< (ostream& os, char v)	{ os.iwrite(v); return (os); }
#endif
#if HAVE_INT64_T
inline ostream&	operator<< (ostream& os, int64_t v)	{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, uint64_t v)	{ os.iwrite(v); return (os); }
#endif
#if SIZE_OF_LONG == SIZE_OF_INT
inline ostream&	operator<< (ostream& os, long v)		{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, unsigned long v)	{ os.iwrite(v); return (os); }
#endif
#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
inline ostream&	operator<< (ostream& os, long long v)		{ os.iwrite(v); return (os); }
inline ostream&	operator<< (ostream& os, unsigned long long v)	{ os.iwrite(v); return (os); }
#endif

} // namespace ustl

#endif

