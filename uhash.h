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
/// \file uhash.h
//

#ifndef UHASH_H_2DE52D8701FC0EA57F4E79D5493CD9E2
#define UHASH_H_2DE52D8701FC0EA57F4E79D5493CD9E2

namespace ustl {

template <typename T> struct hash {};

hashvalue_t char_hash (const char* first, const char* last);

template <> struct hash<char*> {
    inline hashvalue_t	operator()(char* first) const
    { return (char_hash (first, first + strlen(first))); }
};
template <> struct hash<const char*> {
    inline hashvalue_t	operator()(const char* first) const
    { return (char_hash (first, first + strlen(first))); }
};
template <> struct hash<string> {
    inline hashvalue_t	operator()(const string& v) const
    { return (char_hash (v.begin(), v.end())); }
};
template <> struct hash<vector<char> > {
    inline hashvalue_t	operator()(const vector<char>& v) const
    { return (char_hash (v.begin(), v.end())); }
};

template <typename T>
inline hashvalue_t hash_value (const T& v)
{
    hash<T> h;
    return (h (v));
}

}; // namespace ustl

#endif

