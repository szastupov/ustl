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
// ulocale.cc
//

#include "ufacets.h"
#include "uexception.h"
#include "uutility.h"

namespace ustl {

//----------------------------------------------------------------------

locale locale::global;

//----------------------------------------------------------------------

locale::locale (void)
: m_Facets (),
  m_Name (),
  m_bFacetOwner (true)
{
    m_Facets [ctype_bit]	= new ustl::ctype (*this);
    m_Facets [numeric_bit]	= new ustl::num_put (*this);
    m_Facets [collate_bit]	= new ustl::collate (*this);
    m_Facets [time_bit]		= new ustl::time_put (*this);
    m_Facets [monetary_bit]	= new ustl::money_put (*this);
    m_Facets [messages_bit]	= new ustl::messages (*this);
    m_Facets [numpunct_bit]	= new ustl::numpunct (*this);
}

locale::locale (const locale& l)
: m_Facets (l.m_Facets),
  m_Name (l.m_Name),
  m_bFacetOwner (false)
{
}

locale::locale (const locale& l, const char* name, category)
: m_Facets (l.m_Facets),
  m_Name (name),
  m_bFacetOwner (false)
{
}

locale::~locale (void)
{
    if (m_bFacetOwner)
	for_each (m_Facets.begin(), m_Facets.end(), &Delete<facet>);
}

const locale& locale::operator= (const locale& l)
{
    if (m_bFacetOwner)
	for_each (m_Facets.begin(), m_Facets.end(), &Delete<facet>);
    m_bFacetOwner = false;
    m_Facets = l.m_Facets;
    m_Name = l.m_Name;
    return (*this);
}

const bool locale::operator== (const locale& l) const
{
    return (m_Name == l.m_Name);
}

bool locale::operator() (const string& s1, const string& s2) const
{
    return (use_facet<ustl::collate>(*this).compare (s1.begin(), s1.end(), s2.begin(), s2.end()));
}

const locale::facet& locale::get_facet (category_bit f) const
{
    if (!has_facet (f))
	throw bad_cast();
    return (*m_Facets [f]);
}

//----------------------------------------------------------------------

locale::facet::facet (const locale& loc)
: m_Locale (loc)
{
}

locale::facet::~facet (void)
{
}

//----------------------------------------------------------------------

} // namespace ustl

