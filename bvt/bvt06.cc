#include <ustl.h>
using namespace ustl;

void PrintBlock (const cmemlink& l)
{
    const int* numbers = reinterpret_cast<const int*>(l.begin());
    const size_t nNumbers = l.size() / sizeof(int);
    for (size_t i = 0; i < nNumbers; ++ i)
	cout << numbers[i] << ' ';
    cout << endl;
}

int main (void)
{
    vector<memblock> v;
    const size_t nNumbers = 1000;
    int numbers [nNumbers];
    const size_t nLinks = 10;
    cmemlink links [nLinks];
    for (size_t i = 0; i < nNumbers; ++ i)
	numbers[i] = i;
    uoff_t offset = 0;
    for (size_t l = 0; l < nLinks; ++ l) {
	links[l].link (numbers + offset, l * sizeof(int));
	offset += l;
	v.push_back (memblock(links[l]));
    }
    cout << "---" << endl;
    cout << "vector<memblock> of " << v.size() << " elements:" << endl;
    cout << "---" << endl;
    for_each (v.begin(), v.end(), &PrintBlock);
    cout << "---" << endl;
    cout << "size() = " << v.size();
    cout << ", max_size() = " << v.max_size();
    cout << ", empty() = " << (v.empty() ? "true" : "false") << endl;
    v.push_back (memblock(5));
    cout << "back()->size() = " << v.back().size() << endl;
    v.back().resize (40);
    cout << "back()->size() = " << v.back().size() << endl;
    v.pop_back();
    PrintBlock (v.back());
    vector<memblock> cache;
    cache.assign (v.begin(), v.end());
    v.clear();
    v.assign (cache.begin(), cache.end());
    v.erase (v.begin() + 5, 2);
    v.erase (v.end() - 1, 1);
    v.erase (v.end(), size_t(0));
    cout << "---" << endl;
    cout << "vector of " << v.size() << " elements backwards:" << endl;
    cout << "---" << endl;
    for_each (v.rbegin(), v.rend(), &PrintBlock);
    cout << "---" << endl;
    return (0);
}

