#include <mistream.h>
#include <mostream.h>
#include <strmsize.h>
#include <fdostream.h>
#include <uvector.h>
#include <ustack.h>
#include <uqueue.h>
#include <uspecial.h>
using namespace ustl;

int main (void)
{
    stack<vector<int> > s;
    cout << "Testing stack: ";
    for (size_t i = 0; i < 5; ++ i)
	s.push (1 + i);
    cout << "popping: ";
    for (size_t i = 0; i < 5; ++ i) {
	cout << s.top() << ' ';
	s.pop();
    }
    cout << endl;

    queue<vector<int> > q;
    cout << "Testing queue: ";
    for (size_t i = 0; i < 5; ++ i)
	q.push (1 + i);
    cout << "popping: ";
    for (size_t i = 0; i < 5; ++ i) {
	cout << q.front() << ' ';
	q.pop();
    }
    cout << endl;

    return (0);
}

