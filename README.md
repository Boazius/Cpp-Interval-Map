# Interval Map 

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



