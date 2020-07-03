#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <set>
#include <mutex>

// -----------------------------------------------------------------------------------------------------------------------

/*
	Class intended to find prime numbers and save them to the map.

	-- Usage --

	Chrono timer;
	myPrime pr;
	myPrime::container map;

	pr.getPrimes(map, 1, 42000000);
	timer.end();

	pr.saveDataToFile("___primes.txt", map);
*/

// -----------------------------------------------------------------------------------------------------------------------

class myPrime {

	public:
		typedef std::set<size_t> container;
	
		myPrime(size_t nThreads = 11) : numThreads(nThreads)
		{
		}

		void getPrimes			(container &, size_t, size_t = (size_t(1) << 32) - 1);			// Get primes and save them to a map
		void saveDataToFile		(const char *, container &, bool = true);						// Save map of primes to file
		void readPrimesFromFile	(const char *, container &);									// Read map of files from file
static	bool isPrime			(const size_t);													// Check if a number is prime
		void getPrimeFactors	(size_t, container &, std::vector<size_t> &, bool = true);		// Get all prime factors of a number

		template <class Type>
static	Type getNextPrime		(Type);

		template <class Type>
static	Type getNextPrime		(std::map<Type, Type> &, Type);

static	long long getPrevPrime	(long long);

	private:
		void thFunc(size_t, size_t, size_t, size_t, container &, size_t &);						// Thread func

	private:
		size_t		numThreads;
		std::mutex	mutex;
};

// -----------------------------------------------------------------------------------------------------------------------

// Default max = size_t_32(-1) = 4.294.967.295
void myPrime::getPrimes(container &map, size_t min, size_t max)
{
	size_t nFound = 0;

	std::vector<std::shared_ptr<std::thread>> vecThreads;

	for (size_t id = 0; id < numThreads; id++)
		vecThreads.emplace_back(
			std::make_shared<std::thread>
				(&myPrime::thFunc, this, min, max, id, numThreads, std::ref(map), std::ref(nFound)));

	for (auto th : vecThreads)
		th->join();

	return;
}
// -----------------------------------------------------------------------------------------------------------------------

// Checks if a number is a prime
bool myPrime::isPrime(const size_t num)
{
	if (num > 1)
	{
		// Don't want to calculate sqrt for smaller numbers
		size_t maxDivider = num < 1024 ? num : size_t(sqrt(num)) + 1;

		for (size_t i = 2; i < maxDivider; i++)
			if (!(num % i))
				return false;

		return true;
	}

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------

// 
void myPrime::thFunc(size_t nStart, size_t MAX, size_t id, size_t totalThreads, container &map, size_t& found)
{
	auto saveData = [&](container &src, container &res) -> void
	{
		if (src.size())
		{
			std::lock_guard<std::mutex> doLock(mutex);
				for (auto iter : src)
					res.emplace(iter);
		}
		src.clear();
	};

	bool overflowProtection(false);
	size_t cnt = 0, threshold = 1000 + 333 * (id + 1);
	container m;

	if (MAX + totalThreads < MAX)
		overflowProtection = true;

	for (size_t prev = 0, num = nStart + id; num < MAX; num += totalThreads)
	{
		if (overflowProtection)
		{
			if (prev > num)
				break;
			prev = num;
		}

		if (isPrime(num))
		{
			m.emplace(num);

			if (!(++cnt % threshold))
				saveData(m, map);
		}

		if (overflowProtection)
		{
			prev = num;
		}
	}

	saveData(m, map);
	found += cnt;

	return;
}
// -----------------------------------------------------------------------------------------------------------------------

//
void myPrime::saveDataToFile(const char* fileName, container &map, bool overwrite)
{
	std::cout << "\n Writing to file ... ";

	std::fstream f;
	auto flags = overwrite ? std::fstream::out : std::fstream::out | std::fstream::app;
	f.open(fileName, flags);

	if (f.is_open())
	{
		for (auto num : map)
			f << num << std::endl;

		f.close();
		std::cout << "done!\n" << std::endl;
	}

	return;
}
// -----------------------------------------------------------------------------------------------------------------------

//
void myPrime::readPrimesFromFile(const char* fileName, container &map)
{
	std::fstream f;
	f.open(fileName, std::fstream::in);

	if (f.is_open())
	{
		size_t num;
		std::string line;

		while (std::getline(f, line))
		{
			if (line.length() && line[0] == '#')
				continue;

			num = std::atoi(line.c_str());

			map.emplace(num);
		}

		f.close();
	}

	return;
}
// -----------------------------------------------------------------------------------------------------------------------

// Return next prime number following n
template <class Type>
Type myPrime::getNextPrime(Type n)
{
	while (1)
	{
		bool isPrime(true);
		n++;

		for (Type i = 2; i < n; i++)
		{
			if (!(n % i))
			{
				isPrime = false;
				break;
			}
		}

		if (isPrime)
			break;
	}

	return n;
}
// -----------------------------------------------------------------------------------------------

// Return next prime number following n
// Uses external cache map
template <class Type>
Type myPrime::getNextPrime(std::map<Type, Type>& mapPrimes, Type n)
{
	if (mapPrimes.count(n))
	{
		return mapPrimes[n];
	}
	else
	{
		// key is lower than the max key already in the map
		if (mapPrimes.size() && n < mapPrimes.rbegin()->first)
		{
			return mapPrimes.lower_bound(n)->first;
		}
		else
		{
			Type key = n;

			while (true)
			{
				bool isPrime(true);
				n++;

				for (Type i = 2; i < n; i++)
				{
					if (!(n % i))
					{
						isPrime = false;
						break;
					}
				}

				if (isPrime)
				{
					mapPrimes.emplace(key, n);
					break;
				}
			}
		}
	}

	return n;
};
// -----------------------------------------------------------------------------------------------

// Return next prime number before n
long long myPrime::getPrevPrime(long long n)
{
	while (1)
	{
		bool isPrime(true);
		n--;

		for (long long i = 2; i < n; i++)
		{
			if (!(n % i))
			{
				isPrime = false;
				break;
			}
		}

		if (isPrime)
			break;
	}

	return n;
}
// -----------------------------------------------------------------------------------------------

// Get all prime factors of a number
// skipRepeating == false: 100 => [2, 2, 5, 5]
// skipRepeating == true : 100 => [2, 5]
void myPrime::getPrimeFactors(size_t num, container &primesMap, std::vector<size_t> &vec, bool skipRepeating /*=true*/)
{
	vec.clear();

	if (!primesMap.count(num))
	{
		for (auto iter = primesMap.begin(); iter != primesMap.end(); ++iter)
		{
			if (*iter > num)
				break;

			while(num % *iter == 0)
			{
				vec.emplace_back(*iter);
				num = num / *iter;

				if (skipRepeating)
					break;
			}
		}
	}

	return;
}
// -----------------------------------------------------------------------------------------------
