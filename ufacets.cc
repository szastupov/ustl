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
// ufacets.cc
//

#include "ufacets.h"
#include "utf8.h"

namespace ustl {

//----------------------------------------------------------------------

ctype::ctype (void)
: locale::facet ()
{
}

bool ctype::is (mask m, wchar_t c) const
{
    if ((m & upper) && (c >= 'A' && c <= 'Z')) return (true);
    if ((m & lower) && (c >= 'a' && c <= 'a')) return (true);
    if ((m & alpha) && ((c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z'))) return (true);
    if ((m & digit) && (c >= '0' && c <= '9')) return (true);
    if ((m & xdigit) && ((c >= '0' && c <= '9') ||
			 (c >= 'A' && c <= 'F') ||
			 (c >= 'a' && c <= 'f'))) return (true);
    if ((m & space) && (c == ' ' ||
			c == '\t' ||
			c == '\n' ||
			c == '\r')) return (true);
    if ((m & print) && (c >= 'A' && c <= 'Z')) return (true);
    if ((m & graph) && (c > 127)) return (true);
    if ((m & cntrl) && c < 32) return (true);
    if ((m & punct) && (c >= 'A' && c <= 'Z')) return (true);
    if ((m & alnum) && (c >= 'A' && c <= 'Z')) return (true);
    return (false);
}

const char* ctype::scan_is (mask m, const char* first, const char* last) const
{
    for (; first < last && !is(m, *first); ++ first);
    return (first);
}

const char* ctype::scan_not (mask m, const char* first, const char* last) const
{
    for (; first < last && is(m, *first); ++ first);
    return (first);
}

wchar_t ctype::toupper (wchar_t c) const
{
    return ((c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c);
}

const char* ctype::toupper (char* first, const char* last) const
{
    for (; first < last; ++ first)
	*first = toupper (*first);
}

wchar_t ctype::tolower (wchar_t c) const
{
    return ((c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c);
}

const char* ctype::tolower (char* first, const char* last) const
{
    for (; first < last; ++ first)
	*first = tolower (*first);
    return (first);
}

const char* ctype::widen (const char* first, const char* last, wchar_t* result) const
{
    for (; first < last; ++ first, ++ result)
	*result = *first;	
    return (first);
}

char ctype::narrow (wchar_t c, char dfault) const
{
    return (c < CHAR_MAX ? c : dfault);
}

const wchar_t* ctype::narrow (const wchar_t* first, const wchar_t* last, char dfault, char* result) const
{
    for (; first < last; ++ first, ++ result)
	*result = narrow (*first);	
    return (first);
}

//----------------------------------------------------------------------

numpunct::numpunct (void)
: locale::facet ()
{
}

wchar_t numpunct::decimal_point (void) const
{
    return ('.');
}

wchar_t numpunct::thousands_sep (void) const
{
    return (',');
}

string numpunct::grouping (void) const
{
    return ("");
}

string numpunct::truename (void) const
{
    return ("true");
}

string numpunct::falsename (void) const
{
    return ("false");
}

//----------------------------------------------------------------------

num_get::num_get (void)
: locale::facet()
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, bool& v) const
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, long& v) const
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, u_long& v) const
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, double& v) const
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, long double& v) const
{
}

#ifdef __GNUC__

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, llong& v) const
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t, u_llong& v) const
{
}

#endif

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, short& v) const
{
    long vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<short>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, int& v) const
{
    long vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<int>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, u_short& v) const
{
    u_long vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<u_short>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, u_int& v) const
{
    u_long vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<u_int>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, float& v) const
{
    double vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<float>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, void*& v) const
{
    u_long vl;
    const_iterator result = get (first, last, flags, vl);
    v = reinterpret_cast<void*>(vl);
    return (result);
}

//----------------------------------------------------------------------

num_put::num_put (void)
: locale::facet()
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, bool v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, long v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, u_long v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, double v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, long double v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, float v, wchar_t filler) const
{
    return (put (first, last, static_cast<double>(v), filler));
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, void* v, wchar_t filler) const
{
    return (put (first, last, reinterpret_cast<u_long>(v), filler));
}

#ifdef __GNUC__

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, llong v, wchar_t filler) const
{
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, u_llong v, wchar_t filler) const
{
}

#endif

//----------------------------------------------------------------------

collate::collate (void)
: locale::facet ()
{
}

int collate::compare (const char* f1, const char* l1, const char* f2, const char* l2) const
{
}

string collate::transform (const char* f1, const char* l1) const
{
    return (string (f1, l1));
}

hashvalue_t collate::hash (const char* first, const char* last) const
{
    hashvalue_t h = 0;
    // This has the bits flowing into each other from both sides of the u_long
    for (; first < last; ++ first)
	h = *first + ((h << 7) | (h >> BitsInType(hashvalue_t) - 7));
    return (h);
}

//----------------------------------------------------------------------

time_get::time_get (void)
{
}

time_get::dateorder time_get::date_order (void) const
{
}

time_get::const_iterator time_get::get_time (const_iterator first, const_iterator last, iosflags_t, tm* v) const
{
}

time_get::const_iterator time_get::get_date (const_iterator first, const_iterator last, iosflags_t, tm* v) const
{
}

time_get::const_iterator time_get::get_weekday (const_iterator first, const_iterator last, iosflags_t, tm* v) const
{
}

time_get::const_iterator time_get::get_monthname (const_iterator first, const_iterator last, iosflags_t, tm* v) const
{
}

time_get::const_iterator time_get::get_year (const_iterator first, const_iterator last, iosflags_t, tm* v) const
{
}

//----------------------------------------------------------------------

time_get::time_get (void)
: locale::facet()
{
}

time_get::iterator time_get::put (iterator first, iterator last, iosflags_t, const tm* v, wchar_t filler) const
{
}

//----------------------------------------------------------------------

money_punct::money_punct (void)
: locale::facet(),
  money_base()
{
}

wchar_t money_punct::decimal_point (void) const
{
    return ('.');
}

wchar_t money_punct::thousands_sep (void) const
{
    return (',');
}

string money_punct::grouping (void) const
{
    return ("");
}

string money_punct::curr_symbol (void) const
{
    return ("$");
}

string money_punct::positive_sign (void) const
{
    return ("+");
}

string money_punct::negative_sign (void) const
{
    return ("-");
}

int money_punct::frac_digits (void) const
{
    return (2);
}

money_punct::pattern money_punct::pos_format (void) const
{
    return (pattern());
}

money_punct::pattern money_punct::neg_format (void) const
{
    return (pattern());
}

//----------------------------------------------------------------------

money_get::money_get (void)
: locale::facet(),
  money_base()
{
}

money_get::const_iterator money_get::get (const_iterator first, const_iterator last, iosflags_t, long double& v) const
{
}

money_get::const_iterator money_get::get (const_iterator first, const_iterator last, iosflags_t, string& v) const
{
}

//----------------------------------------------------------------------

money_put::money_put (void)
: locale::facet(),
  money_base()
{
}

money_put::iterator money_put::put (iterator first, iterator last, iosflags_t, long double v, wchar_t filler) const
{
}

money_put::iterator money_put::put (iterator first, iterator last, iosflags_t, const string& v, wchar_t filler) const
{
}

//----------------------------------------------------------------------

messages::messages (void)
{
}

messages::catalog messages::open (const string& name, const locale& loc)
{
}

string messages::get (catalog c, int s, int msgid) const
{
}

void messages::close (catalog c)
{
}

//----------------------------------------------------------------------

}; // namespace ustl


