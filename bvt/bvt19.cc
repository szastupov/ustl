// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@talentg.com>
// This file is free software, distributed under the MIT License.
//

#include <ustl.h>
using namespace ustl;

int main (void)
{
    enum EFruit {
	apple,
	orange,
	plum,
	peach,
	pear,
	nectarine,
	NFruits
    };
    const char* fruits [NFruits + 1] = {
	"apple",
	"orange",
	"plum",
	"peach",
	"pear",
	"nectarine",
	"invalid"
    };
    cout << "Testing operator+" << endl;
    cout << "apple = " << fruits [apple] << endl;
    cout << "peach = " << fruits [apple + 3] << endl;

    return (EXIT_SUCCESS);
}

