#include <mistream.h>
#include <mostream.h>
#include <strmsize.h>
#include <fdostream.h>
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

    return (0);
}

