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
// ustring.h
//

#ifndef USTRING_H
#define USTRING_H

#include "memblock.h"
#include "uiterator.h"
#include "ulimits.h"
#include "strmsize.h"

namespace ustl {

///
/// \brief STL basic_string equivalent.
///
/// An STL container for string manipulation.
/// Differences from C++ standard:
/// 	format member function - strstreams on the string are possible, but
/// 		are in many cases inconvenient because they hardcode not only
/// 		the locations of substitutions, but also the constant text in
/// 		between. Such behaviour makes string localization all but
/// 		impossible. Another constant gripe is the requirement for
/// 		space precomputation for strings. string::format will
/// 		reallocate the string, if needed, to hold the entire output.
/// 		(That's only for glibc > 2.0.6. See code for comments)
/// 		Also, take care not to pass in untrusted format strings lest
/// 		they contain a %n, which causes a nasty security hole.
/// 	const char* cast operator. It is much clearer to use this than having
/// 		to type .c_str() every time. A side effect of this is that
/// 		const operator[] is no longer needed (gcc will warn about an
/// 		ambiguous overload)
/// 	copyto avoids having to include <string.h> and knowledge of the string
/// 		size.
/// 	length() currently returns the same number as size(), but eventually
/// 		size() will return the allocated size including the terminator.
///
class string : public memblock {
public:
    typedef char		value_type;
    typedef value_type*		pointer;
    typedef const value_type*	const_pointer;
    typedef pointer		iterator;
    typedef const_pointer	const_iterator;
    typedef value_type&		reference;
    typedef value_type		const_reference;
    typedef ::ustl::reverse_iterator<iterator>		reverse_iterator;
    typedef ::ustl::reverse_iterator<const_iterator>	const_reverse_iterator;
public:
    static const uoff_t npos = static_cast<uoff_t>(-1);		///< Value that means the end of string.
    static const size_t size_Terminator = sizeof(value_type);	///< Most systems terminate strings with '\\0'
    static const value_type c_Terminator = 0;			///< String terminator
    static const char empty_string [size_Terminator];		///< An empty string.
public:
    inline			string (void);
				string (const string& s);
    inline explicit		string (const cmemlink& l);
				string (const_pointer s);
				string (const_pointer s, size_t len);
				string (const_pointer s1, const_pointer s2);
    inline size_t		size (void) const;
    inline size_t		length (void) const;
    inline pointer		data (void);
    inline const_pointer	c_str (void) const;
    inline size_t		max_size (void) const;
    inline bool			empty (void) const;
    inline size_t		capacity (void) const;
    inline void			reserve (size_t n);
    void			resize (size_t n);
    inline const_iterator	begin (void) const;
    inline iterator		begin (void);
    inline const_iterator	end (void) const;
    inline iterator		end (void);
    inline const_reverse_iterator	rbegin (void) const;
    inline reverse_iterator		rbegin (void);
    inline const_reverse_iterator	rend (void) const;
    inline reverse_iterator		rend (void);
    inline const_reference	at (uoff_t pos) const;
    inline reference		at (uoff_t pos);
    inline			operator const value_type* (void) const;
    inline reference		operator[] (uoff_t i);
    inline const string&	operator= (const string& s);
    inline const string&	operator= (const_reference c);
    inline const string&	operator= (const_pointer s);
    inline const string&	operator+= (const string& s);
    inline const string&	operator+= (const_reference c);
    inline const string&	operator+= (const_pointer s);
    inline string		operator+ (const string& s) const;
    bool			operator== (const string& s) const;
    bool			operator== (const_pointer s) const;
    inline bool			operator== (const_reference c) const;
    inline bool			operator!= (const_pointer s) const;
    inline bool			operator< (const string& s) const;
    inline bool			operator< (const_pointer s) const;
    inline bool			operator< (const_reference c) const;
    inline bool			operator> (const_pointer s) const;
    inline void			assign (const_iterator i1, const_iterator i2);
    void	    		assign (const_pointer s, size_t len);
    void	    		assign (const_pointer s);
    inline void			append (const_iterator i1, const_iterator i2);
    void	   		append (const_pointer s, size_t len);
    void	   		append (const_pointer s);
    void			append (size_t n, const_reference c);
    size_t			copyto (pointer p, size_t n, const_iterator start) const;
    inline void			clear (void);
    inline int			compare (const string& s) const;
    inline int			compare (const_pointer s) const;
    int				compare (const_iterator first1, const_iterator last1, const_iterator first2, const_iterator last2) const;
    void			insert (iterator start, const_reference c, size_t n = 1);
    void			insert (iterator start, const_pointer s, size_t n = 1);
    void			insert (iterator start, const_pointer first, const_iterator last, size_t n = 1);
    inline void			push_back (const_reference c);
    inline void			replace (iterator first, iterator last, const_reference c, size_t n = 1);
    void			replace (iterator first, iterator last, const_pointer s, size_t n = 1);
    void			replace (iterator first, iterator last, const_pointer i1, const_pointer i2, size_t n = 1);
    const_iterator		find (const_reference c, const_iterator pos = NULL) const;
    const_iterator		find (const string& s, const_iterator pos = NULL) const;
    const_iterator		rfind (const_reference c, const_iterator pos = NULL) const;
    const_iterator		rfind (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_first_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_first_not_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_last_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_last_not_of (const string& s, const_iterator pos = NULL) const;
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
    void			read (istream&);
    void			write (ostream& os) const;
    size_t			stream_size (void) const;
};

/// Creates an empty string.
inline string::string (void)
: memblock ()
{
    link (empty_string, sizeof(empty_string));
}

/// Assigns itself the value of string \p s
inline string::string (const cmemlink& s)
: memblock (s)
{
}

/// Returns the number of characters in the string, not including the terminator.
inline size_t string::size (void) const
{
    return (memblock::size() - size_Terminator);
}

/// Returns the number of characters in the string, not including the terminator.
inline size_t string::length (void) const
{
    return (size());
}

/// Returns the pointer to the string
inline string::pointer string::data (void)
{
    return (reinterpret_cast<string::pointer>(memblock::data()));
}

/// Returns the pointer to the string
inline string::const_pointer string::c_str (void) const
{
    return (reinterpret_cast<string::const_pointer>(memblock::cdata()));
}

/// Returns the maximum size of the string. On most systems it is max size_t.
inline size_t string::max_size (void) const
{
    return (memblock::max_size() / sizeof(value_type) - size_Terminator);
}

/// Returns \c true for strings with zero characters.
inline bool string::empty (void) const
{
    return (!size());
}

/// \brief Returns the number of bytes allocated for string data.
//
/// The number of bytes allocated may be greater than the number
/// used (and returned by size()), depending on the memory allocation
/// scheme.
///
inline size_t string::capacity (void) const
{
    const size_t n = memblock::capacity() / sizeof(value_type);
    return (n ? n - size_Terminator : 0);
}

/// Allocate enough storage to hold \p n characters.
inline void string::reserve (size_t n)
{
    memblock::reserve (n + size_Terminator);
}

/// Returns the pointer to the first character.
inline string::const_iterator string::begin (void) const
{
    return (reinterpret_cast<const_iterator>(memblock::begin().base()));
}

/// Returns the pointer to the first character.
inline string::iterator string::begin (void)
{
    return (reinterpret_cast<iterator>(memblock::begin().base()));
}

/// Returns the const reference to the \p pos character.
inline string::const_reference string::at (uoff_t pos) const
{
    assert (pos < size());
    assert (begin());
    return (*(begin() + pos));
}

/// Returns the reference to the \p pos character.
inline string::reference string::at (uoff_t pos)
{
    assert (pos <= size());
    assert (begin());
    return (*(begin() + pos));
}

/// Returns the pointer to the terminator.
inline string::const_iterator string::end (void) const
{
    return (begin() + size());
}

/// Returns the pointer to the terminator.
inline string::iterator string::end (void)
{
    return (begin() + size());
}

/// Returns the reverse iterator to the end of the string
inline string::const_reverse_iterator string::rbegin (void) const
{
    return (const_reverse_iterator (end()));
}

/// Returns the reverse iterator to the end of the string
inline string::reverse_iterator string::rbegin (void)
{
    return (reverse_iterator (end()));
}

/// Returns the reverse iterator to the beginning of the string
inline string::const_reverse_iterator string::rend (void) const
{
    return (const_reverse_iterator (begin()));
}

/// Returns the reverse iterator to the beginning of the string
inline string::reverse_iterator string::rend (void)
{
    return (reverse_iterator (begin()));
}

/// Returns the pointer to the first character.
inline string::operator const string::value_type* (void) const
{
    return (begin());
}

/// Returns the reference to the \p pos character.
inline string::reference string::operator[] (uoff_t pos)
{
    return (at(pos));
}

/// Assigns itself the value of string \p s
inline const string& string::operator= (const string& s)
{
    memblock::operator= (s);
    return (*this);
}

/// Assigns itself the value of 0-terminated string \p s
inline const string& string::operator= (const_pointer s)
{
    assign (s);
    return (*this);
}

/// Assigns itself the value of character \p c
inline const string& string::operator= (const_reference c)
{
    assign (&c, 1);
    return (*this);
}

/// Appends string \p s
inline const string& string::operator+= (const string& s)
{
    append (s.begin(), s.size());
    return (*this);
}

/// Appends character \p c
inline const string& string::operator+= (const_reference c)
{
    append (&c, 1);
    return (*this);
}

/// Appends itself the value of 0-terminated string \p s
inline const string& string::operator+= (const_pointer s)
{
    append (s);
    return (*this);
}

/// Concatenates itself with \p s
inline string string::operator+ (const string& s) const
{
    string result (*this);
    result += s;
    return (result);
}

/// Assigns itself the value of the range [\p i1, \p i2]
inline void string::assign (const_iterator i1, const_iterator i2)
{
    assign (i1, distance (i1, i2));
}

/// Appends to itself the value of the range [\p i1, \p i2]
inline void string::append (const_iterator i1, const_iterator i2)
{
    append (i1, distance (i1, i2));
}

/// \brief Returns comparison value regarding string \p s.
/// \see compare(start,len,s,sstart,slen)
inline int string::compare (const string& s) const
{
    return (compare (begin(), end(), s.begin(), s.end()));
}

/// \brief Returns comparison value regarding string \p s.
/// \see compare(start,len,s,sstart,slen)
inline int string::compare (const_pointer s) const
{
    return (compare (begin(), end(), s, NULL));
}

/// Returns true if this string is equal to character \p c.
inline bool string::operator== (const_reference c) const
{
    return (size() == 1 && c == at(0));
}

/// Returns true if this string is not equal to string \p s.
inline bool string::operator!= (const_pointer s) const
{
    return (!operator== (s));
}

/// Returns true if this string is less than to string \p s.
inline bool string::operator< (const string& s) const
{
    return (0 > compare (s));
}

/// Returns true if this string is less than to string \p s.
inline bool string::operator< (const_pointer s) const
{
    return (0 > compare (s));
}

/// Returns true if this string is less than to character \p c.
inline bool string::operator< (const_reference c) const
{
    return (0 > compare (begin(), end(), &c, &c + 1));
}

/// Returns true if this string is less than to string \p s.
inline bool string::operator> (const_pointer s) const
{
    return (0 < compare (s));
}

/// Sets the size of the string to 0. begin is not overwritten.
inline void string::clear (void)
{
    resize (0);
}

/// Inserts \p c at the end of the string.
inline void string::push_back (const_reference c)
{
    insert (end(), c);
}

/// Replaces range [\p start, \p start + \p len] with character \p c.
inline void string::replace (iterator first, iterator last, const_reference c, size_t n)
{
    replace (first, last, &c, &c + 1, n);
}

}; // namespace ustl

STD_STREAMABLE(ustl::string)

#endif

