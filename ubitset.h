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
// ubitset.h
//

#include "ustring.h"
#include "ufunction.h"

namespace ustl {

void convert_to_bitstring (const u_int* v, size_t n, string& buf);
void convert_from_bitstring (const string& buf, u_int* v, size_t n);

template <class T> struct bitwise_or	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a | b); } };
template <class T> struct bitwise_and	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a & b); } };
template <class T> struct bitwise_xor	: public binary_function<T,T,T> { inline T operator()(const T& a, const T& b) const { return (a ^ b); } };
template <class T> struct bitwise_not	: public unary_function<T,T>    { inline T operator()(const T& a) const { return (~a); } };

///
/// bitset is a fixed-size block of memory with addressable bits.
/// Normally used for state flags; allows setting and unsetting of individual
/// bits as well as bitwise operations on the entire set. The interface is
/// most like that of unsigned integers, and is intended to be used as such.
///
template <size_t Size>
class bitset {
public:
    typedef u_int		value_type;
    typedef value_type*		pointer;
    typedef const value_type*	const_pointer;
    typedef pointer		iterator;
    typedef const_pointer	const_iterator;
    typedef size_t		difference_type;
    typedef size_t		size_type;
private:
    static const size_t s_HostBits	= sizeof(value_type) * CHAR_BIT;
    static const size_t	s_nHosts	= Size / s_HostBits + ((Size % s_HostBits) != 0);
    static const size_t	s_nBits		= s_nHosts * s_HostBits;
private:
    inline value_type&		BitRef (size_t n)	{ assert (n < Size); return (m_Bits [n / s_HostBits]); }
    inline const value_type	BitRef (size_t n) const	{ assert (n < Size); return (m_Bits [n / s_HostBits]); }
    inline const value_type	Mask (size_t n) const	{ assert (n < Size); return (1 << (n % s_HostBits)); }
public:
    inline		bitset (value_type v = 0)	{ fill_n (m_Bits, s_nHosts, 0); m_Bits[0] = v; }
    inline		bitset (const string& buf)	{ convert_from_bitstring (buf, m_Bits, s_nHosts); }
    inline void		flip (size_t n)			{ BitRef(n) ^= Mask(n); }
    inline void		reset (void)			{ fill_n (m_Bits, s_nHosts, 0); }
    inline void		set (void)			{ fill_n (m_Bits, s_nHosts, -1); }
    inline bitset	operator~ (void) const		{ bitset rv (*this); rv.flip(); return (rv); }
    inline size_t	size (void) const		{ return (Size); }
    inline size_t	capacity (void) const		{ return (s_nBits); }
    inline const bool	test (size_t n) const		{ return (BitRef(n) & Mask(n)); }
    inline const bool	operator[] (size_t n) const	{ return (test(n)); }
  inline const_iterator	begin (void) const		{ return (m_Bits); }
    inline iterator	begin (void)			{ return (m_Bits); }
  inline const_iterator	end (void) const		{ return (m_Bits + s_nHosts); }
    inline iterator	end (void)			{ return (m_Bits + s_nHosts); }
 			/// Returns the u_long with the equivalent bits. If size() > sizeof(u_long), you'll get only the first BitsInType(value_type) bits.
    inline const u_long	to_ulong (void) const		{ return (m_Bits[0]); }
    			/// Flips all the bits in the set.
    inline void		flip (void) { transform (begin(), end(), begin(), bitwise_not<value_type>()); }
			/// Sets or clears bit \p n.
    inline void		set (size_t n, bool val = true)
			{
			    if (val)
				BitRef(n) |= Mask(n);
			    else
				BitRef(n) &= ~Mask(n);
			}
			/// Returns a string with bits MSB "001101001..." LSB.
    inline string	to_string (void) const
			{
			    string rv (Size, '0');
			    convert_to_bitstring (m_Bits, s_nHosts, rv);
			    return (rv);
			}
    inline bool		operator== (const bitset<Size>& v) const
			{
			    if (s_nHosts == 1)
				return (m_Bits[0] == v.m_Bits[0]);
			    else
				return (equal (begin(), end(), v.begin()));
			}
private:
    value_type		m_Bits [s_nHosts];
};

}; // namespace ustl

