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
// ulimits.h
//
/// \file ulimits.h
///
/// \brief This is a rewrite of the system \<limits\> file to use
/// C limits.h and be smaller.
///

#ifndef ULIMITS_H_1C2192EA3821E0811BBAF86B0F048364
#define ULIMITS_H_1C2192EA3821E0811BBAF86B0F048364

namespace ustl {

/// Defines numeric limits for a type.
template <typename T> 
struct numeric_limits {
    /// Returns the minimum value for type T.
    static inline T min (void) throw()		{ return (static_cast<T>(0)); }
    /// Returns the minimum value for type T.
    static inline T max (void) throw()		{ return (static_cast<T>(0)); }
    /// Returns true if the type is signed.
    static inline bool is_signed (void) throw()	{ return (false); }
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
struct numeric_limits<bool> {
    static inline bool min (void) throw()	{ return (false); }
    static inline bool max (void) throw()	{ return ( true); }
    static inline bool is_signed (void) throw()	{ return (false); }
};

template <>
struct numeric_limits<char> {
    static inline char min (void) throw()	{ return (CHAR_MIN); }
    static inline char max (void) throw()	{ return (CHAR_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<int> {
    static inline int min (void) throw()	{ return (INT_MIN); }
    static inline int max (void) throw()	{ return (INT_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<short> {
    static inline short min (void) throw()	{ return (SHRT_MIN); }
    static inline short max (void) throw()	{ return (SHRT_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<long> {
    static inline long min (void) throw()	{ return (LONG_MIN); }
    static inline long max (void) throw()	{ return (LONG_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<u_char> {
    static inline u_char min (void) throw()	{ return (0); }
    static inline u_char max (void) throw()	{ return (UCHAR_MAX); }
    static inline bool is_signed (void) throw()	{ return (false); }
};

template <>
struct numeric_limits<u_int> {
    static inline u_int min (void) throw()	{ return (0); }
    static inline u_int max (void) throw()	{ return (UINT_MAX); }
    static inline bool is_signed (void) throw()	{ return (false); }
};

template <>
struct numeric_limits<u_short> {
    static inline u_short min (void) throw()	{ return (0); }
    static inline u_short max (void) throw()	{ return (USHRT_MAX); }
    static inline bool is_signed (void) throw()	{ return (false); }
};

template <>
struct numeric_limits<u_long> {
    static inline u_long min (void) throw()	{ return (0); }
    static inline u_long max (void) throw()	{ return (ULONG_MAX); }
    static inline bool is_signed (void) throw()	{ return (false); }
};

template <>
struct numeric_limits<float> {
    static inline float min (void) throw()	{ return (FLT_MIN); }
    static inline float max (void) throw()	{ return (FLT_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<double> {
    static inline double min (void) throw()	{ return (DBL_MIN); }
    static inline double max (void) throw()	{ return (DBL_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<long double> {
    static inline long double min (void) throw(){ return (LDBL_MIN); }
    static inline long double max (void) throw(){ return (LDBL_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

#if defined(__USE_ISOC99) && !defined(__STRICT_ANSI__)
typedef long long llong;
typedef unsigned long long u_llong;

template <>
struct numeric_limits<llong> {
    static inline llong min (void) throw()	{ return (LONG_LONG_MIN); }
    static inline llong max (void) throw()	{ return (LONG_LONG_MAX); }
    static inline bool is_signed (void) throw()	{ return (true); }
};

template <>
struct numeric_limits<u_llong> {
    static inline u_llong min (void) throw()	{ return (0); }
    static inline u_llong max (void) throw()	{ return (ULONG_LONG_MAX); }
    static inline bool is_signed (void) throw()	{ return (false); }
};
#endif
#endif // DOXYGEN_SHOULD_SKIP_THIS

} // namespace ustl

#endif

