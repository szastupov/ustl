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
/// \file strmsize.h
/// \brief This file contains stream_size_of functions for basic types and *STREAMABLE macros.
/// stream_size_of functions return the size of the object's data that is written or
/// read from a stream.
//

#ifndef STRMSIZE_H
#define STRMSIZE_H

namespace ustl {

/// Returns the size of the given object. Overloads for standard types are available.
template <typename T>
inline size_t stream_size_of (T*)	{ return (sizeof(T*));		}
#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template <typename T>
inline size_t stream_size_of (const T*)	{ return (sizeof(const T*));	}
inline size_t stream_size_of (char)	{ return (sizeof(char));	}
inline size_t stream_size_of (u_char)	{ return (sizeof(u_char));	}
inline size_t stream_size_of (short)	{ return (sizeof(short));	}
inline size_t stream_size_of (u_short)	{ return (sizeof(u_short));	}
inline size_t stream_size_of (int)	{ return (sizeof(int));		}
inline size_t stream_size_of (u_int)	{ return (sizeof(u_int));	}
inline size_t stream_size_of (long)	{ return (sizeof(long));	}
inline size_t stream_size_of (u_long)	{ return (sizeof(u_long));	}
inline size_t stream_size_of (float)	{ return (sizeof(float));	}
inline size_t stream_size_of (double)	{ return (sizeof(double));	}
inline size_t stream_size_of (bool)	{ return (sizeof(bool));	}
inline size_t stream_size_of (wchar_t)	{ return (sizeof(wchar_t));	}
#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace ustl

/// Declares that T is not written to istream/ostream.
#define NOT_STREAMABLE(T)	\
    namespace ustl {		\
	inline istream& operator>> (istream& is, T&)		{ return (is); }	\
	inline ostream& operator<< (ostream& os, const T&)	{ return (os); }	\
	inline size_t stream_size_of (const T&)			{ return (0); }		\
    }

//
// Extra overloads in this macro are needed because it is the one used for
// marshalling pointers. Passing a pointer to stream_size_of creates a
// conversion ambiguity between converting to const pointer& and converting
// to bool; the compiler always chooses the bool conversion (because it
// requires 1 conversion instead of 2 for the other choice). There is little
// point in adding the overloads to other macros, since they are never used
// for pointers.
//
/// Declares that T is to be written as is into binary streams.
#define INTEGRAL_STREAMABLE(T)	\
    namespace ustl {		\
	inline istream& operator>> (istream& is, T& v)		{ is.iread(v);  return (is); }	\
	inline ostream& operator<< (ostream& os, const T& v)	{ os.iwrite(v); return (os); }	\
	inline ostream& operator<< (ostream& os, T& v)		{ os.iwrite(v); return (os); }	\
	inline size_t stream_size_of (const T& v)		{ return (sizeof(v)); }		\
	inline size_t stream_size_of (T& v)			{ return (sizeof(v)); }		\
    }

/// Declares that T contains read, write, and stream_size methods.
#define STD_STREAMABLE(T)	\
    namespace ustl {		\
	inline istream& operator>> (istream& is, T& v)		{ v.read (is);  return (is); }	\
	inline ostream& operator<< (ostream& os, const T& v)	{ v.write (os); return (os); }	\
	inline size_t stream_size_of (const T& v)		{ return (v.stream_size()); }	\
    }

/// Declares that private member T is STD_STREAMABLE
#define FRIEND_STD_STREAMABLE(T)					\
    friend ustl::istream& ustl::operator>> (ustl::istream& is, T& v);	\
    friend ustl::ostream& ustl::operator<< (ostream& os, const T& v);	\
    friend size_t ustl::stream_size_of (const T& v)

/// Declares that T is to be cast into TSUB for streaming.
#define CAST_STREAMABLE(T,TSUB)	\
    namespace ustl {		\
	inline istream& operator>> (istream& is, T& v)		{ TSUB sv; is >> sv; v = (T)(sv); return (is); }	\
	inline ostream& operator<< (ostream& os, const T& v)	{ os << TSUB(v); return (os); }			\
	inline size_t stream_size_of (const T& v)		{ return (sizeof(TSUB(v))); }				\
    }

/// Placed into a class it declares the methods required by STD_STREAMABLE. Syntactic sugar.
#define DECLARE_STD_STREAMABLE		\
    public:				\
        void	read (::ustl::istream& is);		\
	void	write (::ustl::ostream& os) const;	\
	size_t	stream_size (void) const

/// Declares T to be writable to text streams. Reading is not implemented because you should not do it.
#define TEXT_STREAMABLE(T)	\
    namespace ustl {		\
	inline ostringstream& operator<< (ostringstream& os, const T& v)	{ v.text_write (os); return (os); }	\
	inline fdostringstream& operator<< (fdostringstream& os, const T& v)	{ v.text_write (os); return (os); }	\
    }

#endif

