#include <ualgo.h>
#include <umultimap.h>
#include <ustring.h>
#include <fdostream.h>
#include <uios.h>
using namespace ustl;

int main (void)
{
    multimap<int,string> employees;
    employees.insert (make_pair (27000, string("Dave"))); 
    employees.insert (make_pair (27000, string("Jim"))); 
    employees.insert (make_pair (99000, string("BigBoss"))); 
    employees.insert (make_pair (47000, string("Gail"))); 
    employees.insert (make_pair (15000, string("Dumb"))); 
    employees.insert (make_pair (47000, string("Mary"))); 
    employees.insert (make_pair (47000, string("Barbara"))); 

    typedef multimap<int,string>::const_iterator miter_t;
    pair<miter_t,miter_t> middles = const_cast<const multimap<int,string>&>(employees).equal_range (47000);
    cout << "Employees making $" << middles.first->first << ":";
    miter_t i;
    for (i = middles.first; i < middles.second; ++ i)
	cout << " " << i->second;
    cout << endl;

    cout << "There are " << employees.count (27000) << " low-paid employees" << endl;

    cout << "Alphabetical listing:" << endl;
    for (i = employees.begin(); i < employees.end(); ++ i)
	cout << i->second << "\t- $" << i->first << endl;

    cout << "Firing all low-paid employees:" << endl;
    employees.erase (27000);
    for (i = employees.begin(); i < employees.end(); ++ i)
	cout << i->second << "\t- $" << i->first << endl;

    cout << "Firing dumb employees:" << endl;
    employees.erase (employees.begin(), employees.begin() + 1);
    for (i = employees.begin(); i < employees.end(); ++ i)
	cout << i->second << "\t- $" << i->first << endl;

    return (0);
}

