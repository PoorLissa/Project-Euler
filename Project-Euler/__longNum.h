#pragma once

#include <string>

//#define _TRACE_

// -----------------------------------------------------------------------------------------------

constexpr bool POS = true;
constexpr bool NEG = false;

// -----------------------------------------------------------------------------------------------

class longNum {

		typedef short digitType;
		static const digitType BASE = 10u;


	public:

		longNum() : _values(nullptr), _length(0), _sign(POS) { ; }
	   ~longNum();


		// Constructors
		longNum(const char *);
		longNum(const std::string& str) : longNum(str.c_str()) { ; }
		longNum(longNum  &);
		longNum(longNum &&) noexcept;
		longNum(size_t);
		longNum(long);

		// test


		// Operators
		longNum &	operator = (longNum  &);
		longNum &	operator = (longNum &&) noexcept;
		longNum &	operator = (size_t);

		bool		operator ==(const longNum &)	const;
		bool		operator !=(const longNum &)	const;

		bool		operator > (const longNum &)	const;
		bool		operator >=(const longNum &)	const;
		bool		operator < (const longNum &)	const;
		bool		operator <=(const longNum &)	const;

		longNum		operator + (const longNum &)	const;
		longNum &	operator +=(const longNum &);
		longNum &	operator +=(	  longNum &&) noexcept;

		longNum		operator - (const longNum &)	const;
		longNum &	operator -=(const longNum &);

		template <class Type> bool		operator ==(const Type) const;
		template <class Type> bool		operator !=(const Type) const;

		template <class Type> bool		operator > (const Type) const;
		template <class Type> bool		operator >=(const Type) const;
		template <class Type> bool		operator < (const Type) const;
		template <class Type> bool		operator <=(const Type) const;

		template <class Type> longNum	operator + (const Type) const;
		template <class Type> longNum &	operator +=(const Type);
		template <class Type> longNum	operator - (const Type) const;

		explicit operator bool() const;


		// Getters
		std::string get() const;


	private:
		void   add2positive	(const longNum &, const longNum &, longNum &)	const;			// Add 2 positive numbers
		bool subtr2positive	(const longNum &, const longNum &, longNum &)	const;			// Subtract 2 positive numbers

		int absValueIsLarger(const longNum &, const longNum &)				const;			// Operator > for absolute values of the 2 numbers

	private:
		digitType*	_values;
		size_t		_length;
		bool		_sign;

	public:
		static size_t cnt;
};

size_t longNum::cnt = 0;

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

longNum::longNum(const char *str) : _length(strlen(str)), _sign(POS)
{
	#if defined _TRACE_
		std::cout << " ---> Alloc" << std::endl;
	#endif

	if (_length && str[0] == '-')
	{
		_sign = NEG;
		_length--;
		str++;
	}

	_values = new digitType[_length];

	for (size_t i = 0; i < _length; i++)
	{
		_values[_length - i - 1] = str[i] - 48;
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(longNum& other) : _length(other._length)
{
#if defined _TRACE_
	std::cout << " ---> Alloc" << std::endl;
#endif
	_values = new digitType[_length];
	memcpy(_values, other._values, sizeof(digitType) * _length);
	_sign = other._sign;
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(longNum&& other) noexcept
{
	_length = std::move(other._length);
	_values = std::move(other._values);
	_sign = other._sign;

	other._length = 0;
	other._values = nullptr;
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(size_t num) : _length(0u), _sign(POS)
{
	size_t i = 0, tmp = num;

	if (num == 0)
	{
#if defined _TRACE_
		std::cout << " ---> Alloc" << std::endl;
#endif
		_length = 1;
		_values = new digitType[_length];
		_values[0] = 0u;
	}
	else
	{
		while (tmp && ++_length)
			tmp /= BASE;

		if (_length)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc" << std::endl;
#endif
			_values = new digitType[_length];
		}

		while (num)
		{
			digitType digit = num % BASE;
			_values[i++] = digit;
			num /= BASE;
		}
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(long num) : _length(0u), _sign(POS)
{
	size_t i = 0;
	long tmp = num;

	if (num == 0)
	{
#if defined _TRACE_
		std::cout << " ---> Alloc" << std::endl;
#endif
		_length = 1;
		_values = new digitType[_length];
		_values[0] = 0u;
	}
	else
	{
		if (num < 0)
		{
			_sign = NEG;
			num = -num;
		}

		while (tmp && ++_length)
			tmp /= BASE;

		if (_length)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc" << std::endl;
#endif
			_values = new digitType[_length];
		}

		while (num)
		{
			digitType digit = num % BASE;
			_values[i++] = digit;
			num /= BASE;
		}
	}
}

// -----------------------------------------------------------------------------------------------

longNum::~longNum()
{
	if (_values)
	{
		delete[] _values;
		_values = nullptr;
		_length = 0;
	}
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(longNum& other)
{
	if (this != &other)
	{
		if (_values)
		{
			// Will reuse the memory if the length is the same
			if (_length != other._length)
			{
				delete[] _values;
				_values = nullptr;
			}
		}

		_length = other._length;
		_sign   = other._sign;

		if (!_values)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc" << std::endl;
#endif
			_values = new digitType[_length];
		}

		memcpy(_values, other._values, sizeof(digitType) * _length);
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(longNum&& other) noexcept
{
	if (this != &other)
	{
		if (_values)
			delete[] _values;

		// Steal the data
		_length = std::move(other._length);
		_values = std::move(other._values);

		_sign = other._sign;

		// Leave the other object in empty state
		other._length = 0;
		other._values = nullptr;
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(size_t other)
{
	*this = longNum(other);

	return *this;
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator ==(const longNum& other) const
{
	if (_length == other._length && _sign == other._sign)
	{
		for (size_t i = 0; i < _length; ++i)
			if (_values[i] != other._values[i])
				return false;

		return true;
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator ==(const Type other) const
{
	return *this == longNum(other);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator !=(const longNum &other) const
{
	return !(*this == other);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator !=(const Type other) const
{
	return *this != longNum(other);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator >(const longNum &other) const
{
	if(_sign > other._sign)
		return true;

	if (_sign < other._sign)
		return false;

	if (_length > other._length)
		return _sign;

	if (_length < other._length)
		return !_sign;

	if (_length)
	{
		for (size_t i = _length-1; i != size_t(-1); --i)
		{
			if (_values[i] > other._values[i])
				return _sign;

			if (_values[i] < other._values[i])
				return !_sign;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >(const Type other) const
{
	longNum obj(other);

	return *this > obj;
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator >=(const longNum &other) const
{
	if (_sign > other._sign)
		return true;

	if (_sign < other._sign)
		return false;

	if (_length > other._length)
		return _sign;

	if (_length < other._length)
		return !_sign;

	if (_length)
	{
		for (size_t i = _length - 1; i != size_t(-1); --i)
		{
			if (_values[i] > other._values[i])
				return _sign;

			if (_values[i] < other._values[i])
				return !_sign;
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >=(const Type other) const
{
	longNum obj(other);

	return *this >= obj;
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <(const longNum &other) const
{
	if (_sign < other._sign)
		return true;

	if (_sign > other._sign)
		return false;

	if (_length < other._length)
		return _sign;

	if (_length > other._length)
		return !_sign;

	if (_length)
	{
		for (size_t i = _length - 1; i != size_t(-1); --i)
		{
			if (_values[i] < other._values[i])
				return _sign;

			if (_values[i] > other._values[i])
				return !_sign;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <(const Type other) const
{
	longNum obj(other);

	return *this < obj;
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <=(const longNum &other) const
{
	if (_sign < other._sign)
		return true;

	if (_sign > other._sign)
		return false;

	if (_length < other._length)
		return _sign;

	if (_length > other._length)
		return !_sign;

	if (_length)
	{
		for (size_t i = _length - 1; i != size_t(-1); --i)
		{
			if (_values[i] < other._values[i])
				return _sign;

			if (_values[i] > other._values[i])
				return !_sign;
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <=(const Type other) const
{
	longNum obj(other);

	return *this <= obj;
}

// -----------------------------------------------------------------------------------------------

longNum longNum::operator +(const longNum &other) const
{
	longNum res;

	if (_sign == other._sign)
	{
		add2positive(*this, other, res);
		res._sign = _sign;
	}
	else
	{
		subtr2positive(*this, other, res);
	}

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
longNum longNum::operator +(const Type other) const
{
	longNum obj(other);

	return std::move(*this + obj);
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(const longNum &other)
{
	if (_sign == other._sign)
	{
		// In case the sign is the same, we add the numbers and keep the sign.
		// Also, there's a chance we'll be able to add the 2 numbers without reallocation
		if (_length < other._length)
		{
			// Go with full alloc cycle
			*this = *this + other;
		}
		else
		{
			// Perform adding in place
			digitType carryOver = 0u, nRes = 0u;

			for (size_t i = 0; i < _length; ++i)
			{
				nRes = _values[i] + carryOver;

				if (i < other._length)
					nRes += other._values[i];

				_values[i] = nRes % BASE;

				carryOver = (nRes >= BASE) ? 1u : 0u;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc" << std::endl;
#endif
				digitType* newVal = new digitType[_length + 1u];

				memcpy(newVal, _values, sizeof(digitType) * _length);

				newVal[_length++] = carryOver;

				delete[] _values;
				_values = newVal;
			}
		}
	}
	else
	{
		// In case the signs are opposite, we subtract smaller number from the greater and keep the sign of the greater one

		// First, we need to determine which number's absolute value is greater
		int isGreater = absValueIsLarger(*this, other);

		// The numbers are equal, return 0
		if (!isGreater)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc for '0' value" << std::endl;
#endif
			// TODO: don't reallocate if _length is relatively small
			// Just set _length to 1 and Values[] to 0
			// Need to test it later
			if (_length > 1)
			{
				delete[] _values;
				_values = new digitType[1];
			}

			_length = 1;
			_values[0] = 0;
			_sign = POS;
		}
		else
		{
			const longNum* pn1(this);
			const longNum* pn2(&other);

			digitType* res = pn1->_values;

			size_t i = 0, cnt = 0;

			if (isGreater == 2)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc tmp buffer" << std::endl;
#endif
				// This < other: Will need another buffer for the result
				pn1 = &other;
				pn2 = this;

				res = new digitType[pn1->_length];

				res[0] = pn1->_values[0];

				for (; i < pn2->_length; ++i)
				{
					if (i < pn2->_length)
						res[i+1] = pn1->_values[i+1];

					while (res[i] < pn2->_values[i])
					{
						res[i+1]--;
						res[i] += BASE;
					}

					res[i] -= pn2->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				for (; i < pn1->_length; ++i)
				{
					res[i+1] = pn1->_values[i+1];

					while (res[i] < 0)
					{
						res[i+1]--;
						res[i] += BASE;
					}

					cnt = res[i] ? 0 : cnt + 1;
				}

				_length = pn1->_length - cnt;				// Adjust the length, so leading zeroes will be trimmed out

				delete[] _values;
				_values = res;
			}
			else
			{
				// This > other: No need for realloc. Subtract numbers in place
				for (; i < pn2->_length; ++i)
				{
					while (res[i] < pn2->_values[i])
					{
						res[i+1]--;
						res[i] += BASE;
					}

					res[i] = pn1->_values[i] - pn2->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				for (; i < pn1->_length; ++i)
				{
					while (pn1->_values[i] < 0)
					{
						pn1->_values[i+1]--;
						pn1->_values[i] += BASE;
					}

					res[i] = pn1->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				_length -= cnt;								// Adjust the length, so leading zeroes will be trimmed out
			}

			_sign = pn1->_sign;								// Sign is determined by the greater number (in absolute terms)
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(longNum&& other) noexcept
{
	// We can steal the data from other number, if it is larger than this one
	if (_length < other._length)
	{
		size_t	   tmpLength = _length;
		digitType* tmpValues = _values;
		bool	   tmpSign   = _sign;

		_length = other._length;
		_sign   = other._sign;
		_values = std::move(other._values);

		other._length = tmpLength;
		other._sign   = tmpSign;
		other._values = std::move(tmpValues);
	}

	*this += other;

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
longNum& longNum::operator +=(const Type other)
{
	*this += longNum(other);

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum longNum::operator -(const longNum& other) const
{
	longNum res;

	if (_sign == other._sign)
	{
		// a < b -- change sign
		// a > b -- ok
		if (subtr2positive(*this, other, res))
			res._sign = !res._sign;
	}
	else
	{
		add2positive(*this, other, res);
		res._sign = _sign;
	}

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
longNum	longNum::operator -(const Type other) const
{
	return std::move(*this - longNum(other));
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator -=(const longNum& other)
{
	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum::operator bool() const
{
	return _length != 1 || _values[0] != 0;
}

// -----------------------------------------------------------------------------------------------

// Return number as a string in a normal readable order
std::string longNum::get() const
{
	std::string str;

	if (_values)
	{
		str.reserve(_sign == NEG ? _length+1 : _length);

		if(_sign == NEG)
			str += '-';

		for (size_t i = 0; i < _length; i++)
			str += static_cast<char>(_values[_length - i - 1] + 48);
	}
	else
	{
		str = "0";
	}

	return str;
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive numbers
void longNum::add2positive(const longNum &n1, const longNum &n2, longNum &res) const
{
	// The longest number will be the first
	const longNum* p1(&n1);
	const longNum* p2(&n2);

	if (n1._length < n2._length)
	{
		p1 = &n2;
		p2 = &n1;
	}

	#if defined _TRACE_
		std::cout << " ---> Alloc" << std::endl;
	#endif

	res._length = p1->_length;
	res._values = new digitType[p1->_length + 1u];
	res._values[p1->_length] = 0u;

	digitType carryOver = 0u, nRes = 0u;

	for (size_t i = 0; i < res._length; ++i)
	{
		// This way the code is slightly faster than with a ternary operator
		{
			nRes = p1->_values[i] + carryOver;

			if (i < p2->_length)
				nRes += p2->_values[i];
		}

		res._values[i] = nRes % BASE;

		carryOver = (nRes >= BASE) ? 1u : 0u;
	}

	if (carryOver)
	{
		res._values[res._length++] = 1u;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers
// Returns:
//	false : n1 >= n2
//	true  : n1 <  n2
bool longNum::subtr2positive(const longNum &n1, const longNum &n2, longNum &res) const
{
	bool bRes = true;

#if defined _TRACE_
	std::cout << " ---> Alloc" << std::endl;
#endif
	const longNum* pn1(&n2);
	const longNum* pn2(&n1);

	// First, we need to determine which number's absolute value is greater
	int isGreater = absValueIsLarger(n1, n2);

	// The numbers are equal: just return 0
	if (!isGreater)
	{
		res._length = 1;
		res._values = new digitType[1];
		res._values[0] = 0;
		return false;
	}

	if (isGreater == 1)
	{
		pn1 = &n1;
		pn2 = &n2;
		bRes = false;
	}

	res._length = pn1->_length;
	res._values = new digitType[pn1->_length];

	size_t i = 0, cnt = 0;

	for (; i < pn2->_length; ++i)
	{
		while (pn1->_values[i] < pn2->_values[i])
		{
			pn1->_values[i+1]--;
			pn1->_values[i] += BASE;
		}

		res._values[i] = pn1->_values[i] - pn2->_values[i];

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	for (; i < pn1->_length; ++i)
	{
		while(pn1->_values[i] < 0)
		{
			pn1->_values[i+1]--;
			pn1->_values[i] += BASE;
		}

		res._values[i] = pn1->_values[i];

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	res._sign = pn1->_sign;						// Sign is determined by the greater number
	res._length -= cnt;							// Adjust the length, so leading zeroes will be trimmed out

	return bRes;
}

// -----------------------------------------------------------------------------------------------

// Operator > for absolute values of the 2 numbers
// Returns:
// n1  > n2	-- 1
// n1  < n2	-- 2
// n1 == n2	-- 0
int longNum::absValueIsLarger(const longNum &n1, const longNum &n2) const
{
	if (n1._length > n2._length)
		return 1;

	if (n1._length < n2._length)
		return 2;

	if (n1._length)
	{
		for (size_t i = n1._length - 1; i != size_t(-1); --i)
		{
			if (n1._values[i] > n2._values[i])
				return 1;

			if (n1._values[i] < n2._values[i])
				return 2;
		}
	}

	// At this point we know the 2 numbers are equal
	return 0;
}

// -----------------------------------------------------------------------------------------------
