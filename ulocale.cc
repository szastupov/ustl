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

#include "ulocale.h"
#include "uexception.h"

namespace ustl {

//----------------------------------------------------------------------

locale locale::global;

//----------------------------------------------------------------------

locale::locale (void)
: m_Facets (),
  m_Name ()
{
}

locale::locale (const locale& l)
: m_Facets (l.m_Facets),
  m_Name (l.m_Name)
{
}

locale::locale (const locale&, const char*, category)
{
}

const bool locale::operator== (const locale&)
{
    return (true);
}

bool locale::operator() (const string&, const string&) const
{
    return (true);
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

}; // namespace ustl

