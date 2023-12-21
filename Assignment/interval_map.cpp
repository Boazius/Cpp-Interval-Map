#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

template <typename K, typename V>
class interval_map
{
	// friend void IntervalMapTest(); // Allow IntervalMapTest() to access private members

public:
	V m_valBegin; // holds the value that is associated with all keys less than the first key in m_map.

	std::map<K, V> m_map;
	// constructor associates whole range of K with val
	interval_map(V const &val) : m_valBegin(val) // Initialize m_valBegin with val
	{
	}

	////////////////////////////////////////////////////////////////////////
	// Assigns value val to the interval [keyBegin, keyEnd)
	void assign(K const &keyBegin, K const &keyEnd, V const &val)
	{
		// Check if the interval is empty and return if so
		if (!(keyBegin < keyEnd))
		{
			return; // Empty interval, do nothing
		}

		// Find the first key that is greater than or equal to keyBegin
		auto it = m_map.lower_bound(keyBegin);

		// Store the previous iterator
		auto prevIt = it;

		// If we reached the end, for example, inserting [6,7),D into the map: (1,A),(3,B),(5,C), or an empty map
		if (it == m_map.end())
		{
			// Check the previous key
			if (it == m_map.begin())
			{
				// This is the first entry in m_map and must not contain the same value as m_valBegin
				if (m_valBegin == val)
				{
					return; // Do nothing if the value is the same as m_valBegin
				}
				// Insert a new key with val from keyBegin to keyEnd
				m_map.emplace(keyBegin, val);
				// Insert a new key with m_valBegin from keyEnd to the end of the line
				m_map.emplace(keyEnd, m_valBegin);
				// Result: (6,D),(7,Initial)
			}
			else
			{
				prevIt--;
				// Insert a new key with val from keyBegin to keyEnd
				m_map.emplace(keyBegin, val);
				// Insert a new key with the previous value from keyEnd to the end of the line
				m_map.emplace(keyEnd, (*prevIt).second);
				// Result: ... (5,C), (6,D), (7,C)
			}
		}
		// Check if lower_bound is exactly the keyBegin.
		// For example, if we insert (3,6),D and the map is (1,A),(3,B),(5,C), so lower_bound is (3,B).
		// cant use equality so use this:
		else if ((!((*it).first < keyBegin) && !(keyBegin < (*it).first)))
		{
			auto temp = (*it).second; // Store the value associated with the found key

			// Remove the current entry and handle different cases
			while (true)
			{
				// Reached the end, for example, if we insert (3,D) and the map is (3,B)
				if (it == m_map.end())
				{
					// Insert (3,D)
					m_map.emplace(keyBegin, val);
					// Insert temp, i.e., the previous (3,B)
					m_map.emplace(keyEnd, temp);
					break;
				}
				else if ((*it).first < keyEnd)
				{
					// i.e., (5,'C') - save it as temp and delete it, and continue
					temp = (*it).second;
					it = m_map.erase(it); // Remove the current entry
					continue;
				}
				else if (!((*it).first < keyEnd) && !(keyEnd < (*it).first))
				{
					// i.e., (6,E) - insert (3,D) and that's it
					m_map.emplace(keyBegin, val);
					break;
				}
				else if (keyEnd < (*it).first)
				{
					// i.e., (7,'F') - insert (3,D) and insert temp (6,C)
					m_map.emplace(keyBegin, val);
					m_map.emplace(keyEnd, temp);
					break;
				}
			}
		}
		// If lower_bound is larger than our key.
		// For example, we insert (3,6),V, to (1,A),(4,B),(5,C),(7,E).
		else if (keyBegin < (*it).first)
		{
			auto temp = m_valBegin; // Initialize temp with m_valBegin

			// Get the previous value.
			if (it != m_map.begin())
			{
				// This is not the first entry, i.e., we insert (3,6),V to (4,B),(8,C). So temp is (4,B).
				auto prevIt = it;
				prevIt--;
				temp = (*prevIt).second;
			}

			while (true)
			{
				// Got to the end, so for example, we insert (3,6),V, to (1,A),(4,B),(5,C).
				if (it == m_map.end())
				{
					// Insert (3,V)
					m_map.emplace(keyBegin, val);
					// Insert (6,C)
					m_map.emplace(keyEnd, temp);
					break;
				}
				else if ((*it).first < keyEnd)
				{
					// We insert (3,6),V to (1,A),(4,B), so we need to delete (4,B)
					// Save the value of the key we want to delete, maybe appended later
					temp = (*it).second;
					it = m_map.erase(it);
					continue;
				}
				else if (!(keyEnd < (*it).first) && (!((*it).first < keyEnd)))
				{
					// We insert (3,6),V to (1,A),(4,B),(6,C),(8,D).
					m_map.emplace(keyBegin, val);
					break;
				}
				else if (keyEnd < (*it).first)
				{
					// We insert (3,6),V to (1,A), (8,B)
					m_map.emplace(keyBegin, val);
					m_map.emplace(keyEnd, temp);
					break;
				}
			}
		}

		// Cleanup non-canonical values - O(n) which is a problem!
		if (m_map.empty())
			return;

		// Remove consecutive duplicate values
		auto it = m_map.begin();
		V prevVal = m_valBegin;
		std::vector<K> keysToRemove;

		while (it != m_map.end())
		{
			auto nextIt = std::next(it);
			if (nextIt != m_map.end() && nextIt->second == it->second)
			{
				keysToRemove.push_back(nextIt->first);
			}
			it = nextIt;
		}

		for (const K &key : keysToRemove)
		{
			m_map.erase(key);
		}

		// Remove initial values that match m_valBegin
		while (!m_map.empty() && m_map.begin()->second == m_valBegin)
		{
			m_map.erase(m_map.begin());
		}
	}
	////////////////////////////////////////////////////////////////////////

	// look-up of the value associated with key
	V const &operator[](K const &key) const
	{
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin())
		{
			return m_valBegin;
		}
		else
		{
			return (--it)->second;
		}
	}
};

using namespace std;

void runTest(const std::map<int, std::string> &initialMap, int intervalStart, int intervalEnd, std::string intervalValue)
{
	std::cout << "Initial map:" << std::endl;
	for (const auto &pair : initialMap)
	{
		std::cout << "(" << pair.first << ",'" << pair.second << "'), ";
	}
	std::cout << std::endl;

	interval_map<int, std::string> testMap("Initial");
	testMap.m_map = initialMap;

	testMap.assign(intervalStart, intervalEnd, intervalValue);

	std::cout << "After assigning interval [" << intervalStart << ", " << intervalEnd << ") with value '" << intervalValue << "':" << std::endl;
	for (auto it = testMap.m_map.begin(); it != testMap.m_map.end(); ++it)
	{
		std::cout << "(" << it->first << ",'" << it->second << "'), ";
	}
	std::cout << std::endl
			  << std::endl;
}

void runTestCase(const std::map<int, std::string> &initialMap, int intervalStart, int intervalEnd, std::string intervalValue, const std::string &expectedOutput)
{
	std::cout << "Initial map:" << std::endl;
	for (const auto &pair : initialMap)
	{
		std::cout << "(" << pair.first << ",'" << pair.second << "'), ";
	}
	std::cout << std::endl;

	interval_map<int, std::string> testMap("Initial");
	testMap.m_map = initialMap;

	testMap.assign(intervalStart, intervalEnd, intervalValue);

	std::cout << "After assigning interval [" << intervalStart << ", " << intervalEnd << ") with value '" << intervalValue << "':" << std::endl;
	for (auto it = testMap.m_map.begin(); it != testMap.m_map.end(); ++it)
	{
		std::cout << "(" << it->first << ",'" << it->second << "'), ";
	}
	std::cout << std::endl;

	// Compare expected output with the resulting map
	std::stringstream ss;
	for (auto it = testMap.m_map.begin(); it != testMap.m_map.end(); ++it)
	{
		ss << "(" << it->first << ",'" << it->second << "'), ";
	}
	std::string actualOutput = ss.str();
	actualOutput = actualOutput.substr(0, actualOutput.size() - 2); // Remove trailing comma and space

	if (expectedOutput == actualOutput)
	{
		std::cout << "Test Passed! Result matches expected output." << std::endl;
	}
	else
	{
		std::cout << "Test Failed! Result does not match expected output.#################################################" << std::endl;
	}
	std::cout << std::endl;
}

int main()
{

	std::map<int, std::string> initialMap = {{1, "A"}, {2, "D"}, {5, "C"}};
	runTestCase(initialMap, 3, 4, "E", "(1,'A'), (2,'D'), (3,'E'), (4,'D'), (5,'C')");
	runTestCase(initialMap, 2, 6, "F", "(1,'A'), (2,'F'), (6,'C')");

	/*

		std::map<int, std::string> initialMap = {{1, "A"}, {3, "B"}, {5, "C"}};
	runTestCase(initialMap, 2, 4, "B", "(1,'A'), (2,'B'), (5,'C')");
	runTestCase(initialMap, 3, 6, "D", "(1,'A'), (3,'D'), (6,'C')");

	std::map<int, std::string> initialMap = {{1, "A"}, {3, "B"}, {5, "C"}};
	runTest(initialMap, 2, 4, "B");
	runTestCase(initialMap, 2, 4, "B", "(1,'A'), (2,'B'), (5,'C')");

	std::map<int, std::string> initialMap1 = {};
	runTest(initialMap1, 2, 4, "X");
	runTestCase(initialMap1, 2, 4, "Initial", "");

	std::map<int, std::string> initialMap2 = {};
	runTest(initialMap2, 0, 6, "Y");
	runTestCase(initialMap2, 0, 6, "Y", "(0,'Y'), (6,'Initial')");


	// Test Case 1
	std::map<int, std::string> initialMap1 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap1, 2, 4, "X");
	runTestCase(initialMap1, 2, 4, "X", "(1,'A'), (2,'X'), (4,'B'), (5,'C')");

	// Test Case 2
	std::map<int, std::string> initialMap2 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap2, 0, 6, "Y");
	runTestCase(initialMap2, 0, 6, "Y", "(0,'Y'), (6,'C')");


	// // Additional Test Cases
	std::map<int, std::string> initialMap3 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap3, 4, 8, "Z");
	runTestCase(initialMap3, 4, 8, "Z","(1,'A'), (3,'B'), (4,'Z'), (8,'C')");

	std::map<int, std::string> initialMap4 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap4, 3, 5, "W");
	runTestCase(initialMap4, 3, 5, "W","(1,'A'), (3,'W'), (5,'C')" );



	std::map<int, std::string> initialMap5 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap5, 0, 2, "V");
	runTestCase(initialMap5, 0, 2, "V", "(0,'V'), (2,'A'), (3,'B'), (5,'C')");

	std::map<int, std::string> initialMap6 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap6, 2, 4, "A");
	runTestCase(initialMap6, 2, 4, "A", "(1,'A'), (4,'B'), (5,'C')");

	std::map<int, std::string> initialMap7 = {{1, "A"}, {2, "B"}, {5, "C"}};
	// runTest(initialMap7, 2, 4, "B");
	runTestCase(initialMap7, 2, 4, "B", "(1,'A'), (2,'B'), (5,'C')");

	std::map<int, std::string> initialMap8 = {{3, "A"}, {5, "B"}, {7, "C"}};
	// runTest(initialMap8, 3, 6, "A");
	runTestCase(initialMap8, 3, 6, "A", "(3,'A'), (6,'B'), (7,'C')");

	std::map<int, std::string> initialMap9 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap9, 1, 5, "A");
	runTestCase(initialMap9, 1, 5, "A", "(1,'A'), (5,'C')");

	std::map<int, std::string> initialMap10 = {{1, "A"}, {3, "B"}, {5, "C"}};
	// runTest(initialMap10, 0, 6, "C");
	runTestCase(initialMap10, 0, 6, "C","(0,'C')");

	*/

	return 0;
}
