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
// uvector.h
//
/** \class ustl::vector<T>
 *
 * \brief STL vector equivalent.
 *
 * In this design elements frequently undergo
 * BITWISE COPY! Don't put it in here if it doesn't support it. pvector
 * template should be used for objects with non-trivial copy methods.
*/

#ifndef UVECTOR_H
#define UVECTOR_H

#include "mistream.h"
#include "mostream.h"
#include "memblock.h"
#include "ualgo.h"
#include "uutility.h"
#include "uiterator.h"
#include "strmsize.h"
#include "unew.h"

namespace ustl {

template <typename T>
class vector : public memblock {
public:
    typedef T		value_type;
    typedef T*		pointer;
    typedef const T*	const_pointer;
    typedef T&		reference;
    typedef const T&	const_reference;
    typedef T*		iterator;
    typedef const T*	const_iterator;
    typedef ::ustl::reverse_iterator<iterator>	reverse_iterator;
    typedef ::ustl::reverse_iterator<const_iterator>	const_reverse_iterator;
    static const size_t c_DefaultPageSize = sizeof(T) * 16;	///< Minimum allocation unit.
public:
				vector (void);
    explicit			vector (size_t n);
				vector (size_t n, const T& v);
				vector (const vector<T>& v);
				vector (const_iterator i1, const_iterator i2);
    virtual		       ~vector (void);
    inline const vector<T>&	operator= (const vector<T>& v);
    inline void			reserve (size_t n);
    inline void			resize (size_t n);
    inline size_t		capacity (void) const;
    inline size_t		size (void) const;
    inline size_t		max_size (void) const;
    inline size_t		stream_size (void) const;
    inline iterator		begin (void);
    inline const_iterator	begin (void) const;
    inline iterator		end (void);
    inline const_iterator	end (void) const;
    inline reverse_iterator		rbegin (void);
    inline const_reverse_iterator	rbegin (void) const;
    inline reverse_iterator		rend (void);
    inline const_reverse_iterator	rend (void) const;
    inline reference		at (size_t i);
    inline const_reference	at (size_t i) const;
    inline reference		operator[] (size_t i);
    inline const_reference	operator[] (size_t i) const;
    inline reference		front (void);
    inline const_reference	front (void) const;
    inline reference		back (void);
    inline const_reference	back (void) const;
    inline void			push_back (const T& v = T());
    inline void			pop_back (void);
    inline void			assign (const_iterator i1, const_iterator i2);
    inline void			assign (size_t n, const T& v);
    inline iterator		insert (iterator ip, const T& v = T());
    inline iterator		insert (iterator ip, size_t n, const T& v);
    inline iterator		insert (iterator ip, const_iterator i1, const_iterator i2);
    inline iterator		erase (iterator ep, size_t n = 1);
    inline iterator		erase (iterator ep1, iterator ep2);
protected:
    virtual void		constructBlock (void* p, size_t s) const;
    virtual void		destructBlock (void* p, size_t s) const;
    virtual void		read (istream& is);
    virtual void		write (ostream& os) const;
};

/// Initializes empty vector.
template <typename T>
vector<T>::vector (void)
: memblock ()
{
    setPageSize (c_DefaultPageSize);
}

/// Initializes a vector of size \p n.
template <typename T>
vector<T>::vector (size_t n)
: memblock ()
{
    setPageSize (c_DefaultPageSize);
    resize (n);
}

/// Copies \p n elements from \p v.
template <typename T>
vector<T>::vector (size_t n, const T& v)
: memblock ()
{
    setPageSize (c_DefaultPageSize);
    resize (n);
    ::ustl::fill (begin(), end(), v);
}

/// Copies \p v.
template <typename T>
vector<T>::vector (const vector<T>& v)
: memblock ()
{
    setPageSize (v.pageSize());
    resize (v.size());
    ::ustl::copy (v.begin(), v.end(), begin());
}

/// Copies range [\p i1, \p i2]
template <typename T>
vector<T>::vector (const_iterator i1, const_iterator i2)
: memblock ()
{
    assert (i1 <= i2);
    setPageSize (c_DefaultPageSize);
    resize (i2 - i1);
    ::ustl::copy (i1, i2, begin());
}

/// Destructor
/** Here the class must call deallocate, which is overloaded in this
 * template to call destructors on all the elements. ~memblock does this too,
 * but by the time the code gets there, the destructBlock overload is gone.
 */
template <typename T>
vector<T>::~vector (void)
{
    deallocate();
}

/// Copies contents of \p v.
template <typename T>
inline const vector<T>& vector<T>::operator= (const vector<T>& v)
{
    assign (v.begin(), v.end());
    return (*this);
}

/// Allocates space for at least \p n elements.
template <typename T>
inline void vector<T>::reserve (size_t n)
{
    memblock::reserve (n * sizeof(T));
}

/// Resizes the vector to contain \p n elements.
template <typename T>
inline void vector<T>::resize (size_t n)
{
    memblock::resize (n * sizeof(T));
}

/// Returns the number of elements for which space has been allocated.
template <typename T>
inline size_t vector<T>::capacity (void) const
{
    return (memblock::capacity() / sizeof(T));
}

/// Returns number of elements in the vector.
template <typename T>
inline size_t vector<T>::size (void) const
{
    return (memblock::size() / sizeof(T));
}

/// Returns the maximum number of elements that can ever be put in this container.
template <typename T>
inline size_t vector<T>::max_size (void) const
{
    return (memblock::max_size() / sizeof(T));
}

/// Returns the number of bytes necessary to write this object to a stream
template <typename T>
size_t vector<T>::stream_size (void) const
{
    size_t s = sizeof(size_t);
    const_iterator first = begin();
    while (first < end())
	s += stream_size_of(*first++);
    s = Align (s);
    return (s);
}

/// Returns the pointer to the first element.
template <typename T>
inline typename vector<T>::iterator vector<T>::begin (void)
{
    return (reinterpret_cast<iterator>(memblock::begin().base()));
}

/// Returns the pointer to the first element.
template <typename T>
inline typename vector<T>::const_iterator vector<T>::begin (void) const
{
    return (reinterpret_cast<const_iterator>(memblock::begin().base()));
}

/// Returns the pointer to the last element.
template <typename T>
inline typename vector<T>::iterator vector<T>::end (void)
{
    return (reinterpret_cast<iterator>(memblock::end().base()));
}

/// Returns the pointer to the last element.
template <typename T>
inline typename vector<T>::const_iterator vector<T>::end (void) const
{
    return (reinterpret_cast<const_iterator>(memblock::end().base()));
}

/// Returns the reverse iterator to the last element.
template <typename T>
inline typename vector<T>::reverse_iterator vector<T>::rbegin (void)
{
    return (reverse_iterator (end()));
}

/// Returns the reverse iterator to the last element.
template <typename T>
inline typename vector<T>::const_reverse_iterator vector<T>::rbegin (void) const
{
    return (const_reverse_iterator (end()));
}

/// Returns the reverse iterator to the first element.
template <typename T>
inline typename vector<T>::reverse_iterator vector<T>::rend (void)
{
    return (reverse_iterator (begin()));
}

/// Returns the reverse iterator to the first element.
template <typename T>
inline typename vector<T>::const_reverse_iterator vector<T>::rend (void) const
{
    return (const_reverse_iterator (begin()));
}

/// Returns the reference to the i'th element.
template <typename T>
inline typename vector<T>::reference vector<T>::at (uoff_t i)
{
    assert (i < size());
    return (*(begin() + i));
}

/// Returns the const reference to the i'th element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::at (uoff_t i) const
{
    assert (i < size());
    return (*(begin() + i));
}

/// Returns the reference to the i'th element.
template <typename T>
inline typename vector<T>::reference vector<T>::operator[] (uoff_t i)
{
    return (at(i));
}

/// Returns the const reference to the i'th element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::operator[] (uoff_t i) const
{
    return (at(i));
}

/// Returns the reference to the first element.
template <typename T>
inline typename vector<T>::reference vector<T>::front (void)
{
    assert (size() > 0);
    return (*begin());
}

/// Returns the const reference to the first element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::front (void) const
{
    assert (size() > 0);
    return (*begin());
}

/// Returns the reference to the last element.
template <typename T>
inline typename vector<T>::reference vector<T>::back (void)
{
    assert (size() > 0);
    return (*(end() - 1));
}

/// Returns the const reference to the last element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::back (void) const
{
    assert (size() > 0);
    return (*(end() - 1));
}

/// Copies the range [\p i1, \p i2]
template <typename T>
inline void vector<T>::assign (const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    resize (i2 - i1);
    ::ustl::copy (i1, i2, begin());
}

/// Copies \p n elements with value \p v.
template <typename T>
inline void vector<T>::assign (size_t n, const T& v)
{
    resize (n);
    ::ustl::fill (begin(), end(), v);
}

/// Inserts \p n elements with value \p v at offsets \p ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (iterator ip, size_t n, const T& v)
{
    ip = reinterpret_cast<iterator>(memblock::insert (ip, n * sizeof(T)).base());
    ::ustl::fill (ip, ip + n, v);
    return (ip);
}

/// Inserts value \p v at offset \p ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (iterator ip, const T& v)
{
    ip = reinterpret_cast<iterator>(memblock::insert (ip, sizeof(T)).base());
    *ip = v;
    return (ip);
}

/// Inserts range [\p i1, \p i2] at offset \p ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (iterator ip, const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    ip = reinterpret_cast<iterator>(memblock::insert (ip, distance(i1, i2) * sizeof(T)).base());
    ::ustl::copy (i1, i2, ip);
    return (ip);
}

/// Removes \p count elements at offset \p ep.
template <typename T>
inline typename vector<T>::iterator vector<T>::erase (iterator ep, size_t n)
{
    return (reinterpret_cast<iterator>(memblock::erase (ep, n * sizeof(T)).base()));
}

/// Removes elements from \p ep1 to \p ep2.
template <typename T>
inline typename vector<T>::iterator vector<T>::erase (iterator ep1, iterator ep2)
{
    assert (ep1 <= ep2);
    return (erase (ep1, distance(ep1, ep2)));
}

/// Inserts value \p v at the end of the vector.
template <typename T>
inline void vector<T>::push_back (const T& v)
{
    insert (end(), v);
}

/// Removes one element from the back of the vector.
template <typename T>
inline void vector<T>::pop_back (void)
{
    erase (end() - 1);
}

/// Calls T() for every element.
/** Because storage is allocated by malloc() in memblock::Reserve(),
 * the constructors must be explicitly called here.
*/
template <typename T>
void vector<T>::constructBlock (void* p, size_t s) const
{
    memblock::constructBlock (p, s);
    assert (s % sizeof(T) == 0);    // vector's constructor ensures this
    new (p) T [s / sizeof(T)];
}

/// Calls ~T() for every element.
/** Because storage is deallocated by free() in memblock::Link(),
 * the destructors must be explicitly called here. This function must
 * also be called from the destructor of this class because it is virtual
 * and cannot be called from ~memblock().
*/
template <typename T>
void vector<T>::destructBlock (void* p, size_t s) const
{
    assert (s % sizeof(T) == 0);    // vector's constructor ensures this
    size_t nObjects = s / sizeof(T);
    T* pt = reinterpret_cast<T*>(p);
    while (nObjects--)
	(*pt++).~T();
    memblock::destructBlock (p, s);
}

template <typename T>
inline void vector<T>::read (istream& is)
{
    size_t n;
    is >> n;
    assert (n * stream_size_of(T()) <= is.remaining());
    resize (n);
    copy_n (istream_iterator<T>(is), size(), begin());
    is.align();
}

template <typename T>
inline void vector<T>::write (ostream& os) const
{
    os << size();
    copy_n (begin(), size(), ostream_iterator<T>(os));
    os.align();
}

} // namespace ustl

#endif

