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
// ulocale.h
//

#ifndef ULOCALE_H_5CFAA64522EEF45851F2DED048480AD4
#define ULOCALE_H_5CFAA64522EEF45851F2DED048480AD4

#include "utuple.h"
#include "ustring.h"

namespace ustl {

//----------------------------------------------------------------------

class locale {
public:
    enum category_bit {
	ctype_bit,
	numeric_bit,
	collate_bit,
	time_bit,
	monetary_bit,
	messages_bit,
	all_categories
    };
    typedef u_int	category;
public:
    class facet {
    protected:
	typedef char*		iterator;
	typedef const char*	const_iterator;
	typedef uint32_t	iosflags_t;
    protected:
	explicit	facet (const locale& loc);
	virtual	       ~facet (void);
	inline const locale&	getloc (void) const { return (m_Locale); }
    private:
	inline		facet (const facet& v) : m_Locale (v.m_Locale) {}
	inline void	operator= (const facet&) {}
    private:
	const locale&	m_Locale;
    };
public:
    static const category none		= 0;
    static const category ctype		= 1 << ctype_bit;
    static const category numeric	= 1 << numeric_bit;
    static const category collate	= 1 << collate_bit;
    static const category time		= 1 << time_bit;
    static const category monetary	= 1 << monetary_bit;
    static const category messages	= 1 << messages_bit;
    static const category all		= BitMask(category,all_categories);
public:
    static locale	global;
public:
			locale (void);
			locale (const locale& l);
			locale (const locale& l, const char* std_name, category cat);
    const bool		operator== (const locale& l);
    bool		operator() (const string& s1, const string& s2) const;
    inline bool		has_facet (category_bit f) const;
    const facet&	get_facet (category_bit f) const;
private:
    typedef tuple<all_categories, facet*> facetvec_t;
private:
    facetvec_t		m_Facets;
    string		m_Name;
};

inline bool locale::has_facet (category_bit f) const
{
    return (m_Facets [f]);
}

template <typename Facet>
const Facet& use_facet (const locale& l)
{
    return (static_cast<const Facet&>(l.get_facet (Facet::c_Category)));
}

template <typename Facet>
bool has_facet (const locale& l)
{
    return (l.has_facet (Facet::c_Category));
}

//----------------------------------------------------------------------

}; // namespace ustl

#endif

