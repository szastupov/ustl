#include <fdostream.h>
#include <mistream.h>
#include <uios.h>
#include <ustring.h>
using namespace ustl;

int main (void)
{
    cout << "A string printed to stdout" << endl;
    cout.format ("%d %s: %ld, %hd, 0x%08X\n", 3, "numbers", 12345678L, 4321, 0x78675645);
    string testString;
    testString.format ("A ustl::string object printed %d times\n", 10);
    for (int i = 0; i < 10; ++ i)
	cout << testString;
    cout.flush();
    cerr << "All done." << endl;
    return (0);
}

