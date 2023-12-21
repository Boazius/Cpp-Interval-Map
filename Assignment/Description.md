# think-cell Recruitment Test Assignment

## Foreword

*Note: Before delving into the details of this recruitment test and my solution, I want to provide some context for those who might come across it.*

This recruitment test is notorious within the software development community and has been in use since at least 2017. It has garnered considerable attention on various platforms, such as Reddit threads like [this one](https://www.reddit.com/r/recruitinghell/comments/ai03x4/a_9_hour_coding_challenge/).

## The 9-Hour Challenge

At the core of this recruitment test is a 9-hour coding challenge, a test that has remained unchanged over the years. Applicants are granted only two attempts to pass it successfully. Failure to meet the strict requirements within these attempts results in an automated rejection.

The test presents a distinctive set of challenges, including **Stringent Time and Type Requirements:** Applicants must navigate complex requirements, including demanding runtime expectations and intricate type constraints. Some of these constraints, like the non-default constructibility of certain data, may not be immediately apparent.

## Sharing My Solution

As someone who has faced this test, I have decided to publish my solution here. My intention is to provide insight into my approach, share the knowledge I gained from this experience, and potentially help others who may find themselves in a similar situation.

I have managed to succeed in 3 out of 4 requirements - the **Running time** requirement unfortunately was not up to their standards.
This is because:
- the cleanup which removes duplicates but requires iterating on the map in O(n)
  - You must find a way to not generate duplicate values when inserting and removing values.
- emplacing values into the map is O(logn).
  - You need to emplace into the map while iterating on it, in O(1).


So, whether you're here to explore my solution or contemplating taking on this well-known but often perceived as **unfair and insulting** test, I hope this publication helps you gain insight into the experience and the unique challenges faced by software development applicants.


# Task Description

`interval_map<K,V>` is a data structure that associates keys of type `K` with values of type `V`. It efficiently handles situations where intervals of consecutive keys are associated with the same value. Your task is to implement the `assign` member function for this data structure.

**`interval_map<K, V>` Implementation:**

`interval_map<K, V>` is built on top of `std::map`. For more information on `std::map`, please refer to [cppreference.com](https://en.cppreference.com/w/cpp/container/map).

Each key-value pair `(k, v)` in `interval_map<K, V>::m_map` signifies that the value `v` is associated with all keys from `k` (inclusive) to the next key (exclusive) in `m_map`. The member `interval_map<K, V>::m_valBegin` holds the value associated with all keys less than the first key in `m_map`.

**Example:**

Let `M` be an instance of `interval_map<int, char>` where:
- `M.m_valBegin == 'A'`
- `M.m_map == { (1, 'B'), (3, 'A') }`

Then, `M` represents the mapping:
```
...
-2 -> 'A'
-1 -> 'A'
0  -> 'A'
1  -> 'B'
2  -> 'B'
3  -> 'A'
4  -> 'A'
5  -> 'A'
...
```

The representation in `std::map` must be canonical, meaning consecutive map entries cannot contain the same value (e.g., `(3, 'A')`, `(5, 'A')`). Also, the first entry in `m_map` must not have the same value as `m_valBegin`. Initially, the entire range of `K` is associated with a given initial value passed to the constructor of the `interval_map<K, V>` data structure.

**Key type `K`:**
- Should be copyable and assignable
- Less-than comparable via `operator<`
- Does not implement other operations, especially no equality comparison or arithmetic operators

**Value type `V`:**
- Should be copyable and assignable
- Equality-comparable via `operator==`
- Does not implement other operations

```c++
#include <map>
template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) 
    {
        // YOUR SOLUTION HERE ################################
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

```

---

### Task Requirements:

1. **Type Requirements Are Met:**
    - Adhere strictly to the specification of the key and value types as given in the task description.
    - Solutions often assume additional operations beyond those explicitly stated (e.g., assuming `V` is default-constructible using `std::map::operator[]`). Ensure compliance to the specified operations only.

2. **Correctness:**
    - Ensure your program generates a functional `interval_map` as described.
    - Pay careful attention to iterator validity. Dereferencing end iterators is illegal.
    - Consider using a checking STL implementation (e.g., shipped with Visual C++ or GCC) to validate iterators.
    - Common issues involve incorrect associations between intervals and values or code paths that lead to invalid or end iterator dereferencing.

3. **Canonicity:**
    - The representation in `m_map` must be canonical.
    - Avoid consecutive map entries containing the same value or repeating `m_valBegin` at the start of the map.
    - Solutions often create non-canonical representations, leading to consecutive map entries with identical values.

4. **Running Time:**
    - Assume your implementation is part of a library and strive for big-O optimality.
    - Minimize unnecessary operations on `K` and `V`, including constructions, destructions, and assignments.
    - Limit the use of amortized O(log N) operations to one in contrast to O(1) operations in `m_map`.
    - Overloads of `std::map::insert/emplace/delete` that need to find an element position have amortized O(log N) time, while those using an iterator have amortized O(1) time if pointing to the correct position.
    - Prioritize simplicity over marginal speed improvements unless crucial for the algorithm's performance.


