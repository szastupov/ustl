// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// cmemlink.cc
//
// See cmemlink.h for documentation.
//

#include "cmemlink.h"
#include "fdostream.h"
#include "strmsize.h"
#include "ualgo.h"

namespace ustl {

/// Default constructor initializes to point to NULL,0
cmemlink::cmemlink (void)
: m_CData (NULL),
  m_Size (0)
{
}

/// Attaches the object to pointer \p p of size \p n.
cmemlink::cmemlink (const void* p, size_type n)
: m_CData (reinterpret_cast<const_pointer>(p)),
  m_Size (n)
{
    assert (p || !n);
}

/// Copies values from l
cmemlink::cmemlink (const cmemlink& l)
: m_CData (l.m_CData),
  m_Size (l.m_Size)
{
}

/// \brief Attaches the object to pointer \p p of size \p n.
///
/// If \p p is NULL and \p n is non-zero, bad_alloc is thrown and current
/// state remains unchanged.
///
void cmemlink::link (const void* p, size_type n)
{
    if (!p && n)
	throw bad_alloc (n);
    unlink();
    relink (p, n);
}

/// \brief Resets all to 0.
///
/// \warning Do NOT override this function. It is virtual only for
/// memlink, memblock, and the streams; there is no way to
/// "unvirtualize" a function, hence the documentation.
///
void cmemlink::unlink (void)
{
    m_CData = NULL;
    m_Size = 0;
}

/// Writes the object to stream \p os
void cmemlink::write (ostream& os) const
{
    const uint32_t sz (size());
    assert (sz == size() && "No support for writing memblocks larger than 4G");
    os << sz;
    os.write (cdata(), sz);
    os.align();
}

/// Writes the object to stream \p os
void cmemlink::text_write (ostringstream& os) const
{
    os.write (begin(), readable_size());
}

/// Returns the number of bytes required to write this object to a stream.
cmemlink::size_type cmemlink::stream_size (void) const
{
    const uint32_t sz (size());
    return (Align (stream_size_of (sz) + sz));
}

/// Writes the data to file \p "filename".
void cmemlink::write_file (const char* filename, int mode) const
{
    fstream f;
    f.exceptions (fstream::allbadbits);
    f.open (filename, fstream::out | fstream::trunc, mode);
    f.write (cdata(), readable_size());
    f.close();
}

/// swaps the contents with \p l
void cmemlink::swap (cmemlink& l)
{
    ::ustl::swap (m_CData, l.m_CData);
    ::ustl::swap (m_Size, l.m_Size);
}

/// Compares to memory block pointed by l. Size is compared first.
bool cmemlink::operator== (const cmemlink& l) const
{
    return (l.m_Size == m_Size &&
	    (l.m_CData == m_CData || 0 == memcmp (l.m_CData, m_CData, m_Size)));
}

} // namespace ustl

