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
// ustl.h
//
/// \mainpage
///
/// \section intro Introduction
///
/// uSTL is a partial implementation of the STL specification intended to
/// reduce code size of the derivative programs. Usually, the STL containers
/// manage their own storage with new[] and delete[] operators, which create
/// strongly typed storage. That is the standard way of allocating C++ object
/// vectors, allowing appropriate constructors and destructors to be called
/// on the allocated storage and ensuring that objects are copied via their
/// copy operators. Although type safety is a good thing, placing memory
/// management code into a template necessitates its reinstantiation for
/// every template instance used by the derivative program. This produces
/// substantial code bloat, that is frequently derided by C developers
/// and used by them as an argument that C is better than C++. The uSTL
/// implementation attempts to work around this problem by factoring
/// memory management code into a non-template base class, ustl::memblock,
/// which performs unstructured memory allocation. STL containers are then
/// implemented as template wrappers for memblock to provide a measure of
/// type safety. The result is that each template instantiation contains
/// less code, and although it does not completely "disappear", due to the
/// requirement for calling placement constructors on the allocated memory,
/// most of it does, being replaced by calls to memblock methods. A vector<T>
/// template instance, for instance, implements virtuals constructBlock
/// (simply a call to placement new[]) and destructBlock (iterates over all
/// elements to manually call the destructor) and two algorithms copy and
/// fill (which most STL programs use anyway); everything else melts away at
/// compile time as if you were using only unstructured storage in the first
/// place. ustl::string is implemented as a static class, not a template,
/// so it is shared among all users of the library. The base classes for
/// unstructured storage management (cmemlink - link to constant memory,
/// memlink - link to mutable memory, and memblock - owner of mutable memory)
/// are, of course, also available for use as data buffers wherever those are
/// needed, and streams that efficiently read and write binary data into them
/// are also available.
///
/// \section Differences
///
/// The main difference you will notice when using uSTL, is that objects in
/// containers may sometimes undergo bitwise copy. To ensure that this does
/// not harm your objects you need to design them not to contain any pointers
/// to data inside themselves or other objects. I have not encountered any
/// situation when such pointers were needed, but if you have one and do not
/// know how to work around the limitation, I would be happy to give it a try.
///
/// \section install Installation
///
/// You should have received a copy of the configure script with your distribution.
/// Then run \c configure with parameters appropriate for your system (usually none).
/// Finally, run \c make and \c make \c install to compile the shared library and
/// install it, along with necessary headers into system-standard directories (see
/// \c LIBDIR and \c INCDIR in Common.mk for values). To use the library, include
/// \c ustl.h and link to \c -lustl . Tutorials on using STL are numerous on the
/// web and the information in there mostly applies, except for iostream usage,
/// which is not supported in uSTL for philosophical reasons.
///

#ifndef USTL_H_99881273
#define USTL_H_99881273

#include "ustl/uspecial.h"
#include "ustl/ulimits.h"
#include "ustl/sistream.h"
#include "ustl/uiosfunc.h"
#include "ustl/uctralgo.h"
#include "ustl/utf8.h"
#include "ustl/umap.h"
#include "ustl/umultimap.h"
#include "ustl/ustack.h"
#ifndef WITHOUT_CIN_COUT_CERR
    #include "ustl/fdostream.h"
#else
    #include "ustl/sostream.h"
#endif
#include "ustl/unumeric.h"

#endif

