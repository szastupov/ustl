// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//
// ufacets.cc
//

#include "ufacets.h"
#include "mistream.h"
#include "mostream.h"
#include "uios.h"
#include "utf8.h"
#include "ulimits.h"

namespace ustl {

//----------------------------------------------------------------------

const char c_Digits[] = "0123456789ABCDEF";

//----------------------------------------------------------------------

ctype::ctype (const locale& loc)
: locale::facet (loc)
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
    if ((m & print) && (c >= ' ' && c <= '~')) return (true);
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
    return (first);
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
    return (c < numeric_limits<char>::max() ? c : dfault);
}

const wchar_t* ctype::narrow (const wchar_t* first, const wchar_t* last, char dfault, char* result) const
{
    for (; first < last; ++ first, ++ result)
	*result = narrow (*first, dfault);	
    return (first);
}

//----------------------------------------------------------------------

numpunct::numpunct (const locale& loc)
: locale::facet (loc)
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
    return (string (""));
}

string numpunct::truename (void) const
{
    return (string ("true"));
}

string numpunct::falsename (void) const
{
    return (string ("false"));
}

//----------------------------------------------------------------------

num_get::num_get (const locale& loc)
: locale::facet (loc)
{
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, bool&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, int32_t&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, uint32_t&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, double&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, long double&) const
{
    return (first);
}

#if HAVE_INT64_T
num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, int64_t&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, uint64_t&) const
{
    return (first);
}
#endif

#if SIZE_OF_LONG == SIZE_OF_INT
num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, long&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, unsigned long&) const
{
    return (first);
}
#endif

#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, long long&) const
{
    return (first);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator, iosflags_t, unsigned long long&) const
{
    return (first);
}
#endif

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, int16_t& v) const
{
    int32_t vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<int16_t>(vl);
    return (result);
}

num_get::const_iterator num_get::get (const_iterator first, const_iterator last, iosflags_t flags, uint16_t& v) const
{
    uint32_t vl;
    const_iterator result = get (first, last, flags, vl);
    v = static_cast<uint32_t>(vl);
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
    uintptr_t vl;
    const_iterator result = get (first, last, flags, vl);
    v = reinterpret_cast<void*>(vl);
    return (result);
}

//----------------------------------------------------------------------

num_put::num_put (const locale& loc)
: locale::facet (loc)
{
}

inline size_t num_put::base_from_flags (iosflags_t flags) const
{
    if (flags & ios::hex)
	return (16);
    else if (flags & ios::oct)
	return (8);
    else
	return (10);
}

inline num_put::iterator num_put::write_buffer (const iterator& from, size_t n, iterator first, iterator last) const
{
    return (copy_n (from, min (n, size_t(last - first)), first));
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t, bool v, wchar_t) const
{
    const numpunct& names = use_facet<numpunct>(getloc());
    string name (v ? names.truename() : names.falsename());
    return (write_buffer (name.begin(), name.size(), first, last));
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, double, wchar_t) const
{
    return (first);
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t flags, long double v, wchar_t filler) const
{
    return (put (first, last, flags, static_cast<double>(v), filler));
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t flags, float v, wchar_t filler) const
{
    return (put (first, last, flags, static_cast<double>(v), filler));
}

num_put::iterator num_put::put (iterator first, iterator last, iosflags_t flags, void* v, wchar_t filler) const
{
    return (put (first, last, flags, reinterpret_cast<uintptr_t>(v), filler));
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, int32_t, wchar_t) const
{
    return (first);
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, uint32_t, wchar_t) const
{
    return (first);
}

#if HAVE_INT64_T
num_put::iterator num_put::put (iterator first, iterator, iosflags_t, int64_t, wchar_t) const
{
    return (first);
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, uint64_t, wchar_t) const
{
    return (first);
}
#endif

#if SIZE_OF_LONG == SIZE_OF_INT
num_put::iterator num_put::put (iterator first, iterator, iosflags_t, long, wchar_t) const
{
    return (first);
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, unsigned long, wchar_t) const
{
    return (first);
}
#endif

#if HAVE_LONG_LONG && (!HAVE_INT64_T || SIZE_OF_LONG_LONG > 8)
num_put::iterator num_put::put (iterator first, iterator, iosflags_t, long long, wchar_t) const
{
    return (first);
}

num_put::iterator num_put::put (iterator first, iterator, iosflags_t, unsigned long long, wchar_t) const
{
    return (first);
}
#endif

//----------------------------------------------------------------------

collate::collate (const locale& loc)
: locale::facet (loc)
{
}

int collate::compare (const char*, const char*, const char*, const char*) const
{
    return (0);
}

string collate::transform (const char* f1, const char* l1) const
{
    return (string (f1, l1));
}

/// Returns a hash value for [first, last)
hashvalue_t collate::hash (const char* first, const char* last) const
{
    hashvalue_t h = 0;
    // This has the bits flowing into each other from both sides of the number
    for (; first < last; ++ first)
	h = *first + ((h << 7) | (h >> BitsInType(hashvalue_t) - 7));
    return (h);
}

//----------------------------------------------------------------------

time_get::time_get (const locale& loc)
: locale::facet (loc)
{
}

time_get::dateorder time_get::date_order (void) const
{
    return (mdy);
}

time_get::const_iterator time_get::get_time (const_iterator first, const_iterator, iosflags_t, tm*) const
{
    return (first);
}

time_get::const_iterator time_get::get_date (const_iterator first, const_iterator, iosflags_t, tm*) const
{
    return (first);
}

time_get::const_iterator time_get::get_weekday (const_iterator first, const_iterator, iosflags_t, tm*) const
{
    return (first);
}

time_get::const_iterator time_get::get_monthname (const_iterator first, const_iterator, iosflags_t, tm*) const
{
    return (first);
}

time_get::const_iterator time_get::get_year (const_iterator first, const_iterator, iosflags_t, tm*) const
{
    return (first);
}

//----------------------------------------------------------------------

time_put::time_put (const locale& loc)
: locale::facet (loc)
{
}

time_put::iterator time_put::put (iterator first, iterator, iosflags_t, const tm*, wchar_t) const
{
    return (first);
}

//----------------------------------------------------------------------

money_punct::money_punct (const locale& loc)
: locale::facet (loc),
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
    return (string (""));
}

string money_punct::curr_symbol (void) const
{
    return (string ("$"));
}

string money_punct::positive_sign (void) const
{
    return (string ("+"));
}

string money_punct::negative_sign (void) const
{
    return (string ("-"));
}

int money_punct::frac_digits (void) const
{
    return (2);
}

money_punct::pattern money_punct::pos_format (void) const
{
    pattern p;
    return (p);
}

money_punct::pattern money_punct::neg_format (void) const
{
    pattern p;
    return (p);
}

//----------------------------------------------------------------------

money_get::money_get (const locale& loc)
: locale::facet (loc),
  money_base()
{
}

money_get::const_iterator money_get::get (const_iterator first, const_iterator, iosflags_t, long double&) const
{
    return (first);
}

money_get::const_iterator money_get::get (const_iterator first, const_iterator, iosflags_t, string&) const
{
    return (first);
}

//----------------------------------------------------------------------

money_put::money_put (const locale& loc)
: locale::facet (loc),
  money_base()
{
}

money_put::iterator money_put::put (iterator first, iterator, iosflags_t, long double, wchar_t) const
{
    return (first);
}

money_put::iterator money_put::put (iterator first, iterator, iosflags_t, const string&, wchar_t) const
{
    return (first);
}

//----------------------------------------------------------------------

messages::messages (const locale& loc)
: locale::facet (loc)
{
}

messages::catalog messages::open (const string&, const locale&)
{
    return (0);
}

string messages::get (catalog, int, int) const
{
    return (string (""));
}

void messages::close (catalog)
{
}

//----------------------------------------------------------------------

} // namespace ustl


