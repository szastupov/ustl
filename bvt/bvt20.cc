#include <ustl.h>
using namespace ustl;

int main (void)
{
    stack<vector<int> > s;
    cout << "Testing stack: ";
    for (size_t i = 0; i < 5; ++ i)
	s.push (1 + i);
    cout << "popping: ";
    for (size_t j = 0; j < 5; ++ j) {
	cout << s.top() << ' ';
	s.pop();
    }
    cout << endl;

    queue<vector<int> > q;
    cout << "Testing queue: ";
    for (size_t k = 0; k < 5; ++ k)
	q.push (1 + k);
    cout << "popping: ";
    for (size_t l = 0; l < 5; ++ l) {
	cout << q.front() << ' ';
	q.pop();
    }
    cout << endl;

    return (0);
}

