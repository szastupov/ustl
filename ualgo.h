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
// ualgo.h
//
// Implementation of STL algorithms.
//
// The function prototypes are copied
// exactly from the SGI version of STL documentation along with comments about
// their use. The code is NOT the same, though the functionality usually is.
//

#ifndef UALGO_H
#define UALGO_H

#include "upair.h"
#include "ualgobase.h"
#include "uiterator.h"
#include "ufunction.h"

namespace ustl {

/// Copy copies elements from the range (last, first] to result.
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy_backward (InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first < last; ++result)
	*result = *--last;
    return (result);
}

/// Swaps corresponding elements of [first, last) and [result,)
template <typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator2 swap_ranges (ForwardIterator1 first, ForwardIterator2 last, ForwardIterator2 result)
{
    for (; first < last; ++first, ++result)
	iterator_swap (first, result);
    return (result);
}

///
/// Copy_if copies elements from the range [first, last) to the range
/// [result, result + (last - first)) if pred(*i) returns true.
///
template <typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    for (; first < last; ++first) {
	if (pred(*first)) {
	    *result = *first;
	    ++ result;
	}
    }
    return (result);
}

///
/// Returns the first iterator i in the range [first, last) such that
/// *i == value. Returns last if no such iterator exists. 
///
template <typename InputIterator, typename EqualityComparable>
inline InputIterator find (InputIterator first, InputIterator last, const EqualityComparable& value)
{
    while (first < last && !(*first == value))
	++ first;
    return (first);
}

///
/// Returns the first iterator i in the range [first, last) such that
/// pred(*i) is true. Returns last if no such iterator exists.
///
template <typename InputIterator, typename Predicate>
inline InputIterator find_if (InputIterator first, InputIterator last, Predicate pred)
{
    while (first < last && !pred (*first))
	++ first;
    return (first);
}

///
/// Returns the pointer to the first pair of unequal elements.
///
template <typename InputIterator, typename BinaryPredicate>
inline pair<InputIterator,InputIterator>
mismatch (InputIterator first1, InputIterator last1, InputIterator first2, BinaryPredicate comp)
{
    while (first1 < last1 && comp(*first1, *first2))
	++ first1, ++ first2;
    return (make_pair (first1, first2));
}

///
/// Returns the pointer to the first pair of unequal elements.
///
template <typename InputIterator>
inline pair<InputIterator,InputIterator>
mismatch (InputIterator first1, InputIterator last1, InputIterator first2)
{
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    return (mismatch (first1, last1, first2, equal_to<value_type>()));
}

///
/// Returns true if two ranges are equal.
/// This is an extension, present in uSTL and SGI STL.
///
template <typename InputIterator, typename BinaryPredicate>
inline bool equal (InputIterator first1, InputIterator last1, InputIterator first2, BinaryPredicate comp)
{
    return (mismatch (first1, last1, first2, comp).first == last1);
}

///
/// Returns true if two ranges are equal.
/// This is an extension, present in uSTL and SGI STL.
///
template <typename InputIterator>
inline bool equal (InputIterator first1, InputIterator last1, InputIterator first2)
{
    return (mismatch (first1, last1, first2).first == last1);
}

///
/// Count finds the number of elements in [first, last) that are equal
/// to value. More precisely, the first version of count returns the
/// number of iterators i in [first, last) such that *i == value.
///
template <typename InputIterator, typename EqualityComparable>
inline size_t count (InputIterator first, InputIterator last, const EqualityComparable& value)
{
    size_t total = 0;
    for (; first < last; ++first)
	if (*first == value)
	    ++ total;
    return (total);
}

///
/// Count_if finds the number of elements in [first, last) that satisfy the
/// predicate pred. More precisely, the first version of count_if returns the
/// number of iterators i in [first, last) such that pred(*i) is true.
///
template <typename InputIterator, typename Predicate>
inline size_t count_if (InputIterator first, InputIterator last, Predicate pred)
{
    size_t total = 0;
    for (; first < last; ++first)
	if (pred (*first))
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
template <typename InputIterator, typename OutputIterator, typename UnaryFunction>
inline OutputIterator transform (InputIterator first, InputIterator last, OutputIterator result, UnaryFunction op)
{
    for (; first < last; ++result, ++first)
	*result = op (*first);
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
template <typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryFunction>
inline OutputIterator transform (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryFunction op)
{
    for (; first1 < last1; ++result, ++first1, ++first2)
	*result = op (*first1, *first2);
    return (result);
}

///
/// Replace replaces every element in the range [first, last) equal to
/// old_value with new_value. That is: for every iterator i,
/// if *i == old_value then it performs the assignment *i = new_value.
///
template <typename ForwardIterator, typename T>
inline void replace (ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
{
    for (; first < last; ++first)
	if (*first == old_value)
	    *first = new_value;
}

///
/// Replace_if replaces every element in the range [first, last) for which
/// pred returns true with new_value. That is: for every iterator i, if
/// pred(*i) is true then it performs the assignment *i = new_value.
///
template <typename ForwardIterator, typename Predicate, typename T>
inline void replace_if (ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value)
{
    for (; first < last; ++first)
	if (pred (*first))
	    *first = new_value;
}

///
/// Replace_copy copies elements from the range [first, last) to the range
/// [result, result + (last-first)), except that any element equal to old_value
/// is not copied; new_value is copied instead. More precisely, for every
/// integer n such that 0 <= n < last-first, replace_copy performs the
/// assignment *(result+n) = new_value if *(first+n) == old_value, and
/// *(result+n) = *(first+n) otherwise.
///
template <typename InputIterator, typename OutputIterator, typename T>
inline OutputIterator replace_copy (InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)
{
    for (; first < last; ++result, ++first)
        *result = (*first == old_value) ? new_value : *first;
}

///
/// Replace_copy_if copies elements from the range [first, last) to the range
/// [result, result + (last-first)), except that any element for which pred is
/// true is not copied; new_value is copied instead. More precisely, for every
/// integer n such that 0 <= n < last-first, replace_copy_if performs the
/// assignment *(result+n) = new_value if pred(*(first+n)),
/// and *(result+n) = *(first+n) otherwise.
///
template <typename InputIterator, typename OutputIterator, typename Predicate, typename T>
inline OutputIterator replace_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) 
{
    for (; first < last; ++result, ++first)
        *result = pred(*first) ? new_value : *first;
}

///
/// Generate assigns the result of invoking gen, a function object that
/// takes no arguments, to each element in the range [first, last).
///
template <typename ForwardIterator, typename Generator>
inline void generate (ForwardIterator first, ForwardIterator last, Generator gen)
{
    for (; first < last; ++first)
	*first = gen();
}

///
/// Generate_n assigns the result of invoking gen, a function object that
/// takes no arguments, to each element in the range [first, first+n).
/// The return value is first + n.
///
template <typename OutputIterator, typename Generator>
inline OutputIterator generate_n (OutputIterator first, size_t n, Generator gen)
{
    for (; n--; ++first)
	*first = gen();
    return (first);
}

///
/// Reverse reverses a range.
/// That is: for every i such that 0 <= i <= (last - first) / 2),
/// it exchanges *(first + i) and *(last - (i + 1)).
///
template <typename BidirectionalIterator>
inline void reverse (BidirectionalIterator first, BidirectionalIterator last)
{
    for (; first < last; ++first)
	swap (*first, *--last);
}

///
/// Exchanges ranges [first, middle) and [middle, last)
///
template <typename ForwardIterator>
ForwardIterator rotate (ForwardIterator first, ForwardIterator middle, ForwardIterator last)
{
    if (first == middle || middle == last)
	return (first); // the push_back implementation will always go here.
    reverse (first, middle);
    reverse (middle, last);
    for (;first < middle && middle < last; ++first)
	swap (*first, *--last);
    if (first == middle)
	reverse (middle, last);
    else
	reverse (first, middle);
    return (first);
}

///
/// Combines two sorted ranges.
///
template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
		      InputIterator2 first2, InputIterator2 last2, OutputIterator result)
{
    for (; first1 < last1 && first2 < last2; ++result) {
	if (*first1 < *first2)
	    *result = *first1++;
	else
	    *result = *first2++;
    }
    if (first1 < last1)
	return (copy (first1, last1, result));
    else
	return (copy (first2, last2, result));
}

///
/// Combines two sorted ranges from the same container.
///
template <typename InputIterator>
void inplace_merge (InputIterator first, InputIterator middle, InputIterator last)
{
    for (; middle < last; ++first) {
	while (*first < *middle)
	    ++ first;
	reverse (first, middle);
	reverse (first, ++middle);
    }
}

///
/// Remove_copy copies elements that are not equal to value from the range
/// [first, last) to a range beginning at result. The return value is the
/// end of the resulting range. This operation is stable, meaning that the
/// relative order of the elements that are copied is the same as in the
/// range [first, last).
///
template <typename InputIterator, typename OutputIterator, typename T>
inline OutputIterator remove_copy (InputIterator first, InputIterator last, OutputIterator result, const T& value)
{
    for (; first < last; ++first) {
	if (*first != value) {
	    *result = *first;
	    ++ result;
	}
    }
    return (result);
}

///
/// Remove_copy copies elements that are not equal to values in [rfirst, rlast)
/// from the range [first, last) to a range beginning at result. The return
/// value is the end of the resulting range. This operation is stable, meaning
/// that the relative order of the elements that are copied is the same as in the
/// range [first, last). Range [rfirst, rlast) is assumed to be sorted.
/// This algorithm is a uSTL extension.
///
template <typename InputIterator, typename OutputIterator, typename RInputIterator>
OutputIterator remove_copy (InputIterator first, InputIterator last, OutputIterator result, RInputIterator rfirst, RInputIterator rlast)
{
    for (; first < last; ++first) {
	while (rfirst < rlast && *rfirst < first)
	    ++ rfirst;
	if (rfirst == rlast || first != *rfirst) {
	    *result = *first;
	    ++ result;
	}
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
template <typename InputIterator, typename OutputIterator, typename Predicate>
inline OutputIterator remove_copy_if (InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    for (; first < last; ++first)
	if (pred (*first))
	    *result++ = *first;
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
template <typename ForwardIterator, typename T>
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
template <typename ForwardIterator, typename Predicate>
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
template <typename InputIterator, typename OutputIterator>
OutputIterator unique_copy (InputIterator first, InputIterator last, OutputIterator result)
{
    if (first < last) {
	*result = *first;
	while (++first < last)
	    if (*first != *result)
		*++result = *first;
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
template <typename InputIterator, typename OutputIterator, typename BinaryPredicate>
OutputIterator unique_copy (InputIterator first, InputIterator last, OutputIterator result, BinaryPredicate binary_pred)
{
    if (first < last) {
	*result = *first;
	while (++first < last)
	    if (binary_pred (*first, prev_value))
		*++result = *first;
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
template <typename ForwardIterator>
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
template <typename ForwardIterator, typename BinaryPredicate>
inline ForwardIterator unique (ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred)
{
    return (unique_copy (first, last, first, binary_pred));
}

///
/// Returns the furthermost iterator i in [first, last) such that,
/// for every iterator j in [first, i), *j < value
/// Assumes the range is sorted.
///
template <typename ForwardIterator, typename LessThanComparable>
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
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
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
template <typename ForwardIterator, typename LessThanComparable>
inline ForwardIterator binary_search (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
{
    ForwardIterator found = lower_bound (first, last, value);
    return ((found == last || value < *found) ? last : found);
}

///
/// Performs a binary search inside the sorted range.
///
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
inline ForwardIterator binary_search (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
{
    ForwardIterator found = lower_bound (first, last, value, comp);
    return ((found == last || comp(value, *found)) ? last : found);
}

///
/// Returns the furthermost iterator i in [first,last) such that for
/// every iterator j in [first,i), value < *j is false.
///
template <typename ForwardIterator, typename LessThanComparable>
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
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
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
template <typename ForwardIterator, typename LessThanComparable>
inline pair<ForwardIterator,ForwardIterator> equal_range (ForwardIterator first, ForwardIterator last, const LessThanComparable& value)
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
template <typename ForwardIterator, typename T, typename StrictWeakOrdering>
inline pair<ForwardIterator,ForwardIterator> equal_range (ForwardIterator first, ForwardIterator last, const T& value, StrictWeakOrdering comp)
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
template <typename RandomAccessIterator, typename Compare>
void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    for (RandomAccessIterator ip (first); ip < last; ++ ip) {
	RandomAccessIterator minEl (ip), curEl (ip);
	while (++curEl < last)
	    if (comp(*curEl, *minEl))
		minEl = curEl;
	if (ip != minEl)
	    swap (*ip, *minEl);
    }
}

///
/// Sorts the container
///
template <typename RandomAccessIterator>
inline void sort (RandomAccessIterator first, RandomAccessIterator last)
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    sort (first, last, less<value_type>());
}

///
/// Sorts the container
///
template <typename RandomAccessIterator>
inline void stable_sort (RandomAccessIterator first, RandomAccessIterator last)
{
    sort (first, last);
}

///
/// Sorts the container
///
template <typename RandomAccessIterator, typename Compare>
inline void stable_sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    sort (first, last, comp);
}

}; // namespace ustl

#endif

