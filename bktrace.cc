// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2006 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// bktrace.cc
//

#include "bktrace.h"
#include "sostream.h"
#include "mistream.h"
#if linux && __GNUC__
    #include <execinfo.h>
#else
    #define backtrace(a,n)		0
    #define backtrace_symbols(a,n)	NULL
#endif
#if __GNUC__ >= 3
    #include <cxxabi.h>
#endif

#if SIZE_OF_LONG == 8
    #define ADDRESS_FMT	"%16p  %s\n"
#else
    #define ADDRESS_FMT	"%8p  %s\n"
#endif

namespace ustl {

/// Default constructor. The backtrace is obtained here.
CBacktrace::CBacktrace (void)
: m_Text (NULL),
  m_nFrames (0)
{
    try {
	m_nFrames = backtrace (m_Addresses, VectorSize(m_Addresses));
	GetSymbols();
    } catch (...) {}
}

/// Copy constructor.
CBacktrace::CBacktrace (const CBacktrace& v)
: m_Text (NULL),
  m_nFrames (0)
{
    operator= (v);
}

/// Copy operator.
const CBacktrace& CBacktrace::operator= (const CBacktrace& v)
{
    memcpy (m_Addresses, v.m_Addresses, sizeof(m_Addresses));
    m_Text = strdup (v.m_Text);
    return (*this);
}

/// Tries to get symbol information for the addresses.
void CBacktrace::GetSymbols (void)
{
    auto_ptr<char*> symbols (backtrace_symbols (m_Addresses, m_nFrames));
    if (!symbols.get())
	return;
    ostringstream os;
    char nmbuf [256];
    char dmbuf [256];
    for (uoff_t i = 0; i < m_nFrames; ++ i) {
	// Prepare the demangled name, if possible
	size_t nmSize = 0;
	const char* isym = symbols.get()[i];
	if (isym) {
	    // Copy out the name; the strings are: "file(function+0x42) [0xAddress]"
	    const char* mnStart = strchr (isym, '(');
	    if (++mnStart == (const char*)(1))
		mnStart = isym;
	    const char* mnEnd = strchr (isym, '+');
	    const char* isymEnd = isym + strlen (isym);
	    if (!mnEnd)
		mnEnd = isymEnd;
	    nmSize = min (size_t (distance (mnStart, mnEnd)), VectorSize(nmbuf));
	    memcpy (nmbuf, mnStart, nmSize);
	}
	nmbuf[nmSize] = 0;
	// Demangle
	#if __GNUC__ >= 3
	    int dmFailed = 0;
	    size_t dmSize = VectorSize (dmbuf);
	    abi::__cxa_demangle (nmbuf, dmbuf, &dmSize, &dmFailed);
	#else
	    int dmFailed = -1;
	#endif
	// Print the result with the address
	os.format (ADDRESS_FMT, m_Addresses[i], dmFailed ? nmbuf : dmbuf);
    }
    m_Text = strdup (os.str().c_str());
}

/// Default destructor.
CBacktrace::~CBacktrace (void)
{
    free_nullok (m_Text);
}

/// Prints the backtrace to \p os.
void CBacktrace::text_write (ostringstream& os) const
{
    if (m_Text)
	os << m_Text;
}

/// Reads the object from stream \p is.
void CBacktrace::read (istream& is)
{
    is.read (m_Addresses, sizeof(m_Addresses));
    is >> m_nFrames;
    size_t tlen = 0;
    is >> tlen;
    free_nullok (m_Text);
    m_Text = (char*) malloc (tlen + 1);
    is.read (m_Text, tlen);
    m_Text[tlen] = 0;
}

/// Writes the object to stream \p os.
void CBacktrace::write (ostream& os) const
{
    os.write (m_Addresses, sizeof(m_Addresses));
    os << m_nFrames;
    size_t tlen = 0;
    if (m_Text) {
	tlen = strlen (m_Text);
	os << tlen;
	os.write (m_Text, tlen);
    } else
	os << tlen;
    os.align();
}

/// Returns the size of the written object.
size_t CBacktrace::stream_size (void) const
{
    return (Align (sizeof(m_Addresses) +
		   stream_size_of (m_nFrames) +
		   stream_size_of (size_t()) +
		   m_Text ? strlen (m_Text) : 0));
}

} // namespace ustl

