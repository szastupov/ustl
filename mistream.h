// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//
// mistream.h
//
#ifndef MISTREAM_H_103AEF1F266C04AA1A817D38705983DA
#define MISTREAM_H_103AEF1F266C04AA1A817D38705983DA

#include "cmemlink.h"
#include "uexception.h"
#ifdef WANT_STREAM_BOUNDS_CHECKING
    #include <typeinfo>
#endif

namespace ustl {

class ostream;
class memlink;
class string;

/// \class istream mistream.h ustl.h
/// \ingroup BinaryStreams
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
///	\li Assume your writes start off aligned.
///	\li After completion, align the stream.
///	\li Non-default alignment is allowed if you plan to frequently write this
///	object in array form and alignment would be costly. For example, an
///	array of uint16_t-sized objects may leave the stream uint16_t aligned
///	as long as you know about it and will default-align the stream after
///	writing the array (note: vector<T> will already do this for you)
/// 
/// Example code:
/// \code
///	memblock b;
///	int br = read (fd, b, b.size());
///	b.resize (br);
///	ostream is (b);
///	is >> boolVar;
///	is.align (sizeof(int));
///	is >> intVar >> floatVar;
///	is.read (binaryData, binaryDataSize);
///	is.align ();
///	// Assuming the input is written by code in mostream.h
///	assert (is.pos() == b.size()); 
/// \endcode
///
class istream : public cmemlink {
public:
			istream (void);
			istream (const void* p, size_type n);
    explicit		istream (const cmemlink& source);
    explicit		istream (const ostream& source);
    inline void		link (const void* p, size_type n)	{ cmemlink::link (p, n); }
    inline void		link (const cmemlink& l)		{ cmemlink::link (l.cdata(), l.readable_size()); }
    inline void		link (const void* f, const void* l)	{ cmemlink::link (f, l); }
			OVERLOAD_POINTER_AND_SIZE_T_V2(link, const void*)
    virtual void	unlink (void);
    inline void		seek (uoff_t newPos);
    inline void		seek (const_iterator newPos);
    inline void		skip (size_type nBytes);
    inline uoff_t	pos (void) const;
    inline const_iterator ipos (void) const;
    inline size_type	remaining (void) const;
    inline bool		aligned (size_type grain = c_DefaultAlignment) const;
    inline size_type	align_size (size_type grain = c_DefaultAlignment) const;
    inline void		align (size_type grain = c_DefaultAlignment);
    void		swap (istream& is);
    void		read (void* buffer, size_type size);
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

/// \class istream_iterator
/// \ingroup BinaryStreamIterators
///
/// \brief An iterator over an istream to use with uSTL algorithms.
///
template <class T>
class istream_iterator {
public:
    typedef T			value_type;
    typedef ptrdiff_t		difference_type;
    typedef const value_type*	pointer;
    typedef const value_type&	reference;
    typedef istream::size_type	size_type;
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
    inline istream_iterator&	operator+= (size_type n) { m_vPos += n; return (*this); }
    inline bool			operator== (const istream_iterator& i) const
				    { return (m_Is.pos() == i.m_Is.pos()); }
    inline bool			operator< (const istream_iterator& i) const
				    { return (m_Is.pos() < i.m_Is.pos()); }
private:
    istream&	m_Is;		///< The host stream.
    T		m_v;		///< Last read value; cached to be returnable as a const reference.
    uoff_t	m_vPos;		///< Current position.
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
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (newPos > size())
	throw stream_bounds_exception ("seek", "", pos(), newPos - pos(), size());
#else
    assert (newPos <= size());
#endif
    m_Pos = newPos;
}

/// Sets the current read position to \p newPos
inline void istream::seek (const_iterator newPos)
{
    seek (distance (begin(), newPos));
}

/// skips \p nBytes without reading anything.
inline void istream::skip (size_type nBytes)
{
    seek (pos() + nBytes);
}

/// Returns the number of bytes remaining in the input buffer.
inline istream::size_type istream::remaining (void) const
{
    return (size() - pos());
}

/// Returns the number of bytes to skip to be aligned on \p grain.
inline istream::size_type istream::align_size (size_type grain) const
{
    return (Align (pos(), grain) - pos());
}

/// Returns \c true if the read position is aligned on \p grain
inline bool istream::aligned (size_type grain) const
{
    assert (uintptr_t(begin()) % grain == 0 && "Streams should be attached aligned at the maximum element grain to avoid bus errors.");
    return (pos() % grain == 0);
}

/// aligns the read position on \p grain
inline void istream::align (size_type grain)
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
    assert (aligned (alignof (v)));
#ifdef WANT_STREAM_BOUNDS_CHECKING
    if (remaining() < sizeof(T))
	throw stream_bounds_exception ("read", typeid(v).name(), pos(), sizeof(T), remaining());
#else
    assert (remaining() >= sizeof(T));
#endif
    const void* pv = begin() + pos();
    v = *reinterpret_cast<const T*>(pv);
    skip (sizeof(T));
}

template <typename T>
inline istream& operator>> (istream& is, T*& v)		{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, int8_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, uint8_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, int16_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, uint16_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, int32_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, uint32_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, float& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, double& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, wchar_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, bool& v)
{
    if (sizeof(bool) == sizeof(uint8_t))
	is.iread(v);
    else {
	uint8_t v8;
	is.iread (v8);
	v = v8;
    }
    return (is);
}
#if HAVE_THREE_CHAR_TYPES
inline istream&	operator>> (istream& is, char& v)	{ is.iread(v); return (is); }
#endif
#if HAVE_INT64_T
inline istream&	operator>> (istream& is, int64_t& v)	{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, uint64_t& v)	{ is.iread(v); return (is); }
#endif
#if SIZE_OF_LONG == SIZE_OF_INT
inline istream&	operator>> (istream& is, long& v)		{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, unsigned long& v)	{ is.iread(v); return (is); }
#endif
#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
inline istream&	operator>> (istream& is, long long& v)		{ is.iread(v); return (is); }
inline istream&	operator>> (istream& is, unsigned long long& v)	{ is.iread(v); return (is); }
#endif

} // namespace ustl

#endif

