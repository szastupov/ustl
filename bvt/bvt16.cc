#include <ustring.h>
#include <uvector.h>
#include <fdostream.h>
#include <uios.h>
#include <utf8.h>
#include <uctralgo.h>
using namespace ustl;

template <typename T>
class Linear {
public:
    		Linear (T initialValue) : m_Value (initialValue) {}
    T		operator()(void) { return (m_Value++); }
private:
    T		m_Value;
};

int main (void)
{
    cout << "Generating Unicode characters ";
    vector<wchar_t> srcChars;
    srcChars.resize (0xFFFF);
    generate (srcChars, Linear<wchar_t>(0));
    cout << srcChars[0] << " - " << srcChars.back() << endl;

    cout << "Encoding to utf8." << endl;
    string encoded;
    encoded.reserve (srcChars.size() * 4);
    copy (srcChars, utf8out (back_inserter(encoded)));

    cout << "Decoding back." << endl;
    vector<wchar_t> decChars;
    decChars.reserve (srcChars.size());
    copy (utf8in (encoded.begin()), utf8in (encoded.end()), back_inserter(decChars));

    cout << "Comparing." << endl;
    cout << "src = " << srcChars.size();
    cout << " chars, encoded = " << encoded.size();
    cout << " chars, decoded = " << decChars.size() << endl;
    size_t nDiffs = 0;
    for (uoff_t i = 0; i < min (srcChars.size(), decChars.size()); ++ i) {
	if (srcChars[i] != decChars[i]) {
	    cout << srcChars[i] << " != " << decChars[i] << endl;
	    ++ nDiffs;
	}
    }
    cout << nDiffs << " differences between src and decoded." << endl;
    return (0);
}

