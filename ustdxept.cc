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
// ustdxept.cc
//

#include "ustdxept.h"
#include "mistream.h"
#include "mostream.h"
#include "strmsize.h"
#include "uiosfunc.h"
#include "uspecial.h"

namespace ustl {

//----------------------------------------------------------------------

/// \p arg contains a description of the error.
logic_error::logic_error (const string& arg)
: m_Arg ()
{
    try { m_Arg = arg; } catch (...) {}
    set_format (xfmt_LogicError);
}

logic_error::~logic_error (void)
{
}

const char* logic_error::what (void) const
{
    return ("logic error");
}

/// Returns a descriptive error message. fmt="%s: %s"
void logic_error::info (string& msgbuf, const char* fmt) const
{
    if (!fmt) fmt = "%s: %s";
    try { msgbuf.format (fmt, what(), m_Arg.cdata()); } catch (...) {}
}

void logic_error::read (istream& is)
{
    exception::read (is);
    is >> m_Arg >> ios::align();
}

void logic_error::write (ostream& os) const
{
    exception::write (os);
    os << m_Arg << ios::align();
}

size_t logic_error::stream_size (void) const
{
    return (exception::stream_size() + Align (stream_size_of (m_Arg)));
}

//----------------------------------------------------------------------

domain_error::domain_error (const string& arg)
: logic_error (arg)
{
}

const char* domain_error::what (void) const
{
    return ("domain error");
}

//----------------------------------------------------------------------

invalid_argument::invalid_argument (const string& arg)
: logic_error (arg)
{
}

const char* invalid_argument::what (void) const
{
    return ("invalid argument");
}

//----------------------------------------------------------------------

length_error::length_error (const string& arg)
: logic_error (arg)
{
}

const char* length_error::what (void) const
{
    return ("length error");
}

//----------------------------------------------------------------------

out_of_range::out_of_range (const string& arg)
: logic_error (arg)
{
}

const char* out_of_range::what (void) const
{
    return ("out of range");
}

//----------------------------------------------------------------------

/// \p arg contains a description of the error.
runtime_error::runtime_error (const string& arg)
: m_Arg ()
{
    try { m_Arg = arg; } catch (...) {}
    set_format (xfmt_RuntimeError);
}

runtime_error::~runtime_error (void)
{
}

const char* runtime_error::what (void) const
{
    return ("runtime error");
}

/// Returns a descriptive error message. fmt="%s: %s"
void runtime_error::info (string& msgbuf, const char* fmt) const
{
    if (!fmt) fmt = "%s: %s";
    try { msgbuf.format (fmt, what(), m_Arg.cdata()); } catch (...) {}
}

void runtime_error::read (istream& is)
{
    exception::read (is);
    is >> m_Arg >> ios::align();
}

void runtime_error::write (ostream& os) const
{
    exception::write (os);
    os << m_Arg << ios::align();
}

size_t runtime_error::stream_size (void) const
{
    return (exception::stream_size() + Align (stream_size_of (m_Arg)));
}

//----------------------------------------------------------------------

range_error::range_error (const string& arg)
: runtime_error (arg)
{
}

const char* range_error::what (void) const
{
    return ("range error");
}

//----------------------------------------------------------------------

overflow_error::overflow_error (const string& arg)
: runtime_error (arg)
{
}

const char* overflow_error::what (void) const
{
    return ("overflow error");
}

//----------------------------------------------------------------------

underflow_error::underflow_error (const string& arg)
: runtime_error (arg)
{
}

const char* underflow_error::what (void) const
{
    return ("underflow error");
}

//----------------------------------------------------------------------

}; // namespace ustl


