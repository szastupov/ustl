#include <ustl.h>
using namespace ustl;

int main (void)
{
    int pt1v[4] = { 1, 2, 3, 4 };
    tuple<4,int> pt1 (pt1v);
    tuple<4,u_int> pt2 (5, 6, 7, 8);

    cout << "pt1: size = " << pt1.size() << ", value = " << pt1 << endl;
    cout << "pt2: " << pt2 << endl;
    iota (pt2.begin(), pt2.end(), 10);
    cout << "pt2: " << pt2 << endl;

    pt1 += pt2;
    cout << "pt1: " << pt1 << endl;
    pt1 -= pt2;
    cout << "pt1: " << pt1 << endl;
    pt1 *= 2;
    cout << "pt1: " << pt1 << endl;
    pt1 /= 3;
    cout << "pt1: " << pt1 << endl;
    pt1 = pt2 + pt2;
    cout << "pt1: " << pt1 << endl;

    tuple<3, string> strv;
    strv[0] = "str0";
    strv[1] = "str1";
    strv[2] = "str2";
    cout << "str: " << strv << endl;

    return (0);
}

