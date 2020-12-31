#pragma once

// Fast copying via the use of hardware
// Mb check this out later
// https://codereview.stackexchange.com/questions/5520/copying-80-bytes-as-fast-as-possible

#include <string>

// -----------------------------------------------------------------------------------------------

#pragma warning (disable:4146)		// Search for 'pragma warning (disable:4146)' to find code parts that need this pragma

// -----------------------------------------------------------------------------------------------

#define _TRACE_

// -----------------------------------------------------------------------------------------------

constexpr bool POS = true;
constexpr bool NEG = false;

// -----------------------------------------------------------------------------------------------

// TODO
/*
	1. Introduce Capacity
	2. 
*/

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
		longNum(const int);


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
		void convertToSizeT_ifPossible();															// Tries to store the number as size_t

		void   add2positive	(const longNum &, const longNum &, longNum &)	const;					// Add 2 positive numbers
		bool subtr2positive	(const longNum &, const longNum &, longNum &)	const;					// Subtract 2 positive numbers

		template <class Type> void   add2positive(const longNum &, const Type, longNum &) const;
		template <class Type> bool subtr2positive(const longNum &, const Type, longNum &) const;

		int absValueIsLarger(const longNum &, const longNum &) const;								// Operator > for absolute values of the 2 numbers

		void add2positive_1(const longNum&, const longNum&, longNum&) const;						// Both numbers are long versions
		void add2positive_2(const longNum&, const longNum&, longNum&) const;						// 
		void add2positive_3(const longNum&, const size_t,   longNum&) const;						//

		bool subtr2positive_1(const longNum&, const longNum&, longNum&) const;						// Both numbers are long versions
		bool subtr2positive_2(const longNum&, const longNum&, longNum&) const;						//
		bool subtr2positive_3(const longNum&, const size_t,   longNum&) const;						//

		void opPlusEqual_1(longNum&, const longNum&) const;											// operator += helper 1
		void opPlusEqual_2(longNum&, const longNum&);												// operator += helper 2
		void opPlusEqual_3(longNum&, const longNum&);												// operator += helper 3
		void opPlusEqual_4(longNum&, const longNum&);												// operator += helper 4

	private:
		digitType*	_values;
		size_t		_length;
		bool		_sign;

};

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

longNum::longNum(const char *str) : _values(nullptr), _length(strlen(str)), _sign(POS)
{
	// Get sign and skip first symbol
	if (_length)
	{
		if (str[0] == '-' || str[0] == '+')
		{
			if(str[0] == '-')
				_sign = NEG;

			_length--;
			str++;
		}
	}

	if (_length >= 20u)
	{
		// Still might be less than size_t(-1)
		if (_length == 20u)
		{
			size_t tmp = static_cast<size_t>(1e19), Max(-1);
			const char* STR = str;

			while (tmp)
			{
				size_t digit1 = static_cast<size_t>(STR++[0]) - 48;		// our number
				size_t digit2 = (Max/tmp) % BASE;						// max size_t number

				if (digit1 > digit2)
				{
					_values = new digitType[_length];
					break;
				}

				if (digit1 < digit2)
				{
					break;
				}

				tmp /= BASE;
			}
		}
		else
		{
			_values = new digitType[_length];
		}

		// More than size_t(-1), store the number as array
		if (_values)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc (Constructor(const char*)) : " << str << std::endl;
#endif

			for (size_t i = 0; i < _length; i++)
				_values[_length - i - 1] = str[i] - 48;
		}
	}

	// Less than size_t(-1), store the number as size_t
	if (!_values)
	{
#if defined _TRACE_
		std::cout << " ---> Constructor(const char*) : store as size_t : " << str << std::endl;
#endif

		size_t tmp(0u);

		for (size_t i = 0; i < _length; i++)
			tmp = tmp * BASE + static_cast<size_t>(str[i]) - 48;

		_length = tmp;
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const longNum& other) : _length(other._length), _sign(other._sign), _values(nullptr)
{
	if (other._values)
	{
#if defined _TRACE_
		std::cout << " ---> Alloc (Copy Constructor)" << std::endl;
#endif
		_values = new digitType[_length];
		memcpy(_values, other._values, sizeof(digitType) * _length);
	}
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

longNum::longNum(const size_t num) : _length(num), _sign(POS), _values(nullptr)
{
	// When constructing longNum from size_t, the new value will always be stored as size_t
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const long num) : _length(num), _sign(POS), _values(nullptr)
{
	// When constructing longNum from long, the new value will always be stored as size_t
	if (num < 0)
	{
		_sign   = NEG;
		_length = -num;
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const int num) : _length(num), _sign(POS), _values(nullptr)
{
	// When constructing longNum from int, the new value will always be stored as size_t
	if (num < 0)
	{
		_sign   = NEG;
		_length = -num;
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
		_length = other._length;
		_sign   = other._sign;

		if (other._values)
		{
			if (this->_values)
			{
				// Will reuse the memory if this.length is the same
				if (_length != other._length)
				{
					delete[] _values;
					_values = nullptr;
				}
			}

			if (!this->_values)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc (operator =)" << std::endl;
#endif
				_values = new digitType[_length];
			}

			memcpy(_values, other._values, sizeof(digitType) * _length);
		}
		else
		{
			// New value will be stored as size_t
			if (this->_values)
			{
				delete[] _values;
				_values = nullptr;
			}
		}
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
		_sign   = std::move(other._sign);

		// Leave the other object in empty state
		other._length = 0;
		other._values = nullptr;
		other._sign   = POS;
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
	if (bool(_values) == bool(other._values))
	{
		if (_length == other._length && _sign == other._sign)
		{
			if (_values)
			{
				for (size_t i = 0; i < _length; ++i)
					if (_values[i] != other._values[i])
						return false;
			}

			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

// TODO: doesnt work with size_t somehow. check this later
template <class Type>
bool longNum::operator ==(const Type other) const
{
	if (_values || _sign != (other >= 0))
		return false;

	return _length == static_cast<size_t>(_sign ? other : -other);
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
	if (bool(_values) == bool(other._values))
	{
		if (_sign > other._sign)
			return true;

		if (_sign < other._sign)
			return false;

		if (_length > other._length)
			return _sign;

		if (_length < other._length)
			return !_sign;

		if (_values && _length)
		{
			for (size_t i = _length - 1; i != size_t(-1); --i)
			{
				if (_values[i] > other._values[i])
					return _sign;

				if (_values[i] < other._values[i])
					return !_sign;
			}
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >(const Type other) const
{
	if (_values || _sign != (other >= 0))
		return _sign;

	if(_sign)
		return _length > static_cast<size_t>(other);

	return _length < static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator >=(const longNum &other) const
{
	if (bool(_values) == bool(other._values))
	{
		if (_sign > other._sign)
			return true;

		if (_sign < other._sign)
			return false;

		if (_length > other._length)
			return _sign;

		if (_length < other._length)
			return !_sign;

		if (_values && _length)
		{
			for (size_t i = _length - 1; i != size_t(-1); --i)
			{
				if (_values[i] > other._values[i])
					return _sign;

				if (_values[i] < other._values[i])
					return !_sign;
			}
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >=(const Type other) const
{
	if (_values || _sign != (other >= 0))
		return _sign;

	if (_sign)
		return _length >= static_cast<size_t>(other);

	return _length <= static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <(const longNum &other) const
{
	if (bool(_values) == bool(other._values))
	{
		if (_sign < other._sign)
			return true;

		if (_sign > other._sign)
			return false;

		if (_length < other._length)
			return _sign;

		if (_length > other._length)
			return !_sign;

		if (_values && _length)
		{
			for (size_t i = _length - 1; i != size_t(-1); --i)
			{
				if (_values[i] < other._values[i])
					return _sign;

				if (_values[i] > other._values[i])
					return !_sign;
			}
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <(const Type other) const
{
	if (_values || _sign != (other >= 0))
		return !_sign;

	if (_sign)
		return _length < static_cast<size_t>(other);

	return _length > static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <=(const longNum &other) const
{
	if (bool(_values) == bool(other._values))
	{
		if (_sign < other._sign)
			return true;

		if (_sign > other._sign)
			return false;

		if (_length < other._length)
			return _sign;

		if (_length > other._length)
			return !_sign;

		if (_values && _length)
		{
			for (size_t i = _length - 1; i != size_t(-1); --i)
			{
				if (_values[i] < other._values[i])
					return _sign;

				if (_values[i] > other._values[i])
					return !_sign;
			}
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <=(const Type other) const
{
	if (_values || _sign != (other >= 0))
		return !_sign;

	if (_sign)
		return _length <= static_cast<size_t>(other);

	return _length >= static_cast<size_t>(-other);
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
	longNum res;

	if (_sign == (other >= 0))
	{
		// Templated version
		add2positive(*this, other, res);
		res._sign = _sign;
	}
	else
	{
		// Templated version
		subtr2positive(*this, other, res);
	}

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(const longNum &other)
{
	if (_values)
	{
		if (other._values)
		{
			opPlusEqual_1(*this, other);
		}
		else
		{
			opPlusEqual_2(*this, other);
		}
	}
	else
	{
		if (other._values)
		{
			opPlusEqual_3(*this, other);
		}
		else
		{
			opPlusEqual_4(*this, other);
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(longNum&& other) noexcept
{
	// We can steal the data from other number, if it is larger than this one
	// BUT
	// Moving something out of '&& other' makes sense only in these two cases:
	// []		+= []
	// size_t	+= []
	if (other._values)
	{
		if (!_values || (_values && _length < other._length))
		{
			size_t	   tmpLength = _length;
			digitType* tmpValues = _values;
			bool	   tmpSign   = _sign;

			_length = other._length;
			_sign   = other._sign;
			_values = other._values;

			other._length = tmpLength;
			other._sign   = tmpSign;
			other._values = tmpValues;
		}
	}

	*this += other;

	return *this;
}

// -----------------------------------------------------------------------------------------------

// []		+ Type
// size_t	+ Type
template <class Type>
longNum& longNum::operator +=(const Type other)
{
	Type* ptr = const_cast<Type*>(&other);

	if (_values)
	{
		// [] += Type
		if (_sign == (other >= 0))
		{
			// Perform adding in place
			digitType carryOver(0);

			if (*ptr < 0)
				*ptr = -*ptr;

			for (size_t i = 0; i < _length; ++i)
			{
				_values[i] += carryOver;

				if (other)
				{
					_values[i] += other % BASE;
					*ptr /= BASE;
				}

				carryOver = (_values[i] >= BASE) ? 1u : 0u;

				if (carryOver)
					_values[i] %= BASE;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc (operator += <Type>)" << std::endl;
#endif
				digitType* data = new digitType[_length + 1u];

				memcpy(data, _values, sizeof(digitType) * _length);

				data[_length++] = carryOver;

				delete[] _values;
				_values = data;
			}
		}
		else
		{
			// |*this| always > |other| -- No need for realloc. Subtract numbers in place

			size_t cnt(0);

			if (*ptr < 0)
				*ptr = -*ptr;

			for (size_t i = 0; i < _length; ++i)
			{
				if (other)
				{
					_values[i] -= other % BASE;
					*ptr /= BASE;
				}

				if (_values[i] < 0)
				{
					_values[i + 1]--;
					_values[i] += BASE;
				}

				cnt = _values[i] ? 0 : cnt + 1u;
			}

			// Adjust the length, so leading zeroes will be trimmed out
			_length -= cnt;

			// Value has reduced...
			convertToSizeT_ifPossible();
		}
	}
	else
	{
		// size_t += Type
		if (_sign == (other >= 0))
		{
			// Add the numbers and keep the sign
			if (*ptr < 0)
				*ptr = -*ptr;

			size_t length = _length + other;

			// Overflow: need to allocate memory
			if (length < _length || length < other)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc (operator += <Type>)" << std::endl;
#endif

				size_t MAX(-1), i(0);
				digitType carryOver(1);

				// 20, not 21. See the comment below.
				digitType* data = new digitType[20u];

				while (MAX)
				{
					data[i] = MAX % BASE + carryOver;

					if (length)
					{
						data[i] += length % BASE;
						length /= BASE;
					}

					MAX /= BASE;
					carryOver = (data[i] >= BASE) ? 1u : 0u;
					data[i] = data[i] % BASE;
					++i;
				}

				// Never going to hit this piece, as max numbers that allow us entering this branch of code
				// are size_t(-1) == 18446744073709551615
				// The sum of two such numbers is 36893488147419103230, which is still 20 digits long
				// So no carryOver at this point.
#if 0
				if (carryOver)
					data[i] = 1u;
#endif

				_length = i;
				_values = data;
			}
			else
			{
				_length = length;
			}
		}
		else
		{
			// Subtract lesser from greater and keep the greater's sign
			if (*ptr < 0)
				*ptr = -*ptr;

			if (_length < other)
			{
				_length = other - _length;
				_sign = !_sign;
			}
			else
			{
				_length -= other;
			}

			if (!_length)
				_sign = POS;
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 1 ([] += [])
void longNum::opPlusEqual_1(longNum &n1, const longNum &n2) const
{
#if 0

	// This code is slower (Tested)
	n1 = n1 + n2;

#else

	if (n1._sign == n2._sign)
	{
		// In case the sign is the same, we add the numbers and keep the sign.
		// Also, there's a chance we'll be able to add the 2 numbers without reallocation
		if (n1._length < n2._length)
		{
			// Go with full alloc cycle
			n1 = std::move(n1 + n2);
		}
		else
		{
			// Perform adding in place
			digitType carryOver = 0u, nRes = 0u;

			for (size_t i = 0; i < n1._length; ++i)
			{
				nRes = n1._values[i] + carryOver;

				if (i < n2._length)
					nRes += n2._values[i];

				n1._values[i] = nRes % BASE;

				carryOver = (nRes >= BASE) ? 1u : 0u;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc (opPlusEqual_1)" << std::endl;
#endif
				digitType* newVal = new digitType[n1._length + 1u];

				memcpy(newVal, n1._values, sizeof(digitType) * n1._length);

				newVal[n1._length++] = carryOver;

				delete[] n1._values;
				n1._values = newVal;
			}
		}
	}
	else
	{
		// In case the signs are opposite, we subtract smaller number from the greater and keep the sign of the greater one

		// First, we need to determine which number's absolute value is greater
		int isGreater = absValueIsLarger(n1, n2);

		// The numbers are equal, return 0
		if (!isGreater)
		{
			delete[] n1._values;
			n1._values = nullptr;

			n1._length = 0;
			n1._sign = POS;
		}
		else
		{
			const longNum* pn1(&n1);
			const longNum* pn2(&n2);

			digitType* res = pn1->_values;

			size_t i(0), cnt(0);

			if (isGreater == 2)
			{
#if defined _TRACE_
				std::cout << " ---> Alloc tmp buffer (opPlusEqual_1)" << std::endl;
#endif
				// n1 < n2: Will need another buffer for the result
				pn1 = &n2;
				pn2 = &n1;

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

				n1._length = pn1->_length - cnt;			// Adjust the length, so leading zeroes will be trimmed out

				delete[] n1._values;
				n1._values = res;
			}
			else
			{
				// n1 > n2: No need for realloc. Subtract numbers in place
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

				n1._length -= cnt;							// Adjust the length, so leading zeroes will be trimmed out
			}

			n1._sign = pn1->_sign;							// Sign is determined by the greater number (in absolute terms)

			// Value has reduced...
			n1.convertToSizeT_ifPossible();
		}
	}
#endif

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 2 ([] += size_t)
void longNum::opPlusEqual_2(longNum &n1, const longNum &n2)
{
	size_t tmp(n2._length);

	if (n1._sign == n2._sign)
	{
		// Perform adding in place
		digitType carryOver(0);

		for (size_t i = 0; i < n1._length; ++i)
		{
			n1._values[i] += carryOver;

			if (tmp)
			{
				n1._values[i] += tmp % BASE;
				tmp /= BASE;
			}

			carryOver = (n1._values[i] >= BASE) ? 1u : 0u;

			if (carryOver)
				n1._values[i] %= BASE;
		}

		// If carryOver > 0, we need to reallocate and normalize
		if (carryOver)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc (opPlusEqual_2)" << std::endl;
#endif
			digitType* newVal = new digitType[n1._length + 1u];

			memcpy(newVal, n1._values, sizeof(digitType) * n1._length);

			newVal[n1._length++] = carryOver;

			delete[] n1._values;
			n1._values = newVal;
		}
	}
	else
	{
		// |n1| always > |n2| -- No need for realloc. Subtract numbers in place

		size_t cnt(0);

		for (size_t i = 0; i < n1._length; ++i)
		{
			if (tmp)
			{
				n1._values[i] -= (tmp % BASE);
				tmp /= BASE;
			}

			if (n1._values[i] < 0)
			{
				n1._values[i + 1]--;
				n1._values[i] += BASE;
			}

			cnt = n1._values[i] ? 0 : cnt + 1u;
		}

		// Adjust the length, so leading zeroes will be trimmed out
		n1._length -= cnt;

		// Value has reduced...
		n1.convertToSizeT_ifPossible();
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 3 (size_t += [])
void longNum::opPlusEqual_3(longNum &n1, const longNum &n2)
{
#if defined _TRACE_
	std::cout << " ---> Alloc (opPlusEqual_3)" << std::endl;
#endif

	size_t tmp(n1._length);
	digitType carryOver(0);

	if (n1._sign == n2._sign)
	{
		n1._values = new digitType[n2._length + 1u];

		for (n1._length = 0; n1._length < n2._length; ++n1._length)
		{
			n1._values[n1._length] = n2._values[n1._length] + carryOver;

			if (tmp)
			{
				n1._values[n1._length] += tmp % BASE;
				tmp /= BASE;
			}

			carryOver = (n1._values[n1._length] >= BASE) ? 1u : 0u;
			n1._values[n1._length] %= BASE;
		}

		if (carryOver)
		{
			n1._values[n1._length++] = 1u;
		}
	}
	else
	{
		// |n1| always < |n2|

		size_t cnt(0);
		n1._values = new digitType[n2._length];

		for (n1._length = 0; n1._length < n2._length; ++n1._length)
		{
			n1._values[n1._length] = n2._values[n1._length] - tmp % BASE - carryOver;
			tmp /= BASE;

			carryOver = (n1._values[n1._length] < 0) ? 1 : 0;

			if (carryOver)
			{
				n1._values[n1._length] += BASE;
			}

			cnt = n1._values[n1._length] ? 0 : cnt + 1u;
		}

		n1._length -= cnt;

		// Value has reduced...
		n1.convertToSizeT_ifPossible();
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 4 (size_t += size_t)
void longNum::opPlusEqual_4(longNum &n1, const longNum &n2)
{
	if (n1._sign == n2._sign)
	{
		// Add the numbers and keep the sign

		size_t length = n1._length + n2._length;

		// Overflow: need to allocate memory
		if (length < n1._length || length < n2._length)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc (opPlusEqual_4)" << std::endl;
#endif

			size_t tmp(-1), i(0);
			digitType carryOver(1);

			digitType* data = new digitType[20u];

			while (tmp)
			{
				data[i] = tmp % BASE + carryOver;

				if (length)
				{
					data[i] += length % BASE;
					length /= BASE;
				}

				tmp /= BASE;
				carryOver = (data[i] >= BASE) ? 1u : 0u;
				data[i] = data[i] % BASE;
				++i;
			}

#if 0
			if (carryOver)
				data[i] = 1u;
#endif

			n1._length = i;
			n1._values = data;
		}
		else
		{
			n1._length = length;
		}
	}
	else
	{
		// Subtract lesser from greater and keep the greater's sign

		const longNum* pn1 = &n1;
		const longNum* pn2 = &n2;

		if (n1._length < n2._length)
		{
			pn1 = &n2;
			pn2 = &n1;
		}

		n1._length = pn1->_length - pn2->_length;
		n1._sign = pn1->_sign;

		if (!n1._length)
			n1._sign = POS;
	}

	return;
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

// TODO :: rewrite, now its wrong
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
		// Return value stored as array
		str.reserve(_sign == NEG ? _length+1 : _length);

		if(_sign == NEG)
			str += '-';

		for (size_t i = 0; i < _length; i++)
			str += static_cast<char>(_values[_length - i - 1] + 48);
	}
	else
	{
		// Return value stored in _length
		if (_length)
		{
			size_t len(0), tmp(_length);

			while (tmp)
			{
				len++;
				tmp /= 10u;
			}

			str.resize(_sign == NEG ? ++len : len);

			if (_sign == NEG)
				str[0] = '-';

			tmp = _length;

			while (tmp)
			{
				len--;
				str[len] = static_cast<char>(tmp % 10u) + 48;
				tmp /= 10u;
			}
		}
		else
		{
			str = "0";
		}
	}

	return str;
}

// -----------------------------------------------------------------------------------------------

// Both numbers are long versions
void longNum::add2positive(const longNum &n1, const longNum &n2, longNum &res) const
{
	if (n1._values)
	{
		if (n2._values)
		{
			// Add 2 positive numbers: each one is a long version
			add2positive_1(n1, n2, res);
		}
		else
		{
			// Add 2 positive numbers: The first one is an array, the second one is a size_t
			add2positive_3(n1, n2._length, res);
		}
	}
	else
	{
		if (n2._values)
		{
			// Add 2 positive numbers: The first one is an array, the second one is a size_t
			add2positive_3(n2, n1._length, res);
		}
		else
		{
			// Add 2 positive numbers: Each of the two holds its number as a size_t
			add2positive_2(n1, n2, res);
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive longNums: Each of the two has allocated array
void longNum::add2positive_1(const longNum &n1, const longNum &n2, longNum &res) const
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
		std::cout << " ---> Alloc (add2positive_1)" << std::endl;
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

// Add 2 positive numbers: Each of the two holds its number as a size_t
void longNum::add2positive_2(const longNum& n1, const longNum& n2, longNum& res) const
{
	res._length = n1._length + n2._length;

	// Overflow: need to allocate memory
	if (res._length < n1._length || res._length < n2._length)
	{
#if defined _TRACE_
		std::cout << " ---> Alloc (add2positive_2)" << std::endl;
#endif

		size_t tmp(-1), i(0);
		digitType carryOver(1);

		res._values = new digitType[20u];

		while (tmp)
		{
			res._values[i] = tmp % BASE + carryOver;

			if (res._length)
			{
				res._values[i] += res._length % BASE;
				res._length /= BASE;
			}

			tmp /= BASE;
			carryOver = (res._values[i] >= BASE) ? 1u : 0u;
			res._values[i] = res._values[i] % BASE;
			++i;
		}

#if 0
		if (carryOver)
			res._values[i] = 1u;
#endif

		res._length = i;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive numbers: The first one is an array, the second one is a size_t
// TODO: replace tmp with consyt_castr ptr
void longNum::add2positive_3(const longNum& n1, const size_t n2, longNum& res) const
{
#if defined _TRACE_
	std::cout << " ---> Alloc (add2positive_3)" << std::endl;
#endif

	size_t tmp(n2);
	digitType carryOver(0);

	res._length = n1._length;
	res._values = new digitType[n1._length + 1u];

	for (size_t i = 0; i < res._length; ++i)
	{
		res._values[i] = n1._values[i] + carryOver;

		if (tmp)
		{
			res._values[i] += tmp % BASE;
			tmp /= BASE;
		}

		carryOver = (res._values[i] >= BASE) ? 1u : 0u;
		res._values[i] = res._values[i] % BASE;
	}

	if (carryOver)
	{
		res._values[res._length++] = 1u;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Pre-requirement: res._values is not allocated yet
// TODO: remove pragma when it's not needed
template <class Type>
void longNum::add2positive(const longNum &n1, const Type n2, longNum &res) const
{
	Type* tmp = const_cast<Type*>(&n2);

	// #pragma warning (disable:4146)
	// This piece of code won't affect any unsigned types.
	// But when we pass unsigned type as template Type, the compiler ignores the fact that the value is compared with zero,
	// but still says we can't assign *tmp to -*tmp
	// So we disable this warning for good
	if (*tmp < 0)
		*tmp = -*tmp;

	if (n1._values)
	{
		// Add 2 positive numbers: The first one is an array, the second one is a Type
		digitType carryOver(0);

#if defined _TRACE_
		std::cout << " ---> Alloc (add2positive<Type>) - 1" << std::endl;
#endif

		res._length = n1._length;
		res._values = new digitType[n1._length + 1u];

		for (size_t i = 0; i < res._length; ++i)
		{
			res._values[i] = n1._values[i] + carryOver;

			if (n2)
			{
				res._values[i] += n2 % BASE;
				*tmp /= BASE;
			}

			carryOver = (res._values[i] >= BASE) ? 1u : 0u;
			res._values[i] = res._values[i] % BASE;
		}

		if (carryOver)
		{
			res._values[res._length++] = 1u;
		}
	}
	else
	{
		// Add 2 positive numbers: The first one holds its value as a size_t, the second one is a Type

		res._length = n1._length + n2;

		// Overflow: need to allocate memory
		if (res._length < n1._length || res._length < n2)
		{
#if defined _TRACE_
			std::cout << " ---> Alloc (add2positive<Type>) - 2" << std::endl;
#endif

			size_t MAX(-1), i(0);
			digitType carryOver(1);

			res._values = new digitType[20u];

			while (MAX)
			{
				res._values[i] = MAX % BASE + carryOver;

				if (res._length)
				{
					res._values[i] += res._length % BASE;
					res._length /= BASE;
				}

				MAX /= BASE;
				carryOver = (res._values[i] >= BASE) ? 1u : 0u;
				res._values[i] = res._values[i] % BASE;
				++i;
			}

#if 0
			if (carryOver)
				res._values[i] = 1u;
#endif

			res._length = i;
		}
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
	if (n1._values)
	{
		if (n2._values)
		{
			// Subtract 2 positive numbers: each one is a long version
			return subtr2positive_1(n1, n2, res);
		}
		else
		{
			// Subtract 2 positive numbers: The first one is an array, the second one is a size_t
			return subtr2positive_3(n1, n2._length, res);
		}
	}
	else
	{
		if (n2._values)
		{
			// Subtract 2 positive numbers: The first one is an array, the second one is a size_t
			return !subtr2positive_3(n2, n1._length, res);
		}
		else
		{
			// Subtract 2 positive numbers: Each of the two holds its number as a size_t
			return subtr2positive_2(n1, n2, res);
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

// [] - []
bool longNum::subtr2positive_1(const longNum &n1, const longNum &n2, longNum &res) const
{
	bool bRes = true;

	const longNum* pn1(&n2);
	const longNum* pn2(&n1);

	// First, we need to determine which number's absolute value is greater
	int isGreater = absValueIsLarger(n1, n2);

	// The numbers are equal: just return 0
	if (!isGreater)
	{
		if (res._values)
			delete[] res._values;

#if defined _TRACE_
		std::cout << " ---> longNum converted to size_t : 0" << std::endl;
#endif

		res._values = nullptr;
		res._length = 0;
		res._sign = POS;

		return false;
	}

	if (isGreater == 1)
	{
		pn1 = &n1;
		pn2 = &n2;
		bRes = false;
	}

#if defined _TRACE_
	std::cout << " ---> Alloc (subtr2positive_1)" << std::endl;
#endif

	res._length = pn1->_length;
	res._values = new digitType[pn1->_length];

	size_t i = 0, cnt = 0, len_minus_one = pn1->_length - 1;

	// One digit is guaranteed to be there
	res._values[0] = pn1->_values[0];

	for (; i < pn2->_length; ++i)
	{
		if (i < len_minus_one)
			res._values[i + 1] = pn1->_values[i + 1];

		while (res._values[i] < pn2->_values[i])
		{
			res._values[i + 1]--;
			res._values[i] += BASE;
		}

		res._values[i] = res._values[i] - pn2->_values[i];

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	for (; i < pn1->_length; ++i)
	{
		if (i < len_minus_one)
			res._values[i + 1] = pn1->_values[i + 1];

		while (res._values[i] < 0)
		{
			res._values[i + 1]--;
			res._values[i] += BASE;
		}

		cnt = res._values[i] ? 0 : cnt + 1;
	}

	res._sign = pn1->_sign;						// Sign is determined by the greater number
	res._length -= cnt;							// Adjust the length, so leading zeroes will be trimmed out

	res.convertToSizeT_ifPossible();

	return bRes;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers: each is a size_t
bool longNum::subtr2positive_2(const longNum &n1, const longNum &n2, longNum &res) const
{
	bool bRes(false);

	if (n1._length < n2._length)
	{
		res._length = n2._length - n1._length;
		res._sign   = n2._sign;
		bRes = true;
	}
	else
	{
		res._length = n1._length - n2._length;
		res._sign   = res._length ? n1._sign : POS;
	}

	return bRes;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers: the first one is an array, the second one is a size_t
// If the resulting number can be stored as a size_t, we'll have to store it like that
// TODO: check if tmp(n2) is faster or slower than const_cast
bool longNum::subtr2positive_3(const longNum &n1, const size_t n2, longNum &res) const
{
#if defined _TRACE_
	std::cout << " ---> Alloc (subtr2positive_3)" << std::endl;
#endif

	res._length = n1._length;
	res._values = new digitType[n1._length];

	size_t tmp(n2), cnt(0);
	digitType carryOver(0);

	for (size_t i = 0; i < res._length; ++i)
	{
		res._values[i] = n1._values[i] - carryOver;

		if (tmp)
		{
			res._values[i] -= tmp % BASE;
			tmp /= BASE;
		}

		carryOver = (res._values[i] >= 0) ? 0u : 1u;

		if(carryOver)
			res._values[i] += BASE;

		cnt = res._values[i] ? 0 : cnt + 1u;
	}

	if (cnt)
		res._length -= cnt;

	res._sign = n1._sign;					// Sign is determined by the greater number

	// Need to check if we can store the number as a size_t
	res.convertToSizeT_ifPossible();

	return false;
}

// -----------------------------------------------------------------------------------------------

// Subtracts 2 positive numbers
// Returns:
//	false : n1 >= n2
//	true  : n1 <  n2
template <class Type>
bool longNum::subtr2positive(const longNum &n1, const Type n2, longNum &res) const
{
	Type* tmp = const_cast<Type*>(&n2);

	if (n1._values)
	{
		// #pragma warning (disable:4146)
		if (*tmp < 0)
			*tmp = -*tmp;

		// Subtract 2 positive numbers: The first one is an array, the second one is a Type

#if defined _TRACE_
		std::cout << " ---> Alloc (subtr2positive<Type>)" << std::endl;
#endif

		res._length = n1._length;
		res._values = new digitType[n1._length];

		size_t cnt(0);
		digitType carryOver(0);

		for (size_t i = 0; i < res._length; ++i)
		{
			res._values[i] = n1._values[i] - carryOver;

			if (n2)
			{
				res._values[i] -= n2 % BASE;
				*tmp /= BASE;
			}

			carryOver = (res._values[i] >= 0) ? 0u : 1u;

			if (carryOver)
				res._values[i] += BASE;

			cnt = res._values[i] ? 0 : cnt + 1u;
		}

		if (cnt)
			res._length -= cnt;

		res._sign = n1._sign;					// Sign is determined by the greater number

		// Need to check if we can store the number as a size_t
		res.convertToSizeT_ifPossible();

		return false;
	}
	else
	{
		bool bRes(false), n2_sign(true);

		// #pragma warning (disable:4146)
		if (*tmp < 0)
		{
			n2_sign = false;
			*tmp = -*tmp;
		}

		// Subtract 2 positive numbers: The first one holds its value as a size_t, the second one is a Type

		if (n1._length < n2)
		{
			res._length = n2 - n1._length;
			res._sign = n2_sign;
			bRes = true;
		}
		else
		{
			res._length = n1._length - n2;
			res._sign = res._length ? n1._sign : POS;
		}

		return bRes;
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

// Tries to store the number as size_t
// Pre-condition: _values is allocated, _length is holding length, not value
void longNum::convertToSizeT_ifPossible()
{
	// max size_t = 18446744073709551615
	static digitType maxSizeT[] = { 5, 1, 6, 1, 5, 5, 9, 0, 7, 3, 7, 0, 4, 4, 7, 6, 4, 4, 8, 1 };

	if (_length < 21u)
	{
		size_t i(_length);

		if (_length == 20)
		{
			// Check if the number is larger than max size_t
			while (i--)
			{
				if (_values[i] < maxSizeT[i])
					break;

				if (_values[i] > maxSizeT[i])
					return;
			}

			i = _length;
		}

		_length = 0u;

		while (i--)
			_length = _length * BASE + _values[i];

		delete[] _values;
		_values = nullptr;

#if defined _TRACE_
		std::cout << " ---> longNum converted to size_t : " << _length << std::endl;
#endif
	}

	return;
}

// -----------------------------------------------------------------------------------------------
