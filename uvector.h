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

#ifndef UVECTOR_H_00BB13AF082BEB7829C031B265518169
#define UVECTOR_H_00BB13AF082BEB7829C031B265518169

#include "memblock.h"
#include "uiterator.h"
#include "umemory.h"

namespace ustl {

/// \class vector uvector.h ustl.h
/// \ingroup Sequences
///
/// \brief STL vector equivalent.
///
/// In this design elements frequently undergo
/// BITWISE COPY! Don't put it in here if it doesn't support it.
///
template <typename T>
class vector : public memblock {
public:
    typedef T				value_type;
    typedef value_type*			pointer;
    typedef const value_type*		const_pointer;
    typedef value_type&			reference;
    typedef const value_type&		const_reference;
    typedef pointer			iterator;
    typedef const_pointer		const_iterator;
    typedef ::ustl::reverse_iterator<iterator>	reverse_iterator;
    typedef ::ustl::reverse_iterator<const_iterator>	const_reverse_iterator;
public:
    inline			vector (void);
    inline explicit		vector (size_t n);
				vector (size_t n, const T& v);
				vector (const vector<T>& v);
				vector (const_iterator i1, const_iterator i2);
    inline virtual	       ~vector (void);
    inline const vector<T>&	operator= (const vector<T>& v);
    inline void			reserve (size_t n);
    inline void			resize (size_t n);
    inline size_t		capacity (void) const;
    inline size_t		size (void) const;
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
    inline virtual size_t	elementSize (void) const;
    inline size_t		elementBytes (size_t n) const { return (n * sizeof(T)); }
};

/// Initializes empty vector.
template <typename T>
inline vector<T>::vector (void)
: memblock()
{
}

/// Initializes a vector of size \p n.
template <typename T>
inline vector<T>::vector (size_t n)
: memblock()
{
    resize (n);
}

/// Copies \p n elements from \p v.
template <typename T>
vector<T>::vector (size_t n, const T& v)
: memblock()
{
    resize (n);
    ::ustl::fill (begin(), end(), v);
}

/// Copies \p v.
template <typename T>
vector<T>::vector (const vector<T>& v)
: memblock()
{
    resize (v.size());
    ::ustl::copy (v.begin(), v.end(), begin());
}

/// Copies range [\p i1, \p i2]
template <typename T>
vector<T>::vector (const_iterator i1, const_iterator i2)
: memblock()
{
    resize (distance (i1, i2));
    ::ustl::copy (i1, i2, begin());
}

/// Destructor
/// Here the class must call deallocate, which is overloaded in this
/// template to call destructors on all the elements. ~memblock does this too,
/// but by the time the code gets there, the destructBlock overload is gone.
///
template <typename T>
inline vector<T>::~vector (void)
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
    memblock::reserve (elementBytes(n));
}

/// Resizes the vector to contain \p n elements.
template <typename T>
inline void vector<T>::resize (size_t n)
{
    memblock::resize (elementBytes(n));
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

/// Returns the pointer to the first element.
template <typename T>
inline typename vector<T>::iterator vector<T>::begin (void)
{
    return (iterator (memblock::begin()));
}

/// Returns the pointer to the first element.
template <typename T>
inline typename vector<T>::const_iterator vector<T>::begin (void) const
{
    return (const_iterator (memblock::begin()));
}

/// Returns the pointer to the last element.
template <typename T>
inline typename vector<T>::iterator vector<T>::end (void)
{
    return (iterator (memblock::end()));
}

/// Returns the pointer to the last element.
template <typename T>
inline typename vector<T>::const_iterator vector<T>::end (void) const
{
    return (const_iterator (memblock::end()));
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
inline typename vector<T>::reference vector<T>::at (uoff_t i1)
{
    assert (i1 < size());
    return (*(begin() + i1));
}

/// Returns the const reference to the i'th element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::at (uoff_t i2) const
{
    assert (i2 < size());
    return (*(begin() + i2));
}

/// Returns the reference to the i'th element.
template <typename T>
inline typename vector<T>::reference vector<T>::operator[] (uoff_t i3)
{
    return (at(i3));
}

/// Returns the const reference to the i'th element.
template <typename T>
inline typename vector<T>::const_reference vector<T>::operator[] (uoff_t i4) const
{
    return (at(i4));
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
    ip = iterator (memblock::insert (memblock::iterator(ip), elementBytes(n)));
    ::ustl::fill (ip, ip + n, v);
    return (ip);
}

/// Inserts value \p v at offset \p ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (iterator ip, const T& v)
{
    ip = iterator (memblock::insert (memblock::iterator(ip), sizeof(T)));
    *ip = v;
    return (ip);
}

/// Inserts range [\p i1, \p i2] at offset \p ip.
template <typename T>
inline typename vector<T>::iterator vector<T>::insert (iterator ip, const_iterator i1, const_iterator i2)
{
    assert (i1 <= i2);
    ip = iterator (memblock::insert (memblock::iterator(ip), elementBytes(distance(i1, i2))));
    ::ustl::copy (i1, i2, ip);
    return (ip);
}

/// Removes \p count elements at offset \p ep.
template <typename T>
inline typename vector<T>::iterator vector<T>::erase (iterator ep, size_t n)
{
    return (iterator (memblock::erase (memblock::iterator(ep), elementBytes(n))));
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

/// \brief Calls T() for every element.
/// Because storage is allocated by malloc() in memblock::Reserve(),
/// the constructors must be explicitly called here.
///
template <typename T>
void vector<T>::constructBlock (void* p, size_t s) const
{
    memblock::constructBlock (p, s);
    assert (s % sizeof(T) == 0);
    T* pt = reinterpret_cast<T*>(p);
    construct (pt, pt + s / sizeof(T));
}

/// \brief Calls ~T() for every element.
/// Because storage is deallocated by free() in memblock::Link(),
/// the destructors must be explicitly called here. This function must
/// also be called from the destructor of this class because it is virtual
/// and cannot be called from ~memblock().
///
template <typename T>
void vector<T>::destructBlock (void* p, size_t s) const
{
    assert (s % sizeof(T) == 0);
    T* pt = reinterpret_cast<T*>(p);
    destroy (pt, pt + s / sizeof(T));
    memblock::destructBlock (p, s);
}

/// Returns the size of each element in the container.
template <typename T>
inline size_t vector<T>::elementSize (void) const
{
    return (sizeof(T));
}

}; // namespace ustl

#endif

