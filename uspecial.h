// uspecial.h
//
// 	Template specializations for uSTL classes.
//

#ifndef USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0
#define USPECIAL_H_947ADYOU0ARE3YOU2REALLY8ARE44CE0

#include "uvector.h"
#include "ustring.h"
#include "uset.h"
#include "umultiset.h"

namespace ustl {

template <>
inline void swap (cmemlink& a, cmemlink& b)
{
    a.swap (b);
}

template <> 
inline void swap (memlink& a, memlink& b)
{
    a.swap (b);
}

template <> 
inline void swap (memblock& a, memblock& b)
{
    a.swap (b);
}

template <> 
inline void swap (string& a, string& b)
{
    a.swap (b);
}

template <typename T> 
inline void swap (vector<T>& a, vector<T>& b)
{
    a.swap (b);
}

template <typename T>
inline void swap (set<T>& a, set<T>& b)
{
    a.swap (b);
}

template <typename T>
inline void swap (multiset<T>& a, multiset<T>& b)
{
    a.swap (b);
}

} // namespace ustl

#endif

