#include <upair.h>
#include <ualgo.h>
#include <umap.h>
#include <ustring.h>
#include <fdostream.h>
#include <uios.h>
using namespace ustl;

int main (void)
{
    map<string,int> months;
    months["january"] = 31;
    months["february"] = 28;
    months["march"] = 31;
    months["april"] = 30;
    months["may"] = 31;
    months["june"] = 30;
    months["july"] = 31;
    months["august"] = 31;
    months["september"] = 30;
    months["october"] = 31;
    months["november"] = 30;
    months["december"] = 31;

    const map<string,int>& cmonths = months;
    cout << "There are " << cmonths["january"] << " days in january." << endl;
    cout << "There are " << cmonths["september"] << " days in september." << endl;
    cout << "There are " << cmonths["december"] << " days in december." << endl;
    map<string,int>::const_iterator found_may = months.find ("may");
    cout << found_may->first << " found at index " << found_may - months.begin() << endl;
    cout << "Alphabetical listing:" << endl;

    map<string,int>::const_iterator i;
    for (i = months.begin(); i < months.end(); ++ i)
	cout << i->first << " has " << i->second << " days." << endl;

    map<string,int> mcopy (months);
    mcopy.erase ("may");
    cout << "After erasing may:" << endl;
    for (i = mcopy.begin(); i < mcopy.end(); ++ i)
	cout << i->first << " ";
    cout << endl;

    mcopy.assign (months.begin(), months.end() - 1);
    mcopy.erase (mcopy.begin() + 1, mcopy.begin() + 4);
    cout << "After erasing months 2, 3, 4, and the last one:" << endl;
    for (i = mcopy.begin(); i < mcopy.end(); ++ i)
	cout << i->first << " ";
    cout << endl;

    mcopy = months;
    map<string,int>::iterator frob = mcopy.insert (make_pair (string("frobuary"), 42));
    cout << "After inserting " << frob->first << "," << frob->second << ":" << endl;
    for (i = mcopy.begin(); i < mcopy.end(); ++ i)
	cout << i->first << " ";
    cout << endl;

    return (0);
}

