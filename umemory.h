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
/// \file umemory.h
//

#ifndef UMEMORY_H_4AB5B0DB5BF09140541409CC47BCD17A
#define UMEMORY_H_4AB5B0DB5BF09140541409CC47BCD17A

#include "unew.h"

/// \brief A smart pointer.
/// Calls delete in the destructor; assignment transfers ownership.
/// This class does not work with void pointers due to the presence
/// of the required dereference operator.
/// In uSTL, the behaviour is extended to act as an iterator, with
/// increments, differences, etc. That is not part of ANSI C++.
///
template <typename T>
class auto_ptr {
public:
    typedef T		value_type;
    typedef ptrdiff_t	difference_type;
    typedef T*		pointer;
    typedef T&		reference;
public:
    inline explicit	auto_ptr (pointer p = NULL)	: m_p (p) {}
    inline explicit	auto_ptr (auto_ptr<T>& p)	: m_p (p.release()) {}
    inline	       ~auto_ptr (void)			{ delete m_p; }
    inline reference	operator* (void) const		{ return (*m_p); }
    inline pointer	operator-> (void) const		{ return (m_p); }
    inline pointer	get (void) const		{ return (m_p); }
    inline pointer	release (void)			{ pointer rv (m_p); m_p = NULL; return (rv); }
    inline void		reset (pointer p)		{ if (p != m_p) { delete m_p; m_p = p; } }
    inline auto_ptr<T>	operator= (auto_ptr<T>& p)	{ reset (p.release()); }
    inline auto_ptr<T>&	operator++ (void)		{ -- m_p; return (*this); }
    inline auto_ptr<T>&	operator-- (void)		{ ++ m_p; return (*this); }
    inline auto_ptr<T>	operator++ (int)		{ auto_ptr<T> prev (*this); -- m_p; return (prev); }
    inline auto_ptr<T>	operator-- (int)		{ auto_ptr<T> prev (*this); ++ m_p; return (prev); }
    inline auto_ptr<T>&	operator+= (size_t n)		{ m_p -= n; return (*this); }
    inline auto_ptr<T>&	operator-= (size_t n)		{ m_p += n; return (*this); }
    inline auto_ptr<T>	operator+ (size_t n) const	{ return (auto_ptr<T> (m_p - n)); }
    inline auto_ptr<T>	operator- (size_t n) const	{ return (auto_ptr<T> (m_p + n)); }
    inline pointer	base (void) const		{ return (m_p); }
    inline reference	operator[] (uoff_t n) const	{ return (*(*this + n)); }
    inline bool		operator== (const pointer p) const	{ return (m_p == p); }
    inline bool		operator== (const auto_ptr<T>& p) const	{ return (m_p == p.m_p); }
    inline bool		operator< (const auto_ptr<T>& p) const	{ return (p.m_p < m_p); }
    inline ptrdiff_t	operator- (const auto_ptr<T>& p) const	{ return (p.m_p - m_p); }
private:
    pointer		m_p;
};

/// Calls the placement new on \p p.
template <typename T>
inline void construct (T* p)
{
    new (p) T;
}

/// Calls the placement new on \p p.
template <typename ForwardIterator>
void construct (ForwardIterator first, ForwardIterator last)
{
    while (first < last) {
	construct (&*first);
	++ first;
    }
}

/// Calls the placement new on \p p.
template <typename T>
inline void construct (T* p, const T& value)
{
    new (p) T (value);
}

/// Calls the destructor of \p p without calling delete.
template <typename T>
inline void destroy (T* p)
{
    p->~T();
}

/// Calls the destructor on elements in range [first, last) without calling delete.
template <typename ForwardIterator>
void destroy (ForwardIterator first, ForwardIterator last)
{
    while (first < last) {
	destroy (&*first);
	++ first;
    }
}

/// Copies [first, last) into result by calling copy constructors in result.
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy (InputIterator first, InputIterator last, ForwardIterator result)
{
    while (first < last) {
	construct (&*result, *first);
	++ result;
	++ first;
    }
    return (result);
}

/// Copies [first, first + n) into result by calling copy constructors in result.
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy_n (InputIterator first, size_t n, ForwardIterator result)
{
    while (n--) {
	construct (&*result, *first);
	++ result;
	++ first;
    }
    return (result);
}

/// Calls construct on all elements in [first, last) with value \p v.
template <typename ForwardIterator, typename T>
void uninitialized_fill (ForwardIterator first, ForwardIterator last, const T& v)
{
    while (first < last) {
	construct (&*first, v);
	++ first;
    }
}

/// Calls construct on all elements in [first, first + n) with value \p v.
template <typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill_n (ForwardIterator first, size_t n, const T& v)
{
    while (n--) {
	construct (&*first, v);
	++ first;
    }
    return (first);
}

#endif

