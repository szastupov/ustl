// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
/// \file ualgo.h
///
/// \brief Implementation of STL algorithms.
///
/// The function prototypes are copied
/// exactly from the SGI version of STL documentation along with comments about
/// their use. The code is NOT the same, though the functionality usually is.
///

#ifndef UALGO_H
#define UALGO_H

#include "upair.h"

namespace ustl {

///
/// Assigns the contents of a to b and the contents of b to a.
/// This is used as a primitive operation by many other algorithms. 
///
template <class Assignable> 
inline void swap (Assignable& a, Assignable& b)
{
    Assignable tmp = a;
    a = b;
    b = tmp;
}

///
/// Copy copies elements from the range [first, last) to the range
/// [result, result + (last - first)). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
/// for every integer n from 0 to last - first, copy performs the assignment
/// *(result + n) = *(first + n). Assignments are performed in forward order,
/// i.e. in order of increasing n. 
///
template <class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
    while (first < last)
	*result++ = *first++;
    return (result);
}

///
/// Copy_n copies elements from the range [first, first + n) to the range
/// [result, result + n). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
/// for every integer i from 0 up to (but not including) n, copy_n performs
/// the assignment *(result + i) = *(first + i). Assignments are performed
/// in forward order, i.e. in order of increasing n.
///
template <class InputIterator, class OutputIterator>
OutputIterator copy_n (InputIterator first, size_t count, OutputIterator result)
{
    while (count--)
	*result++ = *first++;
    return (result);
}

///
/// Copy_if copies elements from the range [first, last) to the range
/// [result, result + (last - first)) if pred(*i) returns true.
///
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    while (first < last) {
	if (pred(*first))
	    *result++ = *first;
	++ first;
    }
    return (result);
}

///
/// For_each applies the function object f to each element in the range
/// [first, last); f's return value, if any, is ignored. Applications are
/// performed in forward order, i.e. from first to last. For_each returns
/// the function object after it has been applied to each element.
///
template <class InputIterator, class UnaryFunction>
UnaryFunction for_each (InputIterator first, InputIterator last, UnaryFunction f)
{
    while (first < last)
	f (*first++);
    return (f);
}

///
/// Returns the first iterator i in the range [first, last) such that
/// *i == value. Returns last if no such iterator exists. 
///
template <class InputIterator, class EqualityComparable>
InputIterator find (InputIterator first, InputIterator last, const EqualityComparable& value)
{
    while (first < last && !(*first == value))
	++ first;
    return (first);
}

///
/// Returns the first iterator i in the range [first, last) such that
/// pred(*i) is true. Returns last if no such iterator exists.
///
template <class InputIterator, class Predicate>
InputIterator find_if (InputIterator first, InputIterator last, Predicate pred)
{
    while (first < last && !pred (*first))
	++ first;
    return (first);
}

///
/// Count finds the number of elements in [first, last) that are equal
/// to value. More precisely, the first version of count returns the
/// number of iterators i in [first, last) such that *i == value.
///
template <class InputIterator, class EqualityComparable>
size_t count (InputIterator first, InputIterator last, const EqualityComparable& value)
{
    size_t total = 0;
    while (first < last)
	if (*first++ == value)
	    ++ total;
    return (total);
}

///
/// Count_if finds the number of elements in [first, last) that satisfy the
/// predicate pred. More precisely, the first version of count_if returns the
/// number of iterators i in [first, last) such that pred(*i) is true.
///
template <class InputIterator, class Predicate>
size_t count_if (InputIterator first, InputIterator last, Predicate pred)
{
    size_t total = 0;
    while (first < last)
	if (pred (*first++))
	    ++ total;
    return (total);
}

///
/// The first version of transform performs the operation op(*i) for each
/// iterator i in the range [first, last), and assigns the result of that
/// operation to *o, where o is the corresponding output iterator. That is,
/// for each n such that 0 <= n < last - first, it performs the assignment
/// *(result + n) = op(*(first + n)).
/// The return value is result + (last - first).
///
template <class InputIterator, class OutputIterator, class UnaryFunction>
OutputIterator transform (InputIterator first, InputIterator last, OutputIterator result, UnaryFunction op)
{
    while (first < last)
	*result++ = op (*first++);
    return (result);
}

///
/// The second version of transform is very similar, except that it uses a
/// Binary Function instead of a Unary Function: it performs the operation
/// op(*i1, *i2) for each iterator i1 in the range [first1, last1) and assigns
/// the result to *o, where i2 is the corresponding iterator in the second
/// input range and where o is the corresponding output iterator. That is,
/// for each n such that 0 <= n < last1 - first1, it performs the assignment
/// *(result + n) = op(*(first1 + n), *(first2 + n).
/// The return value is result + (last1 - first1).
///
template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryFunction>
OutputIterator transform (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op)
{
    while (first1 < last1)
	*result++ = op (*first1++, *first2++);
    return (result);
}

///
/// Replace replaces every element in the range [first, last) equal to
/// old_value with new_value. That is: for every iterator i,
/// if *i == old_value then it performs the assignment *i = new_value.
///
template <class ForwardIterator, class T>
void replace (ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
{
    while (first < last) {
	if (*first == old_value)
	    *first = new_value;
	++ first;
    }
}

///
/// Replace_if replaces every element in the range [first, last) for which
/// pred returns true with new_value. That is: for every iterator i, if
/// pred(*i) is true then it performs the assignment *i = new_value.
///
template <class ForwardIterator, class Predicate, class T>
void replace_if (ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value)
{
    while (first < last) {
	if (pred (*first))
	    *first = new_value;
	++ first;
    }
}

///
/// Replace_copy copies elements from the range [first, last) to the range
/// [result, result + (last-first)), except that any element equal to old_value
/// is not copied; new_value is copied instead. More precisely, for every
/// integer n such that 0 <= n < last-first, replace_copy performs the
/// assignment *(result+n) = new_value if *(first+n) == old_value, and
/// *(result+n) = *(first+n) otherwise.
///
template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy (InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)
{
    while (first < last) {
        *result++ = (*first == old_value) ? new_value : *first;
	++ first;
    }
}

template <class InputIterator, class OutputIterator, class Predicate, class T>
///
/// Replace_copy_if copies elements from the range [first, last) to the range
/// [result, result + (last-first)), except that any element for which pred is
/// true is not copied; new_value is copied instead. More precisely, for every
/// integer n such that 0 <= n < last-first, replace_copy_if performs the
/// assignment *(result+n) = new_value if pred(*(first+n)),
/// and *(result+n) = *(first+n) otherwise.
///
OutputIterator replace_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) 
{
    while (first < last) {
        *result++ = pred(*first) ? new_value : *first;
	++ first;
    }
}

///
/// Fill assigns the value value to every element in the range [first, last).
/// That is, for every iterator i in [first, last),
/// it performs the assignment *i = value.
///
template <class ForwardIterator, class T>
void fill (ForwardIterator first, ForwardIterator last, const T& value)
{
    while (first < last)
	*first++ = value;
}

///
/// Fill_n assigns the value value to every element in the range
/// [first, first+n). That is, for every iterator i in [first, first+n),
/// it performs the assignment *i = value. The return value is first + n.
///
template <class OutputIterator, class T>
OutputIterator fill_n (OutputIterator first, size_t n, const T& value)
{
    while (n--)
	*first++ = value;
    return (first);
}

///
/// Generate assigns the result of invoking gen, a function object that
/// takes no arguments, to each element in the range [first, last).
///
template <class ForwardIterator, class Generator>
void generate (ForwardIterator first, ForwardIterator last, Generator gen)
{
    while (first < last)
	*first++ = gen();
}

///
/// Generate_n assigns the result of invoking gen, a function object that
/// takes no arguments, to each element in the range [first, first+n).
/// The return value is first + n.
///
template <class OutputIterator, class Generator>
OutputIterator generate_n (OutputIterator first, size_t n, Generator gen)
{
    while (n--)
	*first++ = gen();
    return (first);
}

///
/// Exchanges ranges [first, middle) and [middle, last)
///
template <class ForwardIterator>
ForwardIterator rotate (ForwardIterator first, ForwardIterator middle, ForwardIterator last)
{
    reverse (first, middle);
    reverse (middle, last);
    while (first < middle && middle < last)
	swap (*first++, *--last);
    if (first == middle)
	reverse (middle, last);
    else
	reverse (first, middle);
    return (first);
}

///
/// Combines two sorted ranges.
///
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
		      InputIterator2 first2, InputIterator2 last2, OutputIterator result)
{
    while (first1 < last1 && first2 < last2) {
	if (*first1 < *first2)
	    *result++ = *first1++;
	else
	    *result++ = *first2++;
    }
    if (first1 < last1)
	return (copy (first1, last1, result));
    else
	return (copy (first2, last2, result));
}

///
/// Combines two sorted ranges from the same container.
///
template <class InputIterator>
void inplace_merge (InputIterator first, InputIterator middle, InputIterator last)
{
    while (middle < last) {
	while (*first < *middle)
	    ++ first;
	reverse (first, middle);
	reverse (first++, ++middle);
    }
}

///
/// Remove_copy copies elements that are not equal to value from the range
/// [first, last) to a range beginning at result. The return value is the
/// end of the resulting range. This operation is stable, meaning that the
/// relative order of the elements that are copied is the same as in the
/// range [first, last).
///
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy (InputIterator first, InputIterator last, OutputIterator result, const T& value)
{
    while (first < last) {
	T cur_value = *first++;
	if (cur_value != value)
	    *result++ = cur_value;
    }
    return (result);
}

///
/// Remove_copy_if copies elements from the range [first, last) to a range
/// beginning at result, except that elements for which pred is true are not
/// copied. The return value is the end of the resulting range. This operation
/// is stable, meaning that the relative order of the elements that are copied
/// is the same as in the range [first, last).
///
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    while (first < last) {
	if (pred (*first))
	    *result++ = *first;
	++ first;
    }
    return (result);
}

///
/// Remove removes from the range [first, last) all elements that are equal to
/// value. That is, remove returns an iterator new_last such that the range
/// [first, new_last) contains no elements equal to value. [1] The iterators
/// in the range [new_last, last) are all still dereferenceable, but the
/// elements that they point to are unspecified. Remove is stable, meaning
/// that the relative order of elements that are not equal to value is
/// unchanged.
///
template <class ForwardIterator, class T>
inline ForwardIterator remove (ForwardIterator first, ForwardIterator last, const T& value)
{
    return (remove_copy (first, last, first, value));
}

///
/// Remove_if removes from the range [first, last) every element x such that
/// pred(x) is true. That is, remove_if returns an iterator new_last such that
/// the range [first, new_last) contains no elements for which pred is true.
/// The iterators in the range [new_last, last) are all still dereferenceable,
/// but the elements that they point to are unspecified. Remove_if is stable,
/// meaning that the relative order of elements that are not removed is
/// unchanged.
///
template <class ForwardIterator, class Predicate>
inline ForwardIterator remove_if (ForwardIterator first, ForwardIterator last, Predicate pred)
{
    return (remove_copy_if (first, last, first, pred));
}

///
/// Unique_copy copies elements from the range [first, last) to a range
/// beginning with result, except that in a consecutive group of duplicate
/// elements only the first one is copied. The return value is the end of
/// the range to which the elements are copied. This behavior is similar
/// to the Unix filter uniq.
///
template <class InputIterator, class OutputIterator>
OutputIterator unique_copy (InputIterator first, InputIterator last, OutputIterator result)
{
    if (first < last) {
	*result = *first++;
	while (first < last) {
	    if (*first != *result)
		*++result = *first;
	    ++ first;
	}
	++ result;
    }
    return (result);
}

///
/// The reason there are two different versions of unique_copy is that there
/// are two different definitions of what it means for a consecutive group of
/// elements to be duplicates. In the first version, the test is simple
/// equality: the elements in a range [f, l) are duplicates if, for every
/// iterator i in the range, either i == f or else *i == *(i-1). In the second,
/// the test is an arbitrary Binary Predicate binary_pred: the elements in
/// [f, l) are duplicates if, for every iterator i in the range, either
/// i == f or else binary_pred(*i, *(i-1)) is true.
///
template <class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator unique_copy (InputIterator first, InputIterator last, OutputIterator result, BinaryPredicate binary_pred)
{
    if (first < last) {
	*result = *first++;
	while (first < last) {
	    if (binary_pred (*first, prev_value))
		*++result = *first;
	    ++ first;
	}
	++ result;
    }
    return (result);
}

///
/// Every time a consecutive group of duplicate elements appears in the range
/// [first, last), the algorithm unique removes all but the first element.
/// That is, unique returns an iterator new_last such that the range [first,
/// new_last) contains no two consecutive elements that are duplicates.
/// The iterators in the range [new_last, last) are all still dereferenceable,
/// but the elements that they point to are unspecified. Unique is stable,
/// meaning that the relative order of elements that are not removed is
/// unchanged.
///
template <class ForwardIterator>
inline ForwardIterator unique (ForwardIterator first, ForwardIterator last)
{
    return (unique_copy (first, last, first));
}

///
/// Every time a consecutive group of duplicate elements appears in the range
/// [first, last), the algorithm unique removes all but the first element.
/// That is, unique returns an iterator new_last such that the range [first,
/// new_last) contains no two consecutive elements that are duplicates.
/// The iterators in the range [new_last, last) are all still dereferenceable,
/// but the elements that they point to are unspecified. Unique is stable,
/// meaning that the relative order of elements that are not removed is
/// unchanged.
///
template <class ForwardIterator, class BinaryPredicate>
inline ForwardIterator unique (ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred)
{
    return (unique_copy (first, last, first, binary_pred));
}

///
/// Reverse reverses a range.
/// That is: for every i such that 0 <= i <= (last - first) / 2),
/// it exchanges *(first + i) and *(last - (i + 1)).
///
template <class BidirectionalIterator>
void reverse (BidirectionalIterator first, BidirectionalIterator last)
{
    while (first < last)
	swap (*first++, *--last);
}

///
/// Returns the furthermost iterator i in [first, last) such that,
/// for every iterator j in [first, i), *j < value
/// Assumes the range is sorted.
///
template <class ForwardIterator, class LessThanComparable>
ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
{
    ForwardIterator mid;
    while (first < last) {
	mid = (first + (last - first) / 2);
	if (*mid < value)
	    first = mid + 1;
	else
	    last = mid;
    }
    return (first);
}

///
/// Returns the furthermost iterator i in [first, last) such that,
/// for every iterator j in [first, i), comp(*j, value) is true.
/// Assumes the range is sorted.
///
template <class ForwardIterator, class T, class StrictWeakOrdering>
ForwardIterator lower_bound (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator mid;
    while (first < last) {
	mid = (first + (last - first) / 2);
	if (comp (*mid, value))
	    first = mid + 1;
	else
	    last = mid;
    }
    return (first);
}

///
/// Performs a binary search inside the sorted range.
///
template <class ForwardIterator, class LessThanComparable>
ForwardIterator binary_search (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
{
    ForwardIterator found = lower_bound (first, last, value);
    return ((found == last || value < *found) ? last : found);
}

///
/// Performs a binary search inside the sorted range.
///
template <class ForwardIterator, class T, class StrictWeakOrdering>
ForwardIterator binary_search (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator found = lower_bound (first, last, value, comp);
    return ((found == last || comp(value, *found)) ? last : found);
}

///
/// Returns the furthermost iterator i in [first,last) such that for
/// every iterator j in [first,i), value < *j is false.
///
template <class ForwardIterator, class LessThanComparable>
ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
{
    ForwardIterator mid;
    while (first < last) {
	mid = (first + (last - first) / 2);
	if (value < *mid)
	    last = mid;
	else
	    first = mid + 1;
    }
    return (last);
}

///
/// Returns the furthermost iterator i in [first,last) such that for
/// every iterator j in [first,i), comp(value,*j) is false.
///
template <class ForwardIterator, class T, class StrictWeakOrdering>
ForwardIterator upper_bound (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator mid;
    while (first < last) {
	mid = (first + (last - first) / 2);
	if (comp (value, *mid))
	    last = mid;
	else
	    first = mid + 1;
    }
    return (last);
}

///
/// Returns pair<lower_bound,upper_bound>
///
template <class ForwardIterator, class LessThanComparable>
pair<ForwardIterator,ForwardIterator> equal_range (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
{
    pair<ForwardIterator,ForwardIterator> rv;
    rv.second = rv.first = lower_bound (first, last, value);
    while (rv.second < last && !(value < *(rv.second)))
	++ rv.second;
    return (rv);
}

///
/// Returns pair<lower_bound,upper_bound>
///
template <class ForwardIterator, class T, class StrictWeakOrdering>
pair<ForwardIterator,ForwardIterator> equal_range (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    pair<ForwardIterator,ForwardIterator> rv;
    rv.second = rv.first = lower_bound (first, last, value, comp);
    while (rv.second < last && !comp(value, *(rv.second)))
	++ rv.second;
    return (rv);
}

///
/// Sorts the container
///
template <class RandomAccessIterator>
void sort (RandomAccessIterator first, RandomAccessIterator last)
{
    // Insertion sort.
    RandomAccessIterator cur (first + 1);
    while (cur < last) {
	RandomAccessIterator ip = cur;
	while (*cur < *--ip && first < ip);
	reverse (ip, cur);
	reverse (ip, ++cur);
    }
}

///
/// Sorts the container
///
template <class RandomAccessIterator, class Compare>
void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    // Insertion sort.
    RandomAccessIterator cur (first + 1);
    while (cur < last) {
	RandomAccessIterator ip = cur;
	while (comp(*cur, *--ip) && first < ip);
	reverse (ip, cur);
	reverse (ip, ++cur);
    }
}

///
/// Sorts the container
///
template <class RandomAccessIterator>
inline void stable_sort (RandomAccessIterator first, RandomAccessIterator last)
{
    sort (first, last);
}

///
/// Sorts the container
///
template <class RandomAccessIterator, class Compare>
inline void stable_sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    sort (first, last, comp);
}

} // namespace ustl

#endif

