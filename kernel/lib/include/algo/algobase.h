#pragma once

#include <algo/utility.h>
namespace util
{

template <typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 left, ForwardIterator2 right)
{
	swap(*left, *right);
}

template <typename ForwardIterator>
void reverse(ForwardIterator first, ForwardIterator last)
{
	while (first != last)
	{
		iter_swap(first, last);
		first++;
		if (first == last)
			break;
		last--;
	}
}

template <typename Forward_iterator, typename _Val>
Forward_iterator find(Forward_iterator first, Forward_iterator last, const _Val &value)
{
	for (; first != last; ++first)
	{
		if (*first == value)
		{
			break;
		}
	}
	return first;
}
template <typename Forward_iterator, typename Predicate>
Forward_iterator find_if(Forward_iterator first, Forward_iterator last, Predicate pred)
{
	for (; first != last; ++first)
	{
		if (pred(*first))
		{
			break;
		}
	}
	return first;
}
template <typename ForwardIt, typename Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last,
					  Compare comp)
{
	if (first == last)
		return last;

	ForwardIt smallest = first++;
	for (; first != last; ++first)
	{
		if (comp(*first, *smallest))
		{
			smallest = first;
		}
	}
	return smallest;
}
template <typename ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last)
{
	if (first == last)
		return last;

	ForwardIt smallest = first++;
	for (; first != last; ++first)
	{
		if (*first < *smallest)
		{
			smallest = first;
		}
	}
	return smallest;
}
template <typename T>
T max(const T &a, const T &b)
{
	return a < b ? b : a;
}
template <typename T>
T min(const T &a, const T &b)
{
	return a < b ? a : b;
}
template <typename Ty>
Ty clamp(Ty value, Ty min, Ty max)
{
	if (value < min)
	{
		return min;
	}
	else if (max < value)
	{
		return max;
	}
	else
	{
		return value;
	}
}
template <typename Outfit, typename Diff, typename Ty>
Outfit Fill_n(Outfit dest, Diff count, Ty val)
{
	for (; 0 < count; --count, (void)++dest)
		*dest = val;
	return (dest);
}
} // namespace util
