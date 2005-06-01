// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
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
logic_error::logic_error (const char* arg) throw()
: m_Arg ()
{
    try { m_Arg = arg; } catch (...) {}
    set_format (xfmt_LogicError);
}

/// Virtual destructor
logic_error::~logic_error (void) throw()
{
}

const char* logic_error::what (void) const throw()
{
    return ("logic error");
}

/// Returns a descriptive error message. fmt="%s: %s"
void logic_error::info (string& msgbuf, const char* fmt) const throw()
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

domain_error::domain_error (const char* arg) throw()
: logic_error (arg)
{
}

const char* domain_error::what (void) const throw()
{
    return ("domain error");
}

//----------------------------------------------------------------------

invalid_argument::invalid_argument (const char* arg) throw()
: logic_error (arg)
{
}

const char* invalid_argument::what (void) const throw()
{
    return ("invalid argument");
}

//----------------------------------------------------------------------

length_error::length_error (const char* arg) throw()
: logic_error (arg)
{
}

const char* length_error::what (void) const throw()
{
    return ("length error");
}

//----------------------------------------------------------------------

out_of_range::out_of_range (const char* arg) throw()
: logic_error (arg)
{
}

const char* out_of_range::what (void) const throw()
{
    return ("out of range");
}

//----------------------------------------------------------------------

/// \p arg contains a description of the error.
runtime_error::runtime_error (const char* arg) throw()
: m_Arg ()
{
    try { m_Arg = arg; } catch (...) {}
    set_format (xfmt_RuntimeError);
}

runtime_error::~runtime_error (void) throw()
{
}

const char* runtime_error::what (void) const throw()
{
    return ("runtime error");
}

/// Returns a descriptive error message. fmt="%s: %s"
void runtime_error::info (string& msgbuf, const char* fmt) const throw()
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

range_error::range_error (const char* arg) throw()
: runtime_error (arg)
{
}

const char* range_error::what (void) const throw()
{
    return ("range error");
}

//----------------------------------------------------------------------

overflow_error::overflow_error (const char* arg) throw()
: runtime_error (arg)
{
}

const char* overflow_error::what (void) const throw()
{
    return ("overflow error");
}

//----------------------------------------------------------------------

underflow_error::underflow_error (const char* arg) throw()
: runtime_error (arg)
{
}

const char* underflow_error::what (void) const throw()
{
    return ("underflow error");
}

//----------------------------------------------------------------------

} // namespace ustl


