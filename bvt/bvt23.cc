// Tests SIMD algorithms and specializations.
//

#include <ustl.h>
using namespace ustl;
using namespace ustl::simd;

template <typename Ctr>
void TestBitwiseOperations (Ctr op1, Ctr op2, const Ctr op3)
{
    passign (op3, op2);
    pand (op1, op2);
    cout << "pand(op1,op2) = " << op2 << endl;
    passign (op3, op2);
    por (op1, op2);
    cout << "por(op1,op2) = " << op2 << endl;
    passign (op3, op2);
    pxor (op1, op2);
    cout << "pxor(op1,op2) = " << op2 << endl;
    passign (op3, op2);
    pshl (op1, op2);
    cout << "pshl(op1,op2) = " << op2 << endl;
    passign (op3, op2);
    pshr (op1, op2);
    cout << "pshr(op1,op2) = " << op2 << endl;
}

template <> inline void TestBitwiseOperations (tuple<2,float>, tuple<2,float>, const tuple<2,float>) {}
template <> inline void TestBitwiseOperations (tuple<4,float>, tuple<4,float>, const tuple<4,float>) {}

template <typename Ctr>
void TestCtr (const char* ctrType)
{
    cout << "================================================" << endl;
    cout << "Testing " << ctrType << endl;
    cout << "================================================" << endl;
    Ctr op1, op2, op3;
    fill (op1, 2);
    iota (op2.begin(), op2.end(), 1);
    cout << "op1 = " << op1 << endl;
    cout << "op2 = " << op2 << endl;
    passign (op2, op3);
    cout << "passign(op2,op3) = " << op3 << endl;
    padd (op1, op2);
    cout << "padd(op1,op2) = " << op2 << endl;
    psub (op1, op2);
    cout << "psub(op1,op2) = " << op2 << endl;
    pmul (op1, op2);
    cout << "pmul(op1,op2) = " << op2 << endl;
    pdiv (op1, op2);
    cout << "pdiv(op1,op2) = " << op2 << endl;
    TestBitwiseOperations (op1, op2, op3);
    passign (op3, op2);
    reverse (op2);
    pmin (op3, op2);
    cout << "pmin(op3,op2) = " << op2 << endl;
    passign (op3, op2);
    reverse (op2);
    pmax (op3, op2);
    cout << "pmax(op3,op2) = " << op2 << endl;
    passign (op3, op2);
    reverse (op2);
    pavg (op3, op2);
    cout << "pavg(op3,op2) = " << op2 << endl;
    reset_mmx();
}

int main (void)
{
    TestCtr<tuple<8,uint8_t> >("uint8_t[8]");
    TestCtr<tuple<8,int8_t> >("int8_t[8]");
    TestCtr<tuple<4,uint16_t> >("uint16_t[4]");
    TestCtr<tuple<4,int16_t> >("int16_t[4]");
    TestCtr<tuple<2,uint32_t> >("uint32_t[2]");
    TestCtr<tuple<2,int32_t> >("int32_t[2]");
    #if HAVE_INT64_T
	TestCtr<tuple<1,uint64_t> >("uint64_t[1]");
	TestCtr<tuple<1,int64_t> >("int64_t[1]");
    #else
	cout << "CAN'T TEST: 64 bit types are not available on this platform." << endl;
    #endif
    TestCtr<tuple<2,float> >("float[2]");
    TestCtr<tuple<4,float> >("float[4]");
    return (0);
}

