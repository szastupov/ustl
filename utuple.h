// This file is part of the ustl library, an STL implementation.
// Copyright (c) 2003 by Mike Sharov <msharov@talentg.com>
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
// utuple.h
//

#ifndef UTUPLE_H_7324ADEC49B397CA74A56F6050FD5A6B
#define UTUPLE_H_7324ADEC49B397CA74A56F6050FD5A6B

#include "ualgo.h"

namespace ustl {

/// \class tuple utuple.h ustl.h
/// \ingroup Sequences
///
/// \brief A fixed-size array of \p N \p Ts.
///
template <size_t N, typename T>
class tuple {
public:
    typedef T						value_type;
    typedef size_t					size_type;
    typedef value_type*					pointer;
    typedef const value_type*				const_pointer;
    typedef value_type&					reference;
    typedef const value_type&				const_reference;
    typedef pointer					iterator;
    typedef const_pointer				const_iterator;
    typedef ::ustl::reverse_iterator<iterator>		reverse_iterator;
    typedef ::ustl::reverse_iterator<const_iterator>	const_reverse_iterator;
    typedef pair<iterator,iterator>			range_t;
    typedef pair<const_iterator,const_iterator>		const_range_t;
public:
    inline			tuple (void)			{ fill_n (m_v, N, T()); }
    template <typename T2>
    inline			tuple (const tuple<N,T2>& t)	{ for (uoff_t i = 0; i < N; ++ i) m_v[i] = T(t.at(i)); }
    inline			tuple (const tuple<N,T>& t)	{ copy_n (t.begin(), N, m_v); }
    inline			tuple (const_pointer v)		{ copy_n (v, N, m_v); }
    inline			tuple (const_reference v0);
    inline			tuple (const_reference v0, const_reference v1);
    inline			tuple (const_reference v0, const_reference v1, const_reference v2);
    inline			tuple (const_reference v0, const_reference v1, const_reference v2, const_reference v3);
    inline iterator		begin (void)			{ return (m_v); }
    inline const_iterator	begin (void) const		{ return (m_v); }
    inline iterator		end (void)			{ return (begin() + N); }
    inline const_iterator	end (void) const		{ return (begin() + N); }
    inline size_type		size (void) const		{ return (N); }
    inline size_type		max_size (void) const		{ return (N); }
    inline bool			empty (void) const		{ return (N == 0); }
    inline const_reference	at (size_type i) const		{ return (m_v[i]); }
    inline reference		at (size_type i)		{ return (m_v[i]); }
    inline const_reference	operator[] (size_type i) const	{ return (m_v[i]); }
    inline reference		operator[] (size_type i)	{ return (m_v[i]); }
    template <typename T2>
    inline const tuple&		operator= (const tuple<N,T2>& src)
				    { for (uoff_t i = 0; i < N; ++ i) m_v[i] = T(src.at(i)); return (*this); }
    inline const tuple&		operator= (const tuple<N,T>& src)
				    { copy_n (src.begin(), N, begin()); return (*this); }
    inline const tuple&		operator+= (const_reference v)
				    { for (uoff_t i = 0; i < N; ++ i) m_v[i] += v; return (*this); }
    inline const tuple&		operator-= (const_reference v)
				    { for (uoff_t i = 0; i < N; ++ i) m_v[i] -= v; return (*this); }
    inline const tuple&		operator*= (const_reference v)
				    { for (uoff_t i = 0; i < N; ++ i) m_v[i] *= v; return (*this); }
    inline const tuple&		operator/= (const_reference v)
				    { for (uoff_t i = 0; i < N; ++ i) m_v[i] /= v; return (*this); }
    inline const tuple		operator+ (const_reference v) const
				    { tuple result; for (uoff_t i = 0; i < N; ++ i) result[i] = m_v[i] + v; return (result); }
    inline const tuple		operator- (const_reference v) const
				    { tuple result; for (uoff_t i = 0; i < N; ++ i) result[i] = m_v[i] - v; return (result); }
    inline const tuple		operator* (const_reference v) const
				    { tuple result; for (uoff_t i = 0; i < N; ++ i) result[i] = m_v[i] * v; return (result); }
    inline const tuple		operator/ (const_reference v) const
				    { tuple result; for (uoff_t i = 0; i < N; ++ i) result[i] = m_v[i] / v; return (result); }
    inline void			swap (tuple<N,T>& v)
				    { for (uoff_t i = 0; i < N; ++ i) ::ustl::swap (m_v[i], v.m_v[i]); }
private:
    T				m_v [N];
};

template <size_t N, typename T>
inline tuple<N,T>::tuple (const_reference v0)
{ m_v[0] = v0; }

template <size_t N, typename T>
inline tuple<N,T>::tuple (const_reference v0, const_reference v1)
{ m_v[0] = v0; m_v[1] = v1; }

template <size_t N, typename T>
inline tuple<N,T>::tuple (const_reference v0, const_reference v1, const_reference v2)
{ m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; }

template <size_t N, typename T>
inline tuple<N,T>::tuple (const_reference v0, const_reference v1, const_reference v2, const_reference v3)
{ m_v[0] = v0; m_v[1] = v1; m_v[2] = v2; m_v[3] = v3; }

template <size_t N, typename T1, typename T2>
inline bool operator== (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    for (uoff_t i = 0; i < N; ++ i)
	if (t1[i] != t2[i])
	    return (false);
    return (true);
}

template <size_t N, typename T1, typename T2>
inline bool operator< (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    for (uoff_t i = 0; i < N && t1[i] <= t2[i]; ++ i)
	if (t1[i] < t2[i])
	    return (true);
    return (false);
}

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1>& operator+= (tuple<N,T1>& t1, const tuple<N,T2>& t2)
    { for (uoff_t i = 0; i < N; ++ i) t1[i] = T1(t1[i] + t2[i]); return (t1); }

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1>& operator-= (tuple<N,T1>& t1, const tuple<N,T2>& t2)
    { for (uoff_t i = 0; i < N; ++ i) t1[i] = T1(t1[i] - t2[i]); return (t1); }

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1>& operator*= (tuple<N,T1>& t1, const tuple<N,T2>& t2)
    { for (uoff_t i = 0; i < N; ++ i) t1[i] = T1(t1[i] * t2[i]); return (t1); }

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1>& operator/= (tuple<N,T1>& t1, const tuple<N,T2>& t2)
    { for (uoff_t i = 0; i < N; ++ i) t1[i] = T1(t1[i] / t2[i]); return (t1); }

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1> operator+ (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    tuple<N,T1> result;
    for (uoff_t i = 0; i < N; ++ i) result[i] = T1(t1[i] + t2[i]);
    return (result);
}

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1> operator- (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    tuple<N,T1> result;
    for (uoff_t i = 0; i < N; ++ i) result[i] = T1(t1[i] - t2[i]);
    return (result);
}

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1> operator* (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    tuple<N,T1> result;
    for (uoff_t i = 0; i < N; ++ i) result[i] = T1(t1[i] * t2[i]);
    return (result);
}

template <size_t N, typename T1, typename T2>
inline const tuple<N,T1> operator/ (const tuple<N,T1>& t1, const tuple<N,T2>& t2)
{
    tuple<N,T1> result;
    for (uoff_t i = 0; i < N; ++ i) result[i] = T1(t1[i] / t2[i]);
    return (result);
}

} // namespace ustl

#endif

