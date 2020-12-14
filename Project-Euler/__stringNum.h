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

		std::string&	get		()				{ return _str;						}
		size_t			asSizeT	()				{ return std::atoi(_str.c_str());	}

		stringNum  operator + (stringNum &);
		stringNum& operator +=(stringNum &);
		stringNum  operator + (size_t);
		stringNum  operator - (stringNum &);
		stringNum  operator - (size_t);
		stringNum  operator * (stringNum &);
		stringNum  operator * (size_t);
		stringNum  operator / (stringNum &);
		bool	   operator > (stringNum &)			const;
		bool	   operator >=(stringNum &)			const;
		bool	   operator > (size_t)				const;
		bool	   operator < (stringNum &)			const;
		bool	   operator <=(stringNum &)			const;
		bool	   operator < (size_t)				const;
		bool	   operator ==(const stringNum &)	const;
		bool	   operator ==(size_t);
		stringNum& operator ++();
		stringNum& operator --();

		explicit operator bool()					const;

	public:
static	size_t _precision;

	private:
		std::unique_ptr<char[]> add(const stringNum &, const stringNum &, size_t &);

		void mult_General	(const stringNum&, const stringNum&, stringNum&) const;
		void mult_Karatsuba	(stringNum&, stringNum&, stringNum& res, size_t minVal = 10) const;

	private:
		std::string _str;
};

size_t stringNum::_precision = 0;

// -----------------------------------------------------------------------------------------------

// Fast addition (10x times faster than before)
// Returns unique_ptr to char array and size_t & offset (which MUST be taken into account)
std::unique_ptr<char[]> stringNum::add(const stringNum &num1, const stringNum &num2, size_t &data_len)
{
	int overflow = 0;
	const std::string* pStr1 = &num1._str,
		* pStr2 = &num2._str;

	// Swap if needed, so the pStr1 points to the longest of the strings
	if (pStr1->length() < pStr2->length())
	{
		const std::string* pTmp = pStr2;
		pStr2 = pStr1;
		pStr1 = pTmp;
	}

	size_t len(pStr1->length()), dif(len - pStr2->length());

	data_len = len + 2;

	// RAII 'raw' memory to hold the result
	auto data = std::unique_ptr<char[]>(new char[data_len]);
	data[--data_len] = '\0';

	for (int i = static_cast<int>(len - 1); i >= 0; i--)
	{
		int num1 = int(pStr1->at(i) - 48);
		int num2 = i >= dif ? int(pStr2->at(i - dif) - 48) : 0;
		int nRes = num1 + num2 + overflow;

		data[--data_len] = char(nRes % 10 + 48);
		overflow = nRes > 9 ? 1 : 0;
	}

	if (overflow)
		data[--data_len] = '1';

	return data;
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator +(stringNum &other)
{
	size_t offset;
	return stringNum(add(*this, other, offset).get() + offset);
}

// -----------------------------------------------------------------------------------------------

stringNum & stringNum::operator +=(stringNum &other)
{
	size_t offset;
	this->_str = (add(*this, other, offset).get() + offset);
	return *this;
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

bool stringNum::operator >(stringNum &other) const
{
	if (_str.length() > other._str.length())
		return true;

	if (_str.length() < other._str.length())
		return false;

	return _str > other._str;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator >=(stringNum& other) const
{
	return (*this > other) || (*this == other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator >(size_t other) const
{
	return *this > stringNum(other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <=(stringNum& other) const
{
	return (*this < other) || (*this == other);
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <(stringNum& other) const
{
	if (_str.length() != other._str.length())
		return _str.length() < other._str.length();

	return _str < other._str;
}

// -----------------------------------------------------------------------------------------------

bool stringNum::operator <(size_t other) const
{
	return *this < stringNum(other);
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator *(stringNum& other)
{
	stringNum res;

	size_t len1 = this->get().length();
	size_t len2 = other.get().length();

	if (len1 > 300u && len2 > 300u)
	{
		// Need more experimenting
		// Seems that Karatsuba doesn't work really well if the diff in length of numbers is more than 20-25%
		mult_Karatsuba(*this, other, res, 1000000);
	}
	else
	{
		mult_General(*this, other, res);
	}

	return res;
}

// -----------------------------------------------------------------------------------------------

// General multiplication algorithm
void stringNum::mult_General(const stringNum &n1, const stringNum &n2, stringNum &res) const
{
	int cnt = 0, carryOver = 0;

	if (n1 && n2)
	{
		const std::string* s1 = &n1._str;
		const std::string* s2 = &n2._str;

		for (auto iter2 = s2->rbegin(); iter2 != s2->rend(); ++iter2)
		{
			std::string str;
			int i2 = *iter2 - 48;

			for (auto iter1 = s1->rbegin(); iter1 != s1->rend(); ++iter1)
			{
				int i1 = i2 * (*iter1 - 48) + carryOver;

				if (i1 > 9)
				{
					carryOver  = i1 / 10;
					i1 = i1 % 10;
				}
				else
				{
					carryOver = 0;
				}

				str = char(i1 + 48) + str;
			}

			if (carryOver)
			{
				str = char(carryOver + 48) + str;
				carryOver = 0;
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
}

// -----------------------------------------------------------------------------------------------

// Karatsuba fast multiplication
// Works faster with operands of 100+ digits length
// https://en.wikipedia.org/wiki/Karatsuba_algorithm
// minVal adjusts the value for which the conventional multiplication will be is used
void stringNum::mult_Karatsuba(stringNum& n1, stringNum& n2, stringNum& res, size_t minVal) const
{
	if (n1 < minVal || n2 < minVal)
	{
		mult_General(n1, n2, res);
		return;
	}

	size_t m1 = n1.get().length();
	size_t m2 = n2.get().length();

	// Get min of the two
	size_t m = m1 > m2 ? m2 : m1;

	m = m / 2;

	stringNum low1 = n1.get().substr(n1.get().length() - m);
	stringNum hig1 = n1.get().substr(0, n1.get().length() - m);

	stringNum low2 = n2.get().substr(n2.get().length() - m);
	stringNum hig2 = n2.get().substr(0, n2.get().length() - m);

	stringNum z0, z1, z2;

	mult_Karatsuba(low1, low2, z0, minVal);
	mult_Karatsuba(low1 + hig1, low2 + hig2, z1, minVal);
	mult_Karatsuba(hig1, hig2, z2, minVal);

	z1 = z1 - (z2 + z0);

	//	return (z2 * 10 ^ (m * 2)) + ((z1 - z2 - z0) * 10 ^ m2) + z0;

	for (size_t i = 0; i < m; i++)
	{
		z2.get() += "00";
		z1.get() += "0";
	}

	res = z2 + z1 + z0;
}

// -----------------------------------------------------------------------------------------------

stringNum stringNum::operator *(size_t other)
{
	return (*this) * stringNum(other);
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
	*this += stringNum(1);
	return *this;
}

// -----------------------------------------------------------------------------------------------

stringNum& stringNum::operator --()
{
	*this = *this - stringNum(1);
	return *this;
}

// -----------------------------------------------------------------------------------------------

stringNum::operator bool() const
{
	return _str != "0";
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

// Special pow function with overflow handling:
// 1. Calculate the power using native num types, until we've reached the point where overflow occurs (fast part)
// 2. Proceed calculating the power using our type (slow part)
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

// -----------------------------------------------------------------------------------------------

// Recursive pow function. Faster than stringNum_Pow().
stringNum stringNum_Pow_Recursive(size_t num, size_t pow)
{
	if (pow == 0)
		return stringNum(1);

	if (pow % 2 != 0)
		return stringNum_Pow_Recursive(num, pow-1) * num;

	stringNum tmp = stringNum_Pow_Recursive(num, pow/2);

	return tmp * tmp;
}

// -----------------------------------------------------------------------------------------------







class longNum {

	typedef int digitType;

	public:

		longNum() : _values(nullptr), _length(0)
		{
		}

		longNum(const char *str)
		{
			_length = strlen(str);

			_values = new digitType[_length];

			for (size_t i = 0; i < _length; i++)
			{
				_values[_length - i - 1] = (str[i] - 48);
			}
		}

		longNum(longNum&& other) noexcept
		{
			this->_length = std::move(other._length);
			this->_values = std::move(other._values);

			other._length = 0;
			other._values = nullptr;
		}

		~longNum()
		{
			if (_values)
			{
				delete[] _values;
				_values = nullptr;
				_length = 0;
			}
		}

		// Return number as a string in normal order
		std::string get()
		{
			std::string str;
			str.reserve(_length);

			for (size_t i = 0; i < _length; i++)
				str += static_cast<char>(_values[_length - i - 1] + 48);

			return str;
		}

		longNum operator +(const longNum &other)
		{
			const longNum* p1, * p2;
			longNum res;

			if (_length > other._length)
			{
				p1 = this;
				p2 = &other;
			}
			else
			{
				p1 = &other;
				p2 = this;
			}
			
			res._length = p1->_length;
			res._values = new digitType[p1->_length + 1];
			res._values[p1->_length] = 0;

			for (size_t i = 0; i < res._length; i++)
				res._values[i] = p1->_values[i] + (i < p2->_length ? p2->_values[i] : 0);

			res.normalize();

			return std::move(res);
		}

		longNum& longNum::operator =(longNum&& other) noexcept
		{
			if (this != &other)
			{
				if (_values)
					delete[] _values;

				this->_length = std::move(other._length);
				this->_values = std::move(other._values);

				other._length = 0;
				other._values = nullptr;
			}

			return *this;
		}

		void normalize()
		{
			for (size_t i = 0; i < _length; i++)
			{
				digitType nnn = _values[i];

				if (_values[i] >= 10)
				{
					digitType carryOver = _values[i] / 10;

					_values[i + 1] += carryOver;
					_values[  i  ] -= carryOver * 10;

					if (i + 1 >= _length)
						_length++;
				}
			}
		}

	private:

		digitType*	_values;
		size_t		_length;
};


