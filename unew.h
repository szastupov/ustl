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
// unew.h
//
//	Same as <new> to avoid including it and to throw correct exceptions.
//

#ifndef UNEW_H_11D237512B324C9C05A55DAF1BF086F1
#define UNEW_H_11D237512B324C9C05A55DAF1BF086F1

#include "uexception.h"

//
// These are replaceable signatures:
//  - normal single new and delete (no arguments, throw @c bad_alloc on error)
//  - normal array new and delete (same)
//  - @c nothrow single new and delete (take a @c nothrow argument, return
//    @c NULL on error)
//  - @c nothrow array new and delete (same)
//
//  Placement new and delete signatures (take a memory address argument,
//  does nothing) may not be replaced by a user's program.
//
void* operator new (size_t) throw (ustl::bad_alloc);
void* operator new[] (size_t) throw (ustl::bad_alloc);
void  operator delete (void*) throw();
void  operator delete[] (void*) throw();

// Default placement versions of operator new.
inline void* operator new (size_t, void* p) throw() { return (p); }
inline void* operator new[] (size_t, void* p) throw() { return (p); }

// Default placement versions of operator delete.
inline void  operator delete  (void*, void*) throw() { }
inline void  operator delete[](void*, void*) throw() { }

#endif

