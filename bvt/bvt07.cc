#include <ustring.h>
#include <fdostream.h>
#include <mistream.h>
#include <uios.h>
using namespace ustl;

int main (void)
{
    const char c_TestString1[] = "123456789012345678901234567890";
    const char c_TestString2[] = "abcdefghijklmnopqrstuvwxyz";
    const char c_TestString3[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string s1 (c_TestString1);
    string s2 (c_TestString2, VectorSize (c_TestString2));
    string s3 (s1);

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    s3.reserve (48);
    s3.resize (20);

    for (uoff_t i = 0; i < s3.length(); ++ i)
	s3.at(i) = s3.at(i);
    for (uoff_t i = 0; i < s3.length(); ++ i)
	s3[i] = s3[i];
    cout << s3 << endl;
    cout << "s3.size() = " << s3.size();
    cout << ", max_size() = " << s3.max_size();
    cout << ", capacity() = " << s3.capacity() << endl;

    s1.unlink();
    s1 = c_TestString2;
    s1 += c_TestString3;
    s1 += '$';
    cout << s1 << endl;

    s1 = "Hello";
    s2.unlink();
    s2 = "World";
    s3 = s1 + s2;
    cout << s3 << endl;
    s3 = "Concatenated ";
    s3 += s1.c_str();
    s3 += s2;
    s3 += " string.";
    cout << s3 << endl;

    if (s1 < s2)
	cout << "s1 < s2" << endl;
    if (s1 == s1)
	cout << "s1 == s1" << endl;

    string s4;
    s4.link (s1);
    if (s1 == s4)
	cout << "s1 == s4" << endl;

    s1 = c_TestString1;
    string s5 (s1.begin() + 4, s1.begin() + 4 + 5);
    string s6 (s1.begin() + 4, s1.begin() + 4 + 5);
    if (s5 == s6)
	cout << s5 << " == " << s6 << endl;
    string tail (s1.begin() + 7, s1.end());
    cout << "&s1[7] = " << tail << endl;

    cout << s1.data() << endl;
    cout << "erase(5,5)" << endl;
    s1.erase (s1.begin() + 5, 5);
    cout << s1 << endl;
    cout << "insert(10,#)" << endl;
    s1.insert (s1.begin() + 10, '#');
    cout << s1 << endl;
    cout << "replace(0,5,@)" << endl;
    s1.replace (s1.begin(), s1.begin() + 5, '@');
    cout << s1 << endl;

    s1 = c_TestString1;
    cout << "8 found at " << s1.find ('8') - s1.begin() << endl;
    cout << "9 found at " << s1.find ("9") - s1.begin() << endl;
    string::const_iterator poundfound = s1.find ("#");
    if (poundfound != s1.end())
	cout << "# found at " << poundfound - s1.begin() << endl;
    cout << "[456] found at " << s1.find_first_of ("456") - s1.begin() << endl;
    cout << "[456] last found at " << s1.find_last_of ("456") - s1.begin() << endl;

    s2.clear();
    if (s2.empty())
	cout << "s2 is empty [" << s2 << "], capacity " << s2.capacity() << " bytes" << endl;

    s2.format ("<const] %d, %s, 0x%08X", 42, "[rfile>", 0xDEADBEEF);
    cout << "<" << s2.length() << " bytes of " << s2.capacity();
    cout << "> Format '" << s2 << '\''<< endl;

    return (0);
}
