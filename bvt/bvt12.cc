#include <ustl.h>
using namespace ustl;

int main (void)
{
    const void* pBufC;
    void* pBuf;
    memblock buffer;
    string testString ("TestString");
    const string* pStrC = NULL;
    string* pStr = NULL;
    vector<int> tv (6);

    const size_t bufSize = stream_size_of(pBufC) +
			   stream_size_of(pBuf) +
			   Align(stream_size_of(testString)) +
			   stream_size_of(pStrC) +
			   stream_size_of(pStr) +
			   stream_size_of(tv);
    cout << "Allocating " << bufSize << " bytes" << endl;
    buffer.resize (bufSize);
    pBufC = buffer.cdata();
    pBuf = buffer.data();

    ostream os (buffer);
    os << pBufC;
    cout << "Write const void*, pos = " << os.pos() << endl;
    os << pBuf;
    cout << "Write void*, pos = " << os.pos() << endl;
    os << testString;
    cout << "Write string, pos = " << os.pos() << endl;
    os.align();
    os << const_cast<const string*>(&testString);
    cout << "Write const string*, pos = " << os.pos() << endl;
    os << &testString;
    cout << "Write string*, pos = " << os.pos() << endl;
    os << tv;
    cout << "Write vector<int>(6), pos = " << os.pos() << endl;
    assert (os.pos() == bufSize);
    
    istream is (buffer);
    is >> pBufC;
    cout << "Read const void*, pos = " << is.pos();
    cout << ", value is " << (pBufC == buffer.cdata() ? "right" : "wrong") << endl;
    is >> pBuf;
    cout << "Read void*, pos = " << is.pos();
    cout << ", value is " << (pBuf == buffer.cdata() ? "right" : "wrong") << endl;
    testString.clear();
    is >> testString;
    cout << "Read string, pos = " << is.pos() << ", value is " << testString << endl;
    is.align();
    is >> pStrC;
    cout << "Read const string*, pos = " << is.pos();
    cout << ", value is " << (pStrC == &testString ? "right" : "wrong") << endl;
    is >> pStr;
    cout << "Read string*, pos = " << is.pos();
    cout << ", value is " << (pStr == &testString ? "right" : "wrong") << endl;
    vector<int> rv;
    is >> rv;
    cout << "Read vector<int>(" << rv.size() << "), pos = " << is.pos();
    cout << ", value is " << (rv == tv ? "right" : "wrong") << endl;
    assert (is.pos() == bufSize);

    return (0);
}

