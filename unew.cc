// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//
// unew.cc
//

#include "unew.h"
#include <stdlib.h>

void* operator new (size_t n) throw (ustl::bad_alloc)
{
    void* p = malloc (n);
    if (!p)
	throw ustl::bad_alloc (n);
    return (p);
}

void* operator new[] (size_t n) throw (ustl::bad_alloc)
{
    void* p = malloc (n);
    if (!p)
	throw ustl::bad_alloc (n);
    return (p);
}

void operator delete (void* p) throw()
{
    if (p)
	free (p);
}

void operator delete[] (void* p) throw()
{
    if (p)
	free (p);
}


