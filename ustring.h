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
#include <stdarg.h>	// for va_list, va_start, and va_end (in string::format)

namespace ustl {

/// \class string ustring.h ustl.h
/// \ingroup Sequences
///
/// \brief STL basic_string equivalent.
///
/// An STL container for string manipulation.
/// Differences from C++ standard:
///	string is a class, not a template. Wide characters are assumed to be
///		encoded with utf8 at all times except when rendering or editing.
///	all functions work with iterators instead of indexes. This makes for
///		a cleaner syntax in most cases, and, of course, a cleaner
///		implementation. The exception to expression clarity is the
///		code that changes the string based on fixed offsets. You can
///		bite it and use begin() offsets, but a better solution is to
///		simply not do it. Code written in that manner is too
///		inflexible. Search for separators, or use the replace call.
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
/// 	length() returns the number of _characters_, not bytes. This function
///		is O(N), since the character count is not stored, so use wisely.
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
    static const uoff_t npos = static_cast<uoff_t>(-1);			///< Value that means the end of string.
    static const value_type c_Terminator = 0;				///< String terminator
    static const size_type size_Terminator = sizeof(c_Terminator);	///< Most systems terminate strings with '\\0'
    static const char empty_string [size_Terminator];			///< An empty string.
public:
				string (void);
				string (const string& s);
    explicit			string (const cmemlink& l);
				string (const_pointer s);
				string (const_pointer s, size_type len);
				string (const_pointer s1, const_pointer s2);
    explicit			string (size_type n, value_type c = c_Terminator);
    size_type			length (void) const;
    const_iterator		ichar (uoff_t c) const;
    iterator			ichar (uoff_t c);
    inline pointer		data (void)		{ return (string::pointer (memblock::data())); }
    inline const_pointer	c_str (void) const	{ return (string::const_pointer (memblock::cdata())); }
    inline size_type		max_size (void) const	{ return (memblock::max_size() - size_Terminator); }
    inline size_type		capacity (void) const	{ return (memblock::capacity() ? memblock::capacity() - size_Terminator : 0); }
    void			resize (size_type n);
    inline const_iterator	begin (void) const	{ return (const_iterator (memblock::begin())); }
    inline iterator		begin (void)		{ return (iterator (memblock::begin())); }
    inline const_iterator	end (void) const	{ return (const_iterator (memblock::end())); }
    inline iterator		end (void)		{ return (iterator (memblock::end())); }
  inline const_reverse_iterator	rbegin (void) const	{ return (const_reverse_iterator (end())); }
    inline reverse_iterator	rbegin (void)		{ return (reverse_iterator (end())); }
  inline const_reverse_iterator	rend (void) const	{ return (const_reverse_iterator (begin())); }
    inline reverse_iterator	rend (void)		{ return (reverse_iterator (begin())); }
    inline const_reference	at (uoff_t pos) const;
    inline reference		at (uoff_t pos);
    wchar_t			char_at (uoff_t pos) const;
    inline			operator const value_type* (void) const;
    inline			operator value_type* (void);
    inline const string&	operator= (const string& s);
    inline const string&	operator= (const_reference c);
    inline const string&	operator= (const_pointer s);
    inline const string&	operator+= (const string& s);
    inline const string&	operator+= (const_reference c);
    inline const string&	operator+= (const_pointer s);
    inline const string&	operator+= (wchar_t c);
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
    void	    		assign (const_pointer s, size_type len);
    void	    		assign (const_pointer s);
    inline void			append (const_iterator i1, const_iterator i2);
    void	   		append (const_pointer s, size_type len);
    void	   		append (const_pointer s);
    void			append (size_type n, const_reference c);
    void			append (size_type n, wchar_t c);
    size_type			copyto (pointer p, size_type n, const_iterator start) const;
    inline int			compare (const string& s) const;
    inline int			compare (const_pointer s) const;
    int				compare (const_iterator first1, const_iterator last1, const_iterator first2, const_iterator last2) const;
    void			insert (const uoff_t ip, wchar_t c, size_type n = 1);
    void			insert (const uoff_t ip, const wchar_t* first, const wchar_t* last, const size_type n = 1);
    void			insert (iterator start, const_reference c, size_type n = 1);
    void			insert (iterator start, const_pointer s, size_type n = 1);
    void			insert (iterator start, const_pointer first, const_iterator last, size_type n = 1);
    iterator			erase (iterator start, size_type size = 1);
    void			erase (uoff_t start, size_type size = 1);
    inline void			push_back (const_reference c);
    inline void			push_back (wchar_t c);
    inline void			pop_back (void);
    inline void			clear (void);
    inline void			replace (iterator first, iterator last, const_reference c, size_type n = 1);
    void			replace (iterator first, iterator last, const_pointer s, size_type n = 1);
    void			replace (iterator first, iterator last, const_pointer i1, const_pointer i2, size_type n = 1);
    const_iterator		find (const_reference c, const_iterator pos = NULL) const;
    const_iterator		find (const string& s, const_iterator pos = NULL) const;
    const_iterator		rfind (const_reference c, const_iterator pos = NULL) const;
    const_iterator		rfind (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_first_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_first_not_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_last_of (const string& s, const_iterator pos = NULL) const;
    const_iterator		find_last_not_of (const string& s, const_iterator pos = NULL) const;
    int				vformat (const char* fmt, va_list args);
    int				format (const char* fmt, ...) __attribute__((__format__(__printf__, 2, 3)));
    void			read (istream&);
    void			write (ostream& os) const;
    size_t			stream_size (void) const;
protected:
    virtual size_type		minimumFreeCapacity (void) const;
};

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

/// Returns the pointer to the first character.
inline string::operator const string::value_type* (void) const
{
    assert (*end() == c_Terminator && "This string is linked to data that is not 0-terminated. This may cause serious security problems. Please assign the data instead of linking.");
    return (begin());
}

/// Returns the pointer to the first character.
inline string::operator string::value_type* (void)
{
    assert (*end() == c_Terminator && "This string is linked to data that is not 0-terminated. This may cause serious security problems. Please assign the data instead of linking.");
    assert ((begin() || !c_str()) && "Requesting non-const pointer to a const string. Don't do that.");
    return (begin());
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

/// Assigns itself the value of string \p s
inline const string& string::operator= (const string& s)
{
    assign (s.begin(), s.end());
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
    append (1, c);
    return (*this);
}

/// Appends itself the value of 0-terminated string \p s
inline const string& string::operator+= (const_pointer s)
{
    append (s);
    return (*this);
}

/// Appends character \p c
inline const string& string::operator+= (wchar_t c)
{
    append (1, c);
    return (*this);
}

/// Concatenates itself with \p s
inline string string::operator+ (const string& s) const
{
    string result (*this);
    result += s;
    return (result);
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

/// Inserts \p c at the end of the string.
inline void string::push_back (const_reference c)
{
    append (1, c);
}

/// Inserts \p c at the end of the string.
inline void string::push_back (wchar_t c)
{
    append (1, c);
}

/// Erases one element at the end of the string
inline void string::pop_back (void)
{
    resize (size() - 1);
}

/// Removes all characters from the string.
inline void string::clear (void)
{
    resize (0);
}

/// Replaces range [\p start, \p start + \p len] with character \p c.
inline void string::replace (iterator first, iterator last, const_reference c, size_type n)
{
    replace (first, last, &c, &c + 1, n);
}

//----------------------------------------------------------------------
// Operators needed to avoid comparing pointer to pointer

/// Returns true if \p s1 is equal to \p s2
inline bool operator== (const char* s1, const string& s2)
{
    return (s2 == s1);
}

/// Returns true if \p s1 is not equal to \p s2
inline bool operator!= (const char* s1, const string& s2)
{
    return (s2 != s1);
}

/// Returns true if \p s1 is equal to \p s2
inline bool operator< (const char* s1, const string& s2)
{
    return (s2 > s1);
}

/// Returns true if \p s1 is less than or equal to \p s2
inline bool operator<= (const char* s1, const string& s2)
{
    return (s2 >= s1);
}

/// Returns true if \p s1 is equal to \p s2
inline bool operator> (const char* s1, const string& s2)
{
    return (s2 < s1);
}

/// Returns true if \p s1 is greater than or equal to \p s2
inline bool operator>= (const char* s1, const string& s2)
{
    return (s2 <= s1);
}

// Specialization for stream alignment
template <> inline size_t alignof (string) { return (alignof (string::value_type())); }

} // namespace ustl

#endif

