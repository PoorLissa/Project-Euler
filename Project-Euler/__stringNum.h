#pragma once

#include <string>

// -----------------------------------------------------------------------------------------------

class stringNum {

	public:

		stringNum()								: _str("0")					{ ; }
		stringNum(const char* str)				: _str(str)					{ ; }
		stringNum(const std::string& str)		: _str(str)					{ ; }
		stringNum(const int num)				: _str(std::to_string(num))	{ ; }
		stringNum(const size_t num)				: _str(std::to_string(num))	{ ; }
		stringNum(const long long num)			: _str(std::to_string(num))	{ ; }

		std::string& get() { return _str; }

		stringNum  operator  +(stringNum &);
		stringNum  operator  +(size_t);
		stringNum  operator  -(stringNum &);
		stringNum  operator  -(size_t);
		stringNum  operator  *(stringNum &);
		stringNum  operator  *(size_t);
		stringNum  operator  /(stringNum &);
		bool	   operator > (stringNum &);
		bool	   operator >=(stringNum &);
		bool	   operator > (size_t);
		bool	   operator < (stringNum &);
		bool	   operator <=(stringNum &);
		bool	   operator < (size_t);
		bool	   operator ==(const stringNum &) const;
		bool	   operator ==(size_t);
		stringNum& operator ++();

		explicit operator bool();

		size_t asSizeT() { return std::atoi(_str.c_str()); }

	public:
static	size_t _precision;

	private:
		std::string _str;
};

size_t stringNum::_precision = 0;

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator +(stringNum& other)
{
	std::string res, tmp,
		* pStr1 = &_str,
		* pStr2 = &other._str;

	if (_str.length() != other._str.length())
	{
		std::string* pStr;
		size_t len1 = _str.length(), len2 = other._str.length(), diff;

		if (len1 > len2)
		{
			tmp = other._str;
			pStr = pStr2 = &tmp;
			diff = len1 - len2;
		}
		else
		{
			tmp = _str;
			pStr = pStr1 = &tmp;
			diff = len2 - len1;
		}

		for (size_t i = 0; i < diff; i++)
			*pStr = '0' + *pStr;
	}

	size_t len = pStr1->length();
	int overflow = 0;

	for (size_t i = len; i > 0; i--)
	{
		int num1 = int(pStr1->at(i - 1) - 48);
		int num2 = int(pStr2->at(i - 1) - 48);
		int nRes = num1 + num2 + overflow;

		res = char(nRes % 10 + 48) + res;
		overflow = nRes > 9 ? 1 : 0;
	}

	if (overflow)
		res = '1' + res;

	return stringNum(res);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator +(size_t other)
{
	return *this + stringNum(other);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator -(stringNum &other)
{
	size_t zeroPos = 0;
	std::string res;

	if (_str == other._str)
	{
		res = "0";
	}
	else
	{
		std::string tmp, * pStr1 = &_str, * pStr2 = &other._str, *pStr;;
		bool isNegative(*this < other);

		if (_str.length() != other._str.length())
		{
			size_t len1 = _str.length(), len2 = other._str.length(), diff;

			if (len1 > len2)
			{
				tmp = other._str;
				pStr = pStr2 = &tmp;
				diff = len1 - len2;
			}
			else
			{
				tmp = _str;
				pStr = pStr1 = &tmp;
				diff = len2 - len1;
			}

			for (size_t i = 0; i < diff; i++)
				*pStr = '0' + *pStr;
		}

		size_t len = pStr1->length();
		int overflow = 0;

		if (isNegative)
		{
			pStr = pStr1;
			pStr1 = pStr2;
			pStr2 = pStr;
		}

		for (size_t i = len; i > 0; i--)
		{
			int nRes,
				num1 = int(pStr1->at(i - 1) - 48), 
				num2 = int(pStr2->at(i - 1) - 48) + overflow;

			if (num1 < num2)
			{
				overflow = 1;
				nRes = 10 + num1 - num2;
			}
			else
			{
				nRes = num1 - num2;
				overflow = 0;
			}

			res = char(nRes % 10 + 48) + res;

			if (res[0] == '0')
				zeroPos = zeroPos ? zeroPos : i;
			else
				zeroPos = 0;
		}

		if (isNegative)
		{
			if (zeroPos)
				res[--zeroPos] = '-';
			else
				res = '-' + res;
		}
	}

	return stringNum(res.c_str() + zeroPos);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator -(size_t other)
{
	return *this - stringNum(other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator >(stringNum &other)
{
	if (_str.length() > other._str.length())
		return true;

	if (_str.length() < other._str.length())
		return false;

	return _str > other._str;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator >=(stringNum& other)
{
	return (*this > other) || (*this == other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator >(size_t other)
{
	return *this > stringNum(other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <=(stringNum& other)
{
	return (*this < other) || (*this == other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <(stringNum& other)
{
	if (_str.length() != other._str.length())
		return _str.length() < other._str.length();

	return _str < other._str;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <(size_t other)
{
	return *this < stringNum(other);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator *(stringNum &other)
{
	int cnt = 0, overflow = 0;
	stringNum res;

	if (*this && other)
	{
		std::string* s1 = &_str;
		std::string* s2 = &other._str;

		for (auto iter2 = s2->rbegin(); iter2 != s2->rend(); ++iter2)
		{
			std::string str;
			int i2 = *iter2 - 48;

			for (auto iter1 = s1->rbegin(); iter1 != s1->rend(); ++iter1)
			{
				int i1 = i2 * (*iter1 - 48) + overflow;

				if (i1 > 9)
				{
					overflow = i1 / 10;
					i1 = i1 % 10;
				}
				else
				{
					overflow = 0;
				}

				str = char(i1 + 48) + str;
			}

			if (overflow)
			{
				str = char(overflow + 48) + str;
				overflow = 0;
			}

			if (cnt)
			{
				for (int i = 0; i < cnt; i++)
					str.push_back('0');

				res = res + stringNum(str);
			}
			else
			{
				res._str = str;
			}

			cnt++;
		}
	}

	return res;
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator *(size_t other)
{
	return *this * stringNum(other);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator /(stringNum &other)
{
	stringNum tmp(*this), res("0"), one("1");

	while ( 1 )
	{
		if (tmp < other)
			break;

		tmp = tmp - other;
		res = res + one;
	}

	if (_precision)
	{
		stringNum dec("0");
		size_t precision = _precision + 1;
		size_t prev = 0;

		while (precision--)
		{
			if(tmp._str[0] != '0')
				tmp._str += "0";

			size_t res2 = 0;

			while (1)
			{
				if (tmp < other)
					break;

				tmp = tmp - other;
				res2++;
			}

			if (precision > 1)
			{
				if (precision == _precision)
					dec._str[0] = char(res2) + 48;
				else
					dec._str += char(res2) + 48;
			}
			else
			{
				if (precision == 1)
					prev = res2;
				else
				{
					if (res2 < 5)
					{
						dec._str += char(prev) + 48;
					}
					else
					{
						prev++;

						if(prev < 10)
							dec._str += char(prev) + 48;
						else
						{
							dec = dec + one;
						}
					}
				}
			}
		}

		if( dec > one )
			res._str += "." + dec._str;
	}

	return res;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator ==(const stringNum &other) const
{
	return _str == other._str;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator ==(size_t other)
{
	return *this == stringNum(other);
}

// -----------------------------------------------------------------------------------------------

stringNum& stringNum::operator ++()
{
	*this = *this + stringNum(1);
	return *this;
}

// -----------------------------------------------------------------------------------------------

stringNum::operator bool()
{
	return _str != "0";
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

// Special pow function with overflow handling
stringNum stringNum_Pow(size_t num, size_t pow)
{
	size_t i = 0;
	unsigned long long longRes = 1u, old = 0u, max = unsigned long long(-1) / num;

	for (i = 0; i < pow; i++)
	{
		old = longRes * num;

		if (old <= max)
			longRes = old;
		else
			break;
	}

	stringNum res(longRes);

	if (i != pow)
	{
		stringNum strNum(num);

		for (; i < pow; i++)
			res = res * strNum;
	}

	return res;
}


