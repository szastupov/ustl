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
// utf8.h
//
// This file contains stream iterators that read and write UTF-8 encoded
// characters. The encoding is defined as follows:
//
// U-00000000 - U-0000007F: 0xxxxxxx
// U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
// U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
// U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// U-80000000 - U-FFFFFFFF: 11111110 100000xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//
// The last range in not in the UTF-8 standard because Unicode forbids
// characters of those values. However, since ustl::string uses this code
// to write its length, the support is here. The reason it was put here
// in the first place, is that extra code would have been necessary to
// flag that range as invalid.
//
#ifndef UTF8_H_3D7AEEEB3A88928D4D280B785F78B6F4
#define UTF8_H_3D7AEEEB3A88928D4D280B785F78B6F4

#include "uiterator.h"
#include "mistream.h"
#include "mostream.h"

namespace ustl {

//----------------------------------------------------------------------

/// Returns the number of bytes required to UTF-8 encode \p v.
inline size_t Utf8Bytes (wchar_t v)
{
    const uint32_t c_Bounds[] = { 0x00000080, 0x00000800, 0x00010000, 0x00200000, 0x04000000, 0x80000000, 0xFFFFFFFF, };
    size_t bi = 0;
    while (c_Bounds[bi++] <= uint32_t(v))
	;
    return (bi);
}

/// Returns the number of bytes in a UTF-8 sequence that starts with \p c.
inline size_t Utf8SequenceBytes (u_char c)
{
    //
    // Count the leading bits. Header bits are 1 * nBytes followed by a 0.
    //	0 - single byte character. Take 7 bits (0xFF >> 1)
    //	1 - error, in the middle of the character. Take 6 bits (0xFF >> 2)
    //	    so you will keep reading invalid entries until you hit the next character.
    //	>2 - multibyte character. Take remaining bits, and get the next bytes.
    // All errors are ignored, since nothing can be done about them.
    //
    u_char mask = 0x80;
    size_t nBytes = 0;
    while (c & mask) {
	mask >>= 1;
	++ nBytes;
    }
    return (nBytes);
}

//----------------------------------------------------------------------

/// An iterator adaptor to character containers for reading UTF-8 encoded text.
///
/// For example, you can copy from ustl::string to ustl::vector<wchar_t> with
/// copy (utf8in (str.begin()), utf8in (str.end()), back_inserter(wvect));
/// There is no error handling; if the reading frame slips you'll get extra
/// characters, one for every misaligned byte. Although it is possible to skip
/// to the start of the next character, that would result in omitting the
/// misformatted character and the one after it, making it very difficult to
/// detect by the user. It is better to write some strange characters and let
/// the user know his file is corrupted.
///
template <typename Iterator, typename WChar = wchar_t>
class utf8in_iterator {
public:
    typedef typename iterator_traits<Iterator>::value_type	value_type;
    typedef typename iterator_traits<Iterator>::difference_type	difference_type;
    typedef typename iterator_traits<Iterator>::pointer		pointer;
    typedef typename iterator_traits<Iterator>::reference	reference;
public:
    explicit			utf8in_iterator (const Iterator& is) : m_v (0), m_i (is) { operator++(); }
				utf8in_iterator (const utf8in_iterator& i) : m_v (i.m_v), m_i (i.m_i) {} 
    inline const utf8in_iterator& operator= (const utf8in_iterator& i) { m_v = i.m_v; m_i = i.m_i; return (*this); }
    /// Reads and returns the next value.
    inline const WChar&		operator* (void) const { return (m_v); }
    utf8in_iterator&		operator++ (void);
    inline utf8in_iterator	operator++ (int) { utf8in_iterator old (*this); operator++(); return (old); }
    inline bool			operator== (const utf8in_iterator& i) const { return (m_i == i.m_i); }
    inline bool			operator< (const utf8in_iterator& i) const { return (m_i < i.m_i); }
    inline utf8in_iterator&	operator+= (size_t n) { while (n--) operator++(); return (*this); }
    difference_type		operator- (const utf8in_iterator& i) const;
private:
    wchar_t			m_v;
    Iterator			m_i;
};

/// Steps to the next character and updates current returnable value.
template <typename Iterator, typename WChar>
utf8in_iterator<Iterator,WChar>& utf8in_iterator<Iterator,WChar>::operator++ (void)
{
    u_char c = *m_i++;
    size_t nBytes = Utf8SequenceBytes (c);
    m_v = c & (0xFF >> (nBytes + 1));	// First byte contains bits after the header.
    if (nBytes)
	while (--nBytes)		// Each subsequent byte has 6 bits.
	    m_v = (m_v << 6) | (*m_i++ & 0x3F);
    return (*this);
}

/// Returns the distance in characters (as opposed to the distance in bytes).
template <typename Iterator, typename WChar>
typename utf8in_iterator<Iterator,WChar>::difference_type
utf8in_iterator<Iterator,WChar>::operator- (const utf8in_iterator<Iterator,WChar>& last) const
{
    difference_type dist = 0;
    Iterator first;
    while (first < last) {
	advance (first, Utf8SequenceBytes (*first));
	++ dist;
    }
    return (dist);
}

//----------------------------------------------------------------------

/// An iterator adaptor to character containers for writing UTF-8 encoded text.
template <typename Iterator, typename WChar = wchar_t>
class utf8out_iterator {
public:
    typedef typename iterator_traits<Iterator>::value_type	value_type;
    typedef typename iterator_traits<Iterator>::difference_type	difference_type;
    typedef typename iterator_traits<Iterator>::pointer		pointer;
    typedef typename iterator_traits<Iterator>::reference	reference;
public:
    explicit			utf8out_iterator (const Iterator& os) : m_i (os) {}
				utf8out_iterator (const utf8out_iterator& i) : m_i (i.m_i) {} 
    /// Writes \p v into the stream.
    utf8out_iterator&		operator= (WChar v);
    inline utf8out_iterator&	operator* (void) { return (*this); }
    inline utf8out_iterator&	operator++ (void) { return (*this); }
    inline utf8out_iterator	operator++ (int) { return (*this); }
    inline bool			operator== (const utf8out_iterator& i) const { return (m_i == i.m_i); }
    inline bool			operator< (const utf8out_iterator& i) const { return (m_i < i.m_i); }
private:
    Iterator			m_i;
};

/// Writes \p v into the stream.
template <typename Iterator, typename WChar>
utf8out_iterator<Iterator,WChar>& utf8out_iterator<Iterator,WChar>::operator= (WChar v)
{
    const size_t nBytes = Utf8Bytes (v);
    if (nBytes == 1) 	// If only one byte, there is no header.
    	*m_i++ = v;
    else {
	// Write the bits 6 bits at a time, except for the first one,
	// which may be less than 6 bits.
	register wchar_t shift = nBytes * 6;
	*m_i++ = ((v >> (shift -= 6)) & 0x3F) | (0xFF << (8 - nBytes));
	while (shift)
	    *m_i++ = ((v >> (shift -= 6)) & 0x3F) | 0x80;
    }
    return (*this);
}

//----------------------------------------------------------------------

/// Returns a UTF-8 adaptor writing to \p i. Useful in conjuction with back_insert_iterator.
template <typename Iterator>
inline utf8out_iterator<Iterator> utf8out (Iterator i)
{
    return (utf8out_iterator<Iterator> (i));
}

/// Returns a UTF-8 adaptor reading from \p i. First character is read on construction.
template <typename Iterator>
inline utf8in_iterator<Iterator> utf8in (Iterator i)
{
    return (utf8in_iterator<Iterator> (i));
}

/// Returns a UTF-8 adaptor reading from \p is. First character is read on construction.
inline utf8in_iterator<istream_iterator<u_char> > utf8in (istream& is)
{
    istream_iterator<u_char> si (is);
    return (utf8in_iterator<istream_iterator<u_char> > (si));
}

/// Returns a UTF-8 adaptor writing to \p os.
inline utf8out_iterator<ostream_iterator<u_char> > utf8out (ostream& os)
{
    ostream_iterator<u_char> si (os);
    return (utf8out_iterator<ostream_iterator<u_char> > (si));
}

//----------------------------------------------------------------------

} // namespace ustl

#endif

