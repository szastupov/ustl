#include <ustl.h>
using namespace ustl;

template <typename T>
class Linear {
public:
    		Linear (T initialValue) : m_Value (initialValue) {}
    T		operator()(void) { return (m_Value++); }
private:
    T		m_Value;
};

void Widen (const string& str, vector<wchar_t>& result)
{
    result.clear();
    result.resize (str.length());
    copy (utf8in (str.begin()), utf8in (str.end()), result.begin());
}

void DumpWchars (const vector<wchar_t>& v)
{
    foreach (vector<wchar_t>::const_iterator, i, v)
	cout << ' ' << (uint32_t) *i;
}

int main (void)
{
    cout << "Generating Unicode characters ";
    vector<wchar_t> srcChars;
    srcChars.resize (0xFFFF);
    generate (srcChars, Linear<wchar_t>(0));
    cout << size_t(srcChars[0]) << " - " << size_t(srcChars.back()) << endl;

    cout << "Encoding to utf8." << endl;
    string encoded;
    encoded.reserve (srcChars.size() * 4);
    copy (srcChars, utf8out (back_inserter(encoded)));

    cout << "Decoding back." << endl;
    vector<wchar_t> decChars;
    Widen (encoded, decChars);

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

    cout << "Testing wide character string::insert" << endl;
    string ws ("1234567890", 10);

    ws.insert (0, wchar_t(1234));
    ws.insert (3, wchar_t(2345));
    ws.insert (4, wchar_t(3456));
    ws.insert (ws.length(), wchar_t(4567));
    cout << "Values[" << ws.length() << "]:";
    for (uoff_t j = 0; j < ws.length(); ++ j)
	cout << ' ' << (uint32_t) ws.char_at(j);
    cout << endl;

    cout << "Character offsets:";
    for (size_t k = 0; k < ws.length(); ++ k)
	cout << ' ' << distance (ws.begin(), ws.ichar(k));
    cout << endl;

    cout << "Erasing character " << ws.length() - 1 << ": ";
    ws.erase (ws.length() - 1);
    Widen (ws, decChars);
    DumpWchars (decChars);
    cout << endl;

    cout << "Erasing characters 3-5: ";
    ws.erase (3, 2);
    Widen (ws, decChars);
    DumpWchars (decChars);
    cout << endl;

    return (0);
}

