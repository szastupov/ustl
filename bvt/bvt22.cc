// Tests matrix operations
//

#include <ustl.h>
using namespace ustl;

template <size_t NX, size_t NY, typename T>
void TestMatrix (void)
{
    matrix<NX,NY,T> m1, m2;
    load_identity (m1);
    cout << "load_identity(m1)" << endl;
    cout << "    m1 = " << m1 << endl;
    m2 = m1;
    cout << "m1 = m2" << endl;
    cout << "    m2 = " << m2 << endl;
    m1 += m2;
    cout << "m1 += m2" << endl;
    cout << "    m1 = " << m1 << endl;
    m1 /= 2;
    cout << "m1 /= 2" << endl;
    cout << "    m1 = " << m1 << endl;
    m1 = m1 * m2;
    cout << "m1 = m1 * m2" << endl;
    cout << "    m1 = " << m1 << endl;
    m1 += 3;
    cout << "m1 += 3" << endl;
    cout << "    m1 = " << m1 << endl;
    load_identity (m2);
    m2 *= 2;
    m1 = m1 * m2;
    cout << "m1 *= I(2)" << endl;
    cout << "    m1 = " << m1 << endl;
    iota (m1.begin(), m1.end(), 1);
    cout << "m1 = iota(1)" << endl;
    cout << "    m1 = " << m1 << endl;
    cout << "    m1 row [1] = " << m1.row(1) << endl;
    cout << "    m1 column [2] = " << m1.column(2) << endl;
    m1 = m1 * m2;
    cout << "m1 *= I(2)" << endl;
    cout << "    m1 = " << m1 << endl;
    typename matrix<NX,NY,T>::column_type v, vt;
    iota (v.begin(), v.end(), 1);
    cout << "v = iota(1)" << endl;
    cout << "    v = " << v << endl;
    load_identity (m2);
    m2 *= 2;
    for (uoff_t y = 0; y < NY - 1; ++ y)
	m2[NY - 1][y] = 1;
    cout << "m2 = I(2) + T(1)" << endl;
    cout << "    m2 = " << m2 << endl;
    vt = v * m2;
    cout << "vt = v * m2" << endl;
    cout << "    vt = " << vt << endl;
}

int main (void)
{
    cout << "========================================" << endl;
    cout << "Testing 4x4 int matrix:" << endl;
    cout << "========================================" << endl;
    TestMatrix<4,4,int>();
    cout << "========================================" << endl;
    cout << "Testing 4x4 float matrix:" << endl;
    cout << "========================================" << endl;
    cout.set_precision (1);
    TestMatrix<4,4,float>();
    return (EXIT_SUCCESS);
}

