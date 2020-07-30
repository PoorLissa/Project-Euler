#pragma once

#include <vector>
#include <string>
#include <windows.h>

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

#define doPrint(val) std::cout << val << std::endl;

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

class loopCounter {

	public:
		loopCounter(const size_t MAX) : _maxCounter(MAX/100), _counter(0), _cnt(0)
		{
		}

		void operator ++(int a)
		{
			if (++_counter == _maxCounter)
			{
				std::cout << ".";
				_counter = 0;

				if (++_cnt == 33)
				{
					std::cout << std::endl;
					_cnt = 0;
				}
			}
		}

	private:
		size_t _counter, _maxCounter, _cnt;
};

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

template <class Type>
void validateResult(const Type &expected, const Type &answer)
{
	#define clGreen		FOREGROUND_GREEN|								  FOREGROUND_INTENSITY
	#define clWhite		FOREGROUND_RED	|FOREGROUND_GREEN|FOREGROUND_BLUE
	#define clYellow	FOREGROUND_GREEN|FOREGROUND_RED	 |				  FOREGROUND_INTENSITY
	#define clRed		FOREGROUND_RED	|								  FOREGROUND_INTENSITY

	auto console = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << "\n -- Result: ";

	if (expected == answer)
	{
		SetConsoleTextAttribute(console, clGreen);
		std::cout << "OK";
	}
	else
	{
		SetConsoleTextAttribute(console, clRed);
		std::cout << "Fail";
	}

	SetConsoleTextAttribute(console, clWhite);

	std::cout << " --" << std::endl;
}

// -----------------------------------------------------------------------------------------------

// Return binary representation of a number as a string
std::string base10_to_base2(size_t num)
{
	std::string res;
	size_t n = 1;

	while (n <= num)
	{
		res = (num & n ? "1" : "0") + res;
		n *= 2;
	}

	return res;
}

// -----------------------------------------------------------------------------------------------

// Check if a string is palindromic
bool isPalindromic(const std::string& str)
{
	size_t len = str.length();

	for (size_t i = 0; i < len / 2; i++)
		if (str[i] != str[len-i-1])
			return false;

	return true;
}

// -----------------------------------------------------------------------------------------------

// Check if the number is palindromic (in base 10)
bool isPalindromic_base10(size_t n)
{
	size_t nOld = n, nNew = 0, dec = 0;

	while (n)
	{
		size_t digit = n % 10;
		nNew = 10 * nNew + digit;
		n = n / 10;
	}

	return nOld == nNew;
}

// -----------------------------------------------------------------------------------------------

// Check if the number is palindromic (in base 2)
bool isPalindromic_base2(size_t num)
{
	return isPalindromic(base10_to_base2(num));
}

// -----------------------------------------------------------------------------------------------

// If the number is Pandigital [1..n]
bool isPandigital(size_t num)
{
	int cnt = 0, digit = 0;
	unsigned int bin = 0;

	while (num)
	{
		if (!(digit = num % 10))
			return false;

		bin |= 1 << --digit;
		num /= 10;
		cnt++;
	}

	return bin == (1 << cnt) - 1;
};

// -----------------------------------------------------------------------------------------------

// If the number is Pandigital [min..max]
template<class Type>
bool isPandigital(Type num, unsigned int min, unsigned int max)
{
	unsigned int digit = 0;
	Type bin = 0;

	while (num)
	{
		if ((digit = num % 10) < min)
			return false;

		bin |= Type(1) << digit;
		num /= 10;
	}

	bin = bin >> min;

	return bin == (1 << (max - min + 1)) - 1;
};

// -----------------------------------------------------------------------------------------------

//	n ==> n / 2  (n is even)
//	n ==> 3n + 1 (n is odd)
size_t getCollatzSequenceLength(size_t startingNum)
{
	size_t cnt = 1;

	while (startingNum > 1)
	{
		if (startingNum & 1)
			startingNum = 3 * startingNum + 1;
		else
			startingNum = startingNum >> 1;

		cnt++;
	}

	return cnt;
}

// -----------------------------------------------------------------------------------------------

// Splits number into vector
// Tags: parse, split, divide
void split(std::vector<int> &vec, size_t num)
{
	vec.clear();

	while (num)
	{
		size_t rem = num % 10;
		vec.push_back((int)rem);
		num /= 10;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Splits number into map
// Tags: parse, split, divide
template<class Type1, class Type2>
void split(std::map<Type1, Type2> &map, size_t num)
{
	map.clear();

	while (num)
	{
		Type1 rem = num % 10;

		if(!map.count(rem))
			map[rem] = Type2();
		num /= 10;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Splits string into into vector using delimiter
// Tags: parse, split, divide
void split(const std::string &str, std::vector<std::string> &vec, const char delim = ' ')
{
	vec.clear();
	size_t pos1 = 0, pos2 = 0;

	while (pos2 != std::string::npos)
	{
		pos2 = str.find(delim, pos1);
		vec.emplace_back(str.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Converts number to in its text representation
std::string numToStr(size_t n)
{
	std::string res;
	std::vector<int> vec;

	size_t cnt = 0, next = 0;

	split(vec, n);

	std::string str1[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	std::string str2[] = { "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };
	std::string str3[] = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };

	for (auto iter = vec.rbegin(); iter != vec.rend(); ++iter)
	{
		size_t position = vec.size() - cnt,
			digit = vec[position - 1];

		switch (position)
		{
			case 1:
				if (digit > 0)
					res += str1[digit - 1];
				break;

			case 2:
				if (digit)
				{
					if (digit > 1)
					{
						res += str2[digit - 2];
						res += " ";
					}
					else
					{
						next = vec[position - 2];
						res += str3[next];
						++iter;
					}
				}
				break;

			case 3:
				if (digit)
				{
					res += str1[digit - 1];
					res += " hundred ";
				}

				for (int i = 0; i < 2; i++)
				{
					next = vec[position - 2 - i];

					if (next)
					{
						res += "and ";
						break;
					}
				}
				break;

			case 4:
				res += str1[digit - 1];
				res += " thousand ";
				break;
		}

		cnt++;
	}

	return res;
}

// -----------------------------------------------------------------------------------------------

bool isLeapYear(size_t year)
{
	return !(year % 4) && (year % 100) && (year % 400);
}

// -----------------------------------------------------------------------------------------------

// 0 = Jan, 11 = Dec
size_t getDaysInMonth(size_t month, bool isLeap)
{
	switch (month)
	{
		case 3:
		case 5:
		case 8:
		case 10:
			return 30;

		case 1:
			return isLeap ? 29 : 28;
	}

	return 31;
};

// -----------------------------------------------------------------------------------------------

// Get all possible permutations of [srcVec's] elements
void getPermutations(std::vector<std::string>& resVec, std::vector<char> srcVec)
{
	if (srcVec.size() == 2)
	{
		std::string s;

		s  = srcVec[0];
		s += srcVec[1];
		resVec.emplace_back(s);

		s  = srcVec[1];
		s += srcVec[0];
		resVec.emplace_back(s);
	}
	else
	{
		for (size_t i = 0; i < srcVec.size(); i++)
		{
			std::vector<char> vec_char;
			std::vector<std::string> vec_str;

			for (size_t j = 0; j < srcVec.size(); j++)
				if (j != i)
					vec_char.emplace_back(srcVec[j]);

			getPermutations(vec_str, vec_char);

			for (auto str : vec_str)
				resVec.emplace_back(srcVec[i] + str);
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

void getPermutations(std::vector<size_t>& resVec, std::vector<int>& srcVec)
{
	std::vector<size_t> vec_tmp;

	if (srcVec.size() == 2)
	{
		resVec.emplace_back(srcVec[0] * 10 + srcVec[1]);
		resVec.emplace_back(srcVec[1] * 10 + srcVec[0]);
	}
	else
	{
		std::vector<int>	vec_singles;
		std::vector<size_t>	vec_permutations;

		for (size_t i = 0; i < srcVec.size(); i++)
		{
			for (size_t j = 0; j < srcVec.size(); j++)
				if (j != i)
					vec_singles.emplace_back(srcVec[j]);

			getPermutations(vec_permutations, vec_singles);

			for (auto vec = vec_permutations.begin(); vec != vec_permutations.end(); ++vec)
			{
				size_t num = srcVec[i], dec = 10, size = vec_permutations.size(), j = 2;

				while (size != 1)
				{
					size /= j++;
					 dec *= 10;
				}

				resVec.emplace_back(dec*num + *vec);
			}

			vec_singles.clear();
			vec_permutations.clear();
		}
	}

	return;
}
// -----------------------------------------------------------------------------------------------

// Get all possible permutations of [srcVec's] elements
template<class Type>
void getPermutations(std::vector<std::vector<Type>>& resVec, std::vector<Type> &srcVec)
{
	std::vector<Type> vec_tmp;

	if (srcVec.size() == 2)
	{
		resVec.emplace_back(vec_tmp);
		resVec.back().emplace_back(srcVec[0]);
		resVec.back().emplace_back(srcVec[1]);

		resVec.emplace_back(vec_tmp);
		resVec.back().emplace_back(srcVec[1]);
		resVec.back().emplace_back(srcVec[0]);
	}
	else
	{
		std::vector<Type>				vec_singles;
		std::vector<std::vector<Type>>	vec_vectors;

		for (size_t i = 0; i < srcVec.size(); i++)
		{
			for (size_t j = 0; j < srcVec.size(); j++)
				if (j != i)
					vec_singles.emplace_back(srcVec[j]);

			getPermutations(vec_vectors, vec_singles);

			for(auto vec = vec_vectors.begin(); vec != vec_vectors.end(); ++vec)
			{
				vec_tmp.emplace_back(srcVec[i]);

				for (auto v = vec->begin(); v != vec->end(); ++v)
					vec_tmp.emplace_back(*v);

				resVec.emplace_back(vec_tmp);
				vec_tmp.clear();
			}

			vec_singles.clear();
			vec_vectors.clear();
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Move Fibonacci sequence forward by 1
// f2 holds the resulting number
template<class Type>
void getNextFibonacci(Type& f1, Type& f2)
{
	Type tmp = f2;
	f2 = f2 + f1;
	f1 = tmp;
}

// -----------------------------------------------------------------------------------------------

// Factorial of a number
template<class Type>
Type getFactorial(Type num)
{
	if (num > 1)
		return num * getFactorial(num - 1);

	return 1;
}

// -----------------------------------------------------------------------------------------------

// Factorial of a number, with cache
template<class inType, class outType>
outType * getFactorial(std::map<inType, outType> &map, inType num)
{
	if (!map.count(num))
	{
		if (num > 1)
		{
			outType outNum(num);
			outType res = outNum * (*getFactorial(map, num-1));
			map[num] = std::move(res);
		}
		else
		{
			map[num] = outType(1);
		}
	}

	return &map[num];
}

// -----------------------------------------------------------------------------------------------

void getRightDigitRotations(size_t num, std::vector<size_t> &vec)
{
	size_t oldNum = num;
	vec.clear();
	vec.push_back(num);

	while (oldNum > 9)
	{
		size_t lastDigit = num % 10;
		size_t cnt = num = num / 10;

		while (cnt)
		{
			cnt /= 10;
			lastDigit *= 10;
		}

		num += lastDigit;
		vec.push_back(num);

		oldNum /= 10;
	}
}

// -----------------------------------------------------------------------------------------------

void reverseStr(std::string &str)
{
	char ch;
	size_t i = 0, j = str.length()-1;

	while (i < j)
	{
		ch = str[i];
		str[i++] = str[j];
		str[j--] = ch;
	}
}

// -----------------------------------------------------------------------------------------------

// Computes the Greatest Common Divisor of two non-negative integers
// https://en.wikipedia.org/wiki/Binary_GCD_algorithm
// Tags: Common, Divisor, Binary, GCD
size_t GCD_Stein(size_t a, size_t b)
{
	size_t shift = 0;

	if (a == 0 || b == 0)
		return a > b ? a : b;

	if (a == b)
		return a;

	while (((a | b) & 1) == 0)
	{
		shift++;
		a >>= 1;
		b >>= 1;
	}

	while ((a & 1) == 0)
		a >>= 1;

	// From here on, a is always odd
	do {

		while ((b & 1) == 0)
			b >>= 1;

		// Swap u and v
		if (a > b)
		{
			size_t tmp = a;
			a = b;
			b = tmp;
		}

		// Here b >= a
		b -= a;

	} while (b != 0);

	// Restore common factors of 2
	return a << shift;
}

// -----------------------------------------------------------------------------------------------

// Checks if n1 and n2 are permutations of each other
// Tags: permutation, number, digit
bool isPermutation(size_t n1, size_t n2)
{
	std::map<short, short> m1, m2;

	auto dissect = [](std::map<short, short> &map, size_t num) -> void
	{
		while (num)
		{
			short digit = num % 10;
			map[digit] = map.count(digit) ? map[digit] + 1 : 1;
			num /= 10;
		}
	};

	dissect(m1, n1);
	dissect(m2, n2);

	return m1 == m2;
}

// -----------------------------------------------------------------------------------------------

// Saves std::map<size_t, size_t> to file
void saveDataToFile_001(const char* fileName, std::map<size_t, size_t> &map, bool overwrite = true, bool isSilent = true)
{
	std::fstream f;
	auto flags = overwrite ? std::fstream::out : std::fstream::out | std::fstream::app;
	f.open(fileName, flags);

	if (f.is_open())
	{
		if( !isSilent)
			std::cout << "\n Writing data to file ... ";

		for (auto& iter : map)
			f << iter.first << " " << iter.second << std::endl;

		f.close();

		if (!isSilent)
			std::cout << "done!\n" << std::endl;
	}
}

// Gets std::map<size_t, size_t> from file
void loadDataFromFile_001(const char* fileName, std::map<size_t, size_t>& map)
{
	std::fstream f;
	f.open(fileName, std::fstream::in);

	if (f.is_open())
	{
		std::string line;
		std::vector<std::string> vec;

		while (std::getline(f, line))
		{
			if (line.length() && line[0] != '#')
			{
				split(line, vec);

				size_t N = std::atoi(vec[0].c_str());
				size_t n = std::atoi(vec[1].c_str());

				map.emplace(N, n);
			}
		}

		f.close();
	}
}

// Saves std::map<std::pair<size_t, size_t>, size_t> to file
void saveDataToFile_002(const char* fileName, std::map<std::pair<size_t, size_t>, size_t> &map, bool overwrite = true, bool isSilent = true)
{
	std::fstream f;
	auto flags = overwrite ? std::fstream::out : std::fstream::out | std::fstream::app;
	f.open(fileName, flags);

	if (f.is_open())
	{
		if (!isSilent)
			std::cout << "\n Writing data to file ... ";

		for (auto& iter : map)
			f << iter.first.first << " " << iter.first.second << " " << iter.second << std::endl;

		f.close();

		if (!isSilent)
			std::cout << "done!\n" << std::endl;
	}
}

// Gets std::map<std::pair<size_t, size_t>, size_t> from file
void loadDataFromFile_002(const char* fileName, std::map<std::pair<size_t, size_t>, size_t> &map)
{
	std::fstream f;
	f.open(fileName, std::fstream::in);

	if (f.is_open())
	{
		std::string line;
		std::vector<std::string> vec;

		while (std::getline(f, line))
		{
			if (line.length() && line[0] != '#')
			{
				split(line, vec);

				size_t p1 = std::atoi(vec[0].c_str());
				size_t p2 = std::atoi(vec[1].c_str());
				size_t n  = std::atoi(vec[2].c_str());

				map.emplace(std::make_pair(p1, p2), n);
			}
		}

		f.close();
	}
}

// -----------------------------------------------------------------------------------------------

// Returns the number of all possible ways to write N as a sum of values from VALUES
// Tags: summation, permutation
/*
	1 = 1;
	2 = 2; 2 = 1+1;
	3 = 3; 3 = 2+1; 3 = 1+1+1;
	4 = 4; 4 = 3+1; 4 = 2+2; 4 = 2+1+1; 4 = 1+1+1+1;
*/
void countSummations(size_t &res, const size_t N, std::vector<size_t> &VALUES, std::vector<size_t> &COEFF, size_t pos, size_t sum = 0, size_t MAX = 0)
{
	// Need this to 'reverse' VALUES vector, this will drastically decrease the number of iterations
	size_t POS = VALUES.size() - pos - 1;

	size_t max_Coeff_in_this_Pos = N / VALUES[POS];

	// Recursively iterate through all the possible combinations of coefficients
	for (COEFF[POS] = 0; COEFF[POS] <= max_Coeff_in_this_Pos; COEFF[POS]++)
	{
		size_t sumByNow = sum + COEFF[POS] * VALUES[POS];

		if (MAX)
		{
			if (VALUES[POS] > MAX && COEFF[POS] != 0)
				continue;
		}

		if (sumByNow <= N)
		{
			if (pos < VALUES.size() - 1)
			{

				// Continue recursion
				countSummations(res, N, VALUES, COEFF, pos + 1, sumByNow, MAX);

			}
			else
			{
				// Break out of recursion
				if (sumByNow == N)
				{
					res++;
#if 0
					for (size_t i = 0; i < COEFF.size(); i++)
						std::cout << "\t" << VALUES[i] << "x" << COEFF[i] << " ";
					std::cout << "\n" << std::endl;
#endif
				}
			}
		}
#if 0
		if (pos == 0)
			std::cout << " -- lev 0 finished: " << std::setw(3) << std::right << coeff << " of " << N << std::endl;
#endif
	}

	return;
}

// -----------------------------------------------------------------------------------------------

size_t getPartial(size_t rem, size_t num)
{
	std::vector<size_t> vecValues;

	for (size_t i = 1; i <= num; i++)
		vecValues.emplace_back(i);

	size_t res = 0;
	std::vector<size_t> COEFF(vecValues.size(), 0);

	countSummations(res, rem, vecValues, COEFF, 0, 0, num);

	return res;
}

// -----------------------------------------------------------------------------------------------

size_t getPartial2(size_t NUM, size_t MAX_VAL, std::mutex *mtx = nullptr)
{
	size_t total = 0;

	for (size_t i = 1; i <= MAX_VAL; i++)
	{
		size_t result = 0, num = NUM - i;

		if (i == 1 || i == NUM)
		{
			result = 1;
		}
		else
		{
			if (i >= num)
			{
				// Real bruteforce calculation
				result = getPartial(num, i);
			}
			else
			{
				// Meaning, get all variants for num, where all the numbers involved don't exceed i

				if (mtx)
				{
					static std::map<std::pair<size_t, size_t>, size_t> map;

					auto p = std::make_pair(num, i);

					if (map.count(p))
					{
						result = map[p];
					}
					else
					{
						result = getPartial2(num, i, mtx);

						std::lock_guard<std::mutex> lockData(*mtx);
							map[p] = result;
					}
				}
				else
				{
					result = getPartial2(num, i);
				}
			}
		}

		total += result;
	}

	return total;
}

// -----------------------------------------------------------------------------------------------

typedef std::map<std::pair<size_t, size_t>, size_t> myMap;

myMap map;

size_t getPartial3(size_t NUM, size_t MAX_VAL, std::mutex* mtx = nullptr, size_t level = 0)
{
	size_t total = 0;

	auto indent = [](size_t level)
	{
		if (level > 0)
			std::cout << std::endl;

		for (size_t i = 0; i < level+1; i++)
			std::cout << "  ";
	};

	auto checkCache = [](myMap &map, size_t N, size_t n) -> bool
	{
		auto p = std::make_pair(N, n);

		if (map.count(p))
			return true;

		return false;
	};

	auto cacheIt = [](myMap& map, size_t N, size_t n, size_t res)
	{
		auto p = std::make_pair(N, n);
		map[p] = res;
	};

	for (size_t i = 1; i <= MAX_VAL; i++)
	{
		size_t result = 0, num = NUM - i;

		if (i == 1 || i == NUM)
		{
			result = 1;
		}
		else
		{
			if (i >= num)
			{
				// Real bruteforce calculation
				indent(level);
				std::cout << "  gp3: calling getPartial(" << num << ", " << i << ") = ";

				if (checkCache(map, num, i))
				{
					std::cout << " --- cache hit! --- = ";
				}

				result = getPartial(num, i);

				cacheIt(map, num, i, result);

				std::cout << result << std::endl;
			}
			else
			{
				// Meaning, get all variants for num, where all the numbers involved don't exceed i

				if (mtx)
				{
/*
					static std::map<std::pair<size_t, size_t>, size_t> map;

					auto p = std::make_pair(num, i);

					if (map.count(p))
					{
						result = map[p];
					}
					else
					{
						result = getPartial3(num, i, mtx, level + 1);

						std::lock_guard<std::mutex> lockData(*mtx);
							map[p] = result;
					}
*/
				}
				else
				{
					indent(level);
					std::cout << "  gp3: recursion(" << num << ", " << i << ") = ...";

					if (checkCache(map, num, i))
					{
						std::cout << " --- cache hit! --- = ";
					}

					result = getPartial3(num, i, mtx, level + 1);

					cacheIt(map, num, i, result);

					indent(level);
					std::cout << "  ... = " << result << std::endl;
				}
			}
		}

		total += result;
	}

	return total;
}
