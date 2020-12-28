#pragma once

// Fast copying via the use of hardware
// Mb check this out later
// https://codereview.stackexchange.com/questions/5520/copying-80-bytes-as-fast-as-possible

#include <string>

//#define _TRACE_

// -----------------------------------------------------------------------------------------------

constexpr bool POS = true;
constexpr bool NEG = false;

// -----------------------------------------------------------------------------------------------

class longNum {

		typedef short digitType;
		static const digitType BASE = 10;

	public:

		longNum() : _values(nullptr), _length(0), _sign(POS) { ; }
	   ~longNum();


		// Constructors
		longNum(const char *);
		longNum(const std::string& str) : longNum(str.c_str()) { ; }
		longNum(const longNum  &);
		longNum(	  longNum &&) noexcept;
		longNum(const size_t);
		longNum(const long);


		// Operators
		longNum &	operator = (const longNum  &);
		longNum &	operator = (	  longNum &&) noexcept;

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
		longNum &	operator -=(	  longNum &&) noexcept;

		longNum &	operator ++();
		longNum &	operator --();

		template <class Type> longNum & operator = (const Type);

		template <class Type> bool		operator ==(const Type) const;
		template <class Type> bool		operator !=(const Type) const;

		template <class Type> bool		operator > (const Type) const;
		template <class Type> bool		operator >=(const Type) const;
		template <class Type> bool		operator < (const Type) const;
		template <class Type> bool		operator <=(const Type) const;

		template <class Type> longNum	operator + (const Type) const;
		template <class Type> longNum &	operator +=(const Type);
		template <class Type> longNum	operator - (const Type) const;
		template <class Type> longNum & operator -=(const Type);

		explicit operator bool() const;


		// Getters
		std::string get() const;

	private:
		void   add2positive	(const longNum &, const longNum &, longNum &)	const;					// Add 2 positive numbers
		bool subtr2positive	(const longNum &, const longNum &, longNum &)	const;					// Subtract 2 positive numbers

		template <class Type> void   add2positive(const longNum &, const Type, longNum &) const;
		template <class Type> bool subtr2positive(const longNum &, const Type, longNum &) const;

		int absValueIsLarger(const longNum &, const longNum &) const;								// Operator > for absolute values of the 2 numbers

	private:
		digitType*	_values;
		size_t		_length;
		bool		_sign;
};

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

longNum::longNum(const longNum& other) : _length(other._length)
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
	size_t i = 0, tmp(num);

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
	long tmp(num);

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

longNum& longNum::operator =(const longNum& other)
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

template <class Type>
longNum& longNum::operator =(const Type other)
{
	return *this = longNum(other);
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
#if 0
	// This one works ~13 times slower
	return *this == longNum(other);
#else
	// Check sign and length first (max size_t is 18446744073709551615 ==> 20 digits max)
	if (_sign != (other >= 0) || _length > 20u)
		return false;

	// Going to actually change const Type other, as it is passed by value and doesn't affect anything
	Type* ptr = const_cast<Type*>(&other);
	int len(0);

	if (!_sign)
		*ptr *= -1;

	while (other)
	{
		if (_length == len)
			return false;

		if ((other % BASE) != (_values[len++]))
			return false;

		*ptr /= BASE;
	}

	if (_length != len)
	{
		if (len == 0 && !*this)							// If other == 0, len will be 0
			return true;

		return false;
	}

	return true;
#endif
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
	return !(*this == other);
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
#if 0
	// ~7.6 times slower
	return *this > longNum(other);
#else
	if ((_sign != (other >= 0)) || _length > 20u)
		return _sign;

	// Going to actually change const Type other, as it is passed by value and doesn't affect anything
	Type* ptr = const_cast<Type*>(&other);
	int len(0);
	bool res(false);

	if (!_sign)
		*ptr *= -1;

	while (other)
	{
		if (_length == len)
			return !_sign;

		if (_values[len] != other % BASE)
			res = _values[len] > other % BASE ? _sign : !_sign;

		*ptr /= BASE;
		len++;
	}

	if (_length > len)
	{
		if (len == 0 && !*this)
			return false;

		return _sign;
	}

	return res;
#endif
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
#if 0
	// ~7.9 times slower
	return *this >= longNum(other);
#else
	if ((_sign != (other >= 0)) || _length > 20u)
		return _sign;

	// Going to actually change const Type other, as it is passed by value and doesn't affect anything
	Type* ptr = const_cast<Type*>(&other);
	int len(0);
	bool res(true);

	if (!_sign)
		*ptr *= -1;

	while (other)
	{
		if (_length == len)
			return !_sign;

		if (_values[len] != other % BASE)
			res = _values[len] > other % BASE ? _sign : !_sign;

		*ptr /= BASE;
		len++;
	}

	if (_length > len)
	{
		if (len == 0 && !*this)
			return true;

		return _sign;
	}

	return res;
#endif
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
#if 0
	// 7.7 times slower
	return *this < longNum(other);
#else
	if ((_sign != (other >= 0)) || _length > 20u)
		return !_sign;

	// Going to actually change const Type other, as it is passed by value and doesn't affect anything
	Type* ptr = const_cast<Type*>(&other);
	int len(0);
	bool res(false);

	if (!_sign)
		*ptr *= -1;

	while (other)
	{
		if (_length == len)
			return _sign;

		if (_values[len] != other % BASE)
			res = _values[len] < other % BASE ? _sign : !_sign;

		*ptr /= BASE;
		len++;
	}

	if (_length > len)
	{
		if (len == 0 && !*this)
			return false;

		return !_sign;
	}

	return res;
#endif
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
#if 0
	// 7.5 times slower
	return *this <= longNum(other);
#else
	if ((_sign != (other >= 0)) || _length > 20u)
		return !_sign;

	// Going to actually change const Type other, as it is passed by value and doesn't affect anything
	Type* ptr = const_cast<Type*>(&other);
	int len(0);
	bool res(true);

	if (!_sign)
		*ptr *= -1;

	while (other)
	{
		if (_length == len)
			return _sign;

		if (_values[len] != other % BASE)
			res = _values[len] < other % BASE ? _sign : !_sign;

		*ptr /= BASE;
		len++;
	}

	if (_length > len)
	{
		if (len == 0 && !*this)
			return true;

		return !_sign;
	}

	return res;
#endif
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

// TODO: don't need to build new object longNum from 'other'. can be done faster. test it later
template <class Type>
longNum longNum::operator +(const Type other) const
{
#if 0
	return std::move(*this + longNum(other));
#else
	longNum res;

	if (_sign == (other >= 0))
	{
		add2positive(*this, other, res);
		res._sign = _sign;
	}
	else
	{
		subtr2positive(*this, other, res);
	}

	return std::move(res);
#endif
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(const longNum &other)
{
#if 0
	// This code is slower (Tested)
	*this = *this + other;
#else
	if (_sign == other._sign)
	{
		// In case the sign is the same, we add the numbers and keep the sign.
		// Also, there's a chance we'll be able to add the 2 numbers without reallocation
		if (_length < other._length)
		{
			// Go with full alloc cycle
			*this = std::move(*this + other);
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

			size_t i(0), cnt(0);

			if (isGreater == 2)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc tmp buffer" << std::endl;
#endif
				// This < other: Will need another buffer for the result
				pn1 = &other;
				pn2 = this;

				size_t len_minus_one = pn1->_length - 1;

				res = new digitType[pn1->_length];
				res[0] = pn1->_values[0];

				for (; i < pn2->_length; ++i)
				{
					if (i < len_minus_one)
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
					if (i < len_minus_one)
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
#endif

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

// TODO: don't need to build new object longNum from 'other'. can be done faster. test it later
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

// TODO: don't need to build new object longNum from 'other'. can be done faster. test it later
template <class Type>
longNum	longNum::operator -(const Type other) const
{
	return std::move(*this - longNum(other));
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator -=(const longNum& other)
{
#if 0
	*this = *this - other;
#else
	// (a -= b) <==> (a += -b)
	// So copy-paste from operator +=, adjusted for the sign
	if (_sign != other._sign)
	{
		// In case the sign is the same, we add the numbers and keep the sign.
		// Also, there's a chance we'll be able to add the 2 numbers without reallocation
		if (_length < other._length)
		{
			// Go with full alloc cycle
			*this = std::move(*this - other);
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

			size_t i(0), cnt(0);

			if (isGreater == 2)
			//if (_length < other._length)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc tmp buffer" << std::endl;
#endif
				// This < other: Will need another buffer for the result
				pn1 = &other;
				pn2 = this;

				size_t len_minus_one = pn1->_length - 1;

				res = new digitType[pn1->_length];
				res[0] = pn1->_values[0];

				for (; i < pn2->_length; ++i)
				{
					if (i < len_minus_one)
						res[i + 1] = pn1->_values[i + 1];

					while (res[i] < pn2->_values[i])
					{
						res[i + 1]--;
						res[i] += BASE;
					}

					res[i] -= pn2->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				for (; i < pn1->_length; ++i)
				{
					if (i < len_minus_one)
						res[i + 1] = pn1->_values[i + 1];

					while (res[i] < 0)
					{
						res[i + 1]--;
						res[i] += BASE;
					}

					cnt = res[i] ? 0 : cnt + 1;
				}

				_length = pn1->_length - cnt;				// Adjust the length, so leading zeroes will be trimmed out

				delete[] _values;
				_values = res;
				_sign = !pn1->_sign;
			}
			else
			{
				// This > other: No need for realloc. Subtract numbers in place
				for (; i < pn2->_length; ++i)
				{
					while (res[i] < pn2->_values[i])
					{
						res[i + 1]--;
						res[i] += BASE;
					}

					res[i] = pn1->_values[i] - pn2->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				for (; i < pn1->_length; ++i)
				{
					while (pn1->_values[i] < 0)
					{
						pn1->_values[i + 1]--;
						pn1->_values[i] += BASE;
					}

					res[i] = pn1->_values[i];
					cnt = res[i] ? 0 : cnt + 1;
				}

				_length -= cnt;								// Adjust the length, so leading zeroes will be trimmed out
				_sign = pn1->_sign;
			}
		}
	}
#endif

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator -=(longNum&& other) noexcept
{
	// (a -= b) <==> (a += -b)
	// And we don't care about 'other', as it is an RValue
	other._sign = !other._sign;

	return *this += std::move(other);
}

// -----------------------------------------------------------------------------------------------

// TODO: don't need to build new object longNum from 'other'. can be done faster. test it later
template <class Type>
longNum& longNum::operator -=(const Type other)
{
	*this -= longNum(other);

	return *this;
}

// -----------------------------------------------------------------------------------------------

// This implementation is almost 2 times faster than standard "return *this += longNum(1l)"
longNum& longNum::operator ++()
{
	if (_length)
	{
		// General case
		if (_sign)
		{
			if (_values[0] != 9)
			{
				// It is the same as within the for-loop, but without the cycle -- should be a bit faster
				_values[0]++;
			}
			else
			{
				_values[0] = 0;

				for (size_t i = 1; i < _length; ++i)
				{
					if (_values[i] != 9)
					{
						_values[i]++;
						return *this;
					}

					_values[i] = 0;
				}

#if defined _TRACE_
				std::cout << " ---> Alloc for " << _length + 1 << " digits" << std::endl;
#endif
				// In case we reached this point, the array is full of '9's
				// Will need to realloc: 999 + 1 = 1000
				delete[] _values;

				_values = new digitType[_length + 1];
				memset(_values, 0, _length*sizeof(_values[0]));
				_values[_length] = 1;
				_length++;
			}
		}
		else
		{
			if (_values[0] != 0)
			{
				_values[0]--;

				if (_values[0] == 0 && _length == 1)
					_sign = POS;
			}
			else
			{
				_values[0] = 9;

				for (size_t i = 1; i < _length; ++i)
				{
					if (_values[i] != 0)
					{
						_values[i]--;

						if (_values[i] == 0 && i == _length - 1)
							_length--;

						return *this;
					}

					_values[i] = 9;
				}
			}
		}
	}
	else
	{
		// Deep zero state. Will return 1
#if defined _TRACE_
		std::cout << " ---> Alloc for zero" << std::endl;
#endif
		if (_values)
			delete[] _values;

		_values = new digitType[1];
		_values[0] = 1;
		_length = 1;
		_sign = POS;
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator --()
{
	if (_length)
	{
		// General case
		if (_sign)
		{
			if (_values[0] != 0)
			{
				_values[0]--;
			}
			else
			{
				if (_length > 1)
				{
					_values[0] = 9;

					for (size_t i = 1; i < _length; ++i)
					{
						if (_values[i] != 0)
						{
							_values[i]--;

							if (_values[i] == 0 && i == _length - 1)
								_length--;

							return *this;
						}

						_values[i] = 9;
					}
				}
				else
				{
					_sign = NEG;
					_values[0] = 1;
				}
			}
		}
		else
		{
			if (_values[0] != 9)
			{
				_values[0]++;
			}
			else
			{
				_values[0] = 0;

				for (size_t i = 1; i < _length; ++i)
				{
					if (_values[i] != 9)
					{
						_values[i]++;
						return *this;
					}

					_values[i] = 0;
				}

#if defined _TRACE_
				std::cout << " ---> Alloc for " << _length + 1 << " digits" << std::endl;
#endif
				// In case we reached this point, the array is full of '9's
				// Will need to realloc: 999 + 1 = 1000
				delete[] _values;

				_values = new digitType[_length + 1];
				memset(_values, 0, _length * sizeof(_values[0]));
				_values[_length] = 1;
				_length++;
			}
		}
	}
	else
	{
		// Deep zero state. Will return 1
#if defined _TRACE_
		std::cout << " ---> Alloc for zero" << std::endl;
#endif
		if (_values)
			delete[] _values;

		_values = new digitType[1];
		_values[0] = 1;
		_length = 1;
		_sign = NEG;
	}

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

// Pre-requirement: res._values is not allocated yet
template <class Type>
void longNum::add2positive(const longNum &n1, const Type n2, longNum &res) const
{
	if (n1._length > 20u)
	{
		res._length = n1._length;
	}
	else
	{
		size_t len(sizeof(Type));

		if (len < 8)
		{
			len = len < 4 ? 5 : 10;
		}
		else
		{
			len = 20;
		}

		res._length = (n1._length > len ? n1._length : len);
	}

#if defined _TRACE_
	std::cout << " ---> Alloc" << std::endl;
#endif

	Type* ptr = const_cast<Type*>(&n2);

	if (!n1._sign)
		*ptr *= -1;

	res._values = new digitType[res._length+1];

	digitType nRes = 0;
	res._length = 0;

	while(res._length < n1._length || n2)
	{
		if (res._length < n1._length)
		{
			nRes += n1._values[res._length];
		}

		if (n2)
		{
			nRes += n2 % BASE;
			*ptr /= BASE;
		}

		res._values[res._length++] = nRes % BASE;

		nRes = (nRes >= BASE) ? 1 : 0;
	}

	if (nRes)
	{
		res._values[res._length++] = 1;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Subtracts 2 positive numbers
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
		if (res._values)
		{
			delete[] res._values;
		}

		res._sign   = POS;
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

	size_t len_minus_one = pn1->_length - 1;

	// One digit is guaranteed to be there
	res._values[0] = pn1->_values[0];

	for (; i < pn2->_length; ++i)
	{
		if (i < len_minus_one)
			res._values[i+1] = pn1->_values[i+1];

		while (res._values[i] < pn2->_values[i])
		{
			res._values[i+1]--;
			res._values[i] += BASE;
		}

		res._values[i] = res._values[i] - pn2->_values[i];

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	for (; i < pn1->_length; ++i)
	{
		if (i < len_minus_one)
			res._values[i+1] = pn1->_values[i+1];

		while(res._values[i] < 0)
		{
			res._values[i+1]--;
			res._values[i] += BASE;
		}

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	res._sign = pn1->_sign;						// Sign is determined by the greater number
	res._length -= cnt;							// Adjust the length, so leading zeroes will be trimmed out

	return bRes;
}

// -----------------------------------------------------------------------------------------------

// Subtracts 2 positive numbers
// Returns:
//	false : n1 >= n2
//	true  : n1 <  n2
template <class Type>
bool longNum::subtr2positive(const longNum &n1, const Type n2, longNum &res) const
{
	// To subtract, we basically need to know what number is greater
	// Here, we don't know that
	// Let's try to do this anyway

	if (n1._length > 20u)
	{
		res._length = n1._length;
	}
	else
	{
		size_t len(sizeof(Type));

		if (len < 8)
		{
			len = len < 4 ? 5 : 10;
		}
		else
		{
			len = 20;
		}

		res._length = (n1._length > len ? n1._length : len);
	}

#if defined _TRACE_
	std::cout << " ---> Alloc" << std::endl;
#endif

	Type* ptr = const_cast<Type*>(&n2);

Type asdasdasd = n2;

	res._values = new digitType[res._length];

	digitType nRes = 0, nNext = 0;
	res._length = 0;
	size_t cnt = 0;

	while (res._length < n1._length && n2)
	{
		nRes = n1._values[res._length] + nNext;

		nRes -= n2 % BASE;
		*ptr /= BASE;

		if (nRes < 0)
		{
			nRes += 10;
			nNext = -1;
		}

		res._values[res._length] = nRes;
		cnt = res._values[res._length++] ? 0 : cnt + 1;
	}

	if (nNext < 0)
	{
		int a = 1;
	}

	if (!n2)
	{
		while (res._length < n1._length)
		{
			res._values[res._length] = n1._values[res._length] + nNext;

			if (res._values[res._length] < 0)
			{
				res._values[res._length] += 10;
				nNext = -1;
			}

			cnt = res._values[res._length] ? 0 : cnt + 1;
			res._length++;
		}

		if (res._length == cnt)
		{
			res._length = 1;
			res._sign = POS;
		}
		else
		{
			res._length -= cnt;
			res._sign = n1._sign;
		}
	}

	return true;
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
