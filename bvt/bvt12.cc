#include <ustring.h>
#include <mistream.h>
#include <mostream.h>
#include <strmsize.h>
#include <fdostream.h>
#include <mistream.h>
#include <uios.h>
using namespace ustl;

int main (void)
{
    memblock buffer;
    const void* pBufC = buffer.cdata();
    void* pBuf = buffer.data();
    string testString ("TestString");
    const string* pStrC = NULL;
    string* pStr = NULL;

    const size_t bufSize = stream_size_of(pBufC) +
			   stream_size_of(pBuf) +
			   Align(stream_size_of(testString)) +
			   stream_size_of(pStrC) +
			   stream_size_of(pStr);
    cout << "Allocating " << bufSize << " bytes" << endl;
    buffer.resize (bufSize);

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
    assert (is.pos() == bufSize);

    return (0);
}

