// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//

#include "stdtest.h"

void Widen (const string& str, vector<wchar_t>& result)
{
    result.clear();
    result.resize (str.length());
    copy (str.utf8_begin(), str.utf8_end(), result.begin());
}

void DumpWchars (const vector<wchar_t>& v)
{
    foreach (vector<wchar_t>::const_iterator, i, v)
	cout << ' ' << uint32_t(*i);
}

void TestUTF8 (void)
{
    cout << "Generating Unicode characters ";
    vector<wchar_t> srcChars;
    srcChars.resize (0xFFFF);
    iota (srcChars.begin(), srcChars.end(), 0);
    cout << size_t(srcChars[0]) << " - " << size_t(srcChars.back()) << endl;

    cout << "Encoding to utf8." << endl;
    string encoded;
    encoded.reserve (srcChars.size() * 4);
    copy (srcChars, utf8out (back_inserter(encoded)));
    const char c_ProperEncoding[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    if (encoded.compare (encoded.begin(), encoded.begin() + VectorSize(c_ProperEncoding), VectorRange(c_ProperEncoding))) {
	cout << "Encoding failed: ";
	for (string::const_iterator i = encoded.begin(); i != encoded.begin() + VectorSize(c_ProperEncoding); ++ i)
	    cout << uint32_t(*i);
	cout << endl;
    }

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
	    cout << uint32_t(srcChars[i]) << " != " << uint32_t(decChars[i]) << endl;
	    ++ nDiffs;
	}
    }
    cout << nDiffs << " differences between src and decoded." << endl;

    cout << "Testing wide character string::insert" << endl;
    string ws ("1234567890", 10);

    ws.insert (0, wchar_t(1234));
    ws.insert (3, wchar_t(2345));
    const wchar_t c_WChars[2] = { 3456, 4567 };
    ws.insert (3, VectorRange(c_WChars), 2);
    ws.insert (ws.length(), wchar_t(5678));
    cout << "Values[" << ws.length() << "]:";
    for (string::utf8_iterator j = ws.utf8_begin(); j < ws.utf8_end(); ++ j)
	cout << ' ' << (uint32_t) *j;
    cout << endl;

    cout << "Character offsets:";
    for (string::utf8_iterator k = ws.utf8_begin(); k < ws.utf8_end(); ++ k)
	cout << ' ' << distance (ws.begin(), k.base());
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
}

StdBvtMain (TestUTF8)

