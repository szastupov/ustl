// "Testing string reads" 12345678 4321 0x78675645 1.234567890123456

#include <ustl.h>
using namespace ustl;

int main (void)
{
    string testStr;
    cin >> testStr;
    if (testStr != "//") {
	cout << "You must put bvt13.cc on stdin (read " << testStr << ")" << endl;
	return (-1);
    }
    uint32_t n1 = 0, n3 = 0;
    uint16_t n2 = 0;
    double f1 = 0.0;
    cin >> testStr >> n1 >> n2 >> n3 >> f1;
    cout << testStr << endl;
    cout << "A string printed to stdout" << endl;
    cout.format ("%d %s: %d, %hd, 0x%08X, %1.15f\n", 4, "numbers", n1, n2, n3, f1);
    string testString;
    testString.format ("A ustl::string object printed %d times\n", 3);
    for (int i = 0; i < 3; ++ i)
	cout << testString;
    cout.flush();
    cerr << "All done." << endl;
    return (0);
}

