#include "stdafx.h"
#include "__longNum.h"

#include <map>
#include <iostream>

// -----------------------------------------------------------------------------------------------

longNum::digitType longNum::maxSizeT[] = { 5, 1, 6, 1, 5, 5, 9, 0, 7, 3, 7, 0, 4, 4, 7, 6, 4, 4, 8, 1 };

// -----------------------------------------------------------------------------------------------

// TODO: check all cases of using values[i] and replace with *digit

// Main Class Contents
#if 1

// TODO: try to make it a bit faster
longNum::longNum(const char* str) : _values(nullptr), _length(strlen(str)), _sign(POS)
{
	auto doTheAlloc = [&](size_t len) -> void
	{
		digitType* data = new digitType[len], * digit(data + len);

		TRACE_MSG_IF2("Alloc Constructor(const char*) : ", str);

		for (size_t i(0u); i < len; ++i)
		{
			--digit;
			*digit = digitType(str[i] - 48);
		}

		_values = data;
	};

	// ----------------------------------------------------------------------------

	TRACE_CODE_FLOW("longNum::longNum(const char *)");

	// Get sign and skip first symbol
	if (_length)
	{
		if (str[0] == '-' || str[0] == '+')
		{
			if (str[0] == '-')
				_sign = NEG;

			--_length;
			++str;
		}

		if (_length >= longNum_MAX_SIZE_T_LENGTH)
		{
			// Still might be less than size_t(-1)
			static const char ch = char(maxSizeT[longNum_MAX_SIZE_T_LENGTH - 1] + 48);

			if (_length == longNum_MAX_SIZE_T_LENGTH && str[0] <= ch)
			{
				const char* STR = str + 1;
				digitType digit1, *digit2 = &maxSizeT[longNum_MAX_SIZE_T_LENGTH-1];

				for (size_t i = 1; i < longNum_MAX_SIZE_T_LENGTH; ++i)
				{
					digit1 = *STR++ - 48;	// our digit
					digit2--;				// max size_t digit

					if (digit1 != *digit2)
					{
						if (digit1 > *digit2)
							doTheAlloc(longNum_MAX_SIZE_T_LENGTH);

						break;
					}
				}
			}
			else
			{
				doTheAlloc(_length);
			}
		}

		// Less than size_t(-1), store the number as size_t
		if (!_values)
		{
			TRACE_MSG_IF2("Constructor(const char*) -- store as size_t: ", str);

			size_t num(0u);

			for (size_t i(0u); i < _length; i++)
				num = num * BASE + static_cast<size_t>(str[i]) - 48u;

			_length = num;
		}
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const longNum& other) : _length(other._length), _sign(other._sign), _values(nullptr)
{
	TRACE_CODE_FLOW("longNum::longNum(const longNum &)");

	if (other._values)
	{
		TRACE_MSG_IF1("Alloc (Copy Constructor)");

		_values = new digitType[_length];
		memcpy(_values, other._values, sizeof(digitType) * _length);
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(longNum&& other) noexcept
{
	TRACE_CODE_FLOW("longNum::longNum(longNum &&)");

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
	TRACE_CODE_FLOW("longNum::longNum(const size_t)");
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const long num) : _sign(num >= 0), _length(_sign ? num : -num), _values(nullptr)
{
	TRACE_CODE_FLOW("longNum::longNum(const long)");

#if defined _IS_LESSER_

	// When _IS_LESSER_ is defined, all the numbers > longNum_MAX_VALUE will be created as array-based
	long NUM(_sign ? num : -num);

	if (NUM <= longNum_MAX_VALUE)
	{
		_length = NUM;
	}
	else
	{
		TRACE_MSG_IF2("Alloc (Constructor(const long)): ", num);

		long len = 0, tmp(NUM);

		while (tmp)
		{
			len++;
			tmp /= BASE;
		}

		_length = 0u;
		tmp = NUM;
		_values = new digitType[len];

		while (tmp)
		{
			_values[_length++] = tmp % 10;
			tmp /= BASE;
		}
	}

#endif
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const long long num) : _sign(num >= 0), _length(_sign ? num : -num), _values(nullptr)
{
	// When constructing longNum from int, the new value will always be stored as size_t
	TRACE_CODE_FLOW("longNum::longNum(const long long)");
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(const int num) : _sign(num >= 0), _length(_sign ? num : -num), _values(nullptr)
{
	// When constructing longNum from int, the new value will always be stored as size_t
	TRACE_CODE_FLOW("longNum::longNum(const int)");
}

// -----------------------------------------------------------------------------------------------

longNum::~longNum()
{
	TRACE_MSG_IF1("Calling Destructor");

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
	TRACE_CODE_FLOW("longNum::operator =(const longNum &)");

	if (this != &other)
	{
		if (other._values)
		{
			if (_values)
			{
				// Will reuse the memory if this.length is the same
				if (_length != other._length)
				{
					delete[] _values;
					_values = nullptr;
				}
			}

			if (!_values)
			{
				TRACE_MSG_IF1("Alloc (operator =)");

				_values = new digitType[other._length];
			}

			memcpy(_values, other._values, sizeof(digitType) * other._length);
		}
		else
		{
			// New value will be stored as size_t
			if (_values)
			{
				delete[] _values;
				_values = nullptr;
			}
		}

		_length = other._length;
		_sign = other._sign;
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(longNum&& other) noexcept
{
	TRACE_CODE_FLOW("longNum::operator =(longNum &&)");

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
	TRACE_CODE_FLOW("longNum::operator =(const Type)");

#if defined _IS_LESSER_

	*this = longNum(other);

#else

	if (_values)
	{
		delete[] _values;
		_values = nullptr;
	}

	_sign = (other >= 0);
	_length = _sign ? other : -other;

#endif

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <>
longNum& longNum::operator =(const char *str)
{
	TRACE_CODE_FLOW("longNum::operator =(const char *)");

	// TODO: this will work, be it can be done without realloc if str length is the same
	if (_values)
	{
		delete[] _values;
		_values = nullptr;
	}

	return *this = longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator ==(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator ==(const longNum &)");

	if (bool(_values) == bool(other._values))
	{
		if (_length == other._length && _sign == other._sign)
		{
			if (_values)
			{
#if 1
				// Somehow this works a bit faster
				return (absValueIsLarger(*this, other) == 0);
#else
				long long* n1digitLong = reinterpret_cast<long long*>(		_values + _length);
				long long* n2digitLong = reinterpret_cast<long long*>(other._values + _length);

				for (size_t i = 0; i < _length / ratio_ll_to_digitType; ++i)
				{
					n1digitLong--;
					n2digitLong--;

					if (*n1digitLong != *n2digitLong)
						return false;
				}

				if (_length % ratio_ll_to_digitType)
				{
					digitType* n1digit(		 _values + _length % ratio_ll_to_digitType);
					digitType* n2digit(other._values + _length % ratio_ll_to_digitType);

					for (int i = 0; i < _length % ratio_ll_to_digitType; ++i)
					{
						n1digit--;
						n2digit--;

						if (*n1digit != *n2digit)
							return false;
					}
				}
#endif
			}

			return true;
		}
	}

	return false;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator ==(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator ==(const Type)");

#if defined _IS_LESSER_

	if (_sign != (other >= 0))
		return false;

	return as_size_t() == static_cast<size_t>(_sign ? other : -other);

#endif

	if (_values || _sign != (other >= 0))
		return false;

	return _length == static_cast<size_t>(_sign ? other : -other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator ==(const char *str) const
{
	TRACE_CODE_FLOW("longNum::operator ==(const char *)");

	// TODO: make this work without constructor
	return *this == longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator !=(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator !=(const longNum &)");

	return !(*this == other);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator !=(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator !=(const Type)");

	return !(*this == other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator !=(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator !=(const char *)");

	// TODO: make this work without constructor
	return *this != longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator >(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator >(const longNum &)");

	if (_sign != other._sign)
		return _sign > other._sign;

	if (bool(_values) == bool(other._values))
	{
		if (_length != other._length)
			return (_length > other._length) ? _sign : !_sign;

		if (_values && _length)
		{
			int isGreater = absValueIsLarger(*this, other);

			if (isGreater)
				return isGreater < 2 ? _sign : !_sign;
		}

		return false;
	}

	return _values ? _sign : !_sign;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator >(const Type)");

#if defined _IS_LESSER_

	if (_sign != (other >= 0))
		return _sign;

	if (_sign)
		return as_size_t() > static_cast<size_t>(other);

	return as_size_t() < static_cast<size_t>(-other);

#endif

	if (_values || _sign != (other >= 0))
		return _sign;

	if (_sign)
		return _length > static_cast<size_t>(other);

	return _length < static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator >(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator >(const char *)");

	// TODO: optimize later
	return *this > longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator >=(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator >=(const longNum &)");

	if (_sign != other._sign)
		return _sign > other._sign;

	if (bool(_values) == bool(other._values))
	{
		if (_length != other._length)
			return (_length > other._length) ? _sign : !_sign;

		if (_values && _length)
		{
			int isGreater = absValueIsLarger(*this, other);

			if (isGreater)
				return isGreater < 2 ? _sign : !_sign;
		}

		return true;
	}

	return _values ? _sign : !_sign;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator >=(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator >=(const Type)");

#if defined _IS_LESSER_

	if (_sign != (other >= 0))
		return _sign;

	if (_sign)
		return as_size_t() >= static_cast<size_t>(other);

	return as_size_t() <= static_cast<size_t>(-other);

#endif

	if (_values || _sign != (other >= 0))
		return _sign;

	if (_sign)
		return _length >= static_cast<size_t>(other);

	return _length <= static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator >=(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator >=(const char *)");

	// TODO: optimize later
	return *this >= longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator <(const longNum &)");

	if (_sign != other._sign)
		return _sign < other._sign;

	if (bool(_values) == bool(other._values))
	{
		if (_length != other._length)
			return (_length < other._length) ? _sign : !_sign;

		if (_values && _length)
		{
			int isGreater = absValueIsLarger(*this, other);

			if (isGreater)
				return isGreater > 1 ? _sign : !_sign;
		}

		return false;
	}

	return _values ? !_sign : _sign;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator <(const Type)");

#if defined _IS_LESSER_

	if (_sign != (other >= 0))
		return !_sign;

	if (_sign)
		return as_size_t() < static_cast<size_t>(other);

	return as_size_t() > static_cast<size_t>(-other);

#endif

	if (_values || _sign != (other >= 0))
		return !_sign;

	if (_sign)
		return _length < static_cast<size_t>(other);

	return _length > static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator <(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator <(const char *)");

	// TODO: optimize later
	return *this < longNum(str);
}

// -----------------------------------------------------------------------------------------------

bool longNum::operator <=(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator <=(const longNum &)");

	if (_sign != other._sign)
		return _sign < other._sign;

	if (bool(_values) == bool(other._values))
	{
		if (_length != other._length)
			return (_length < other._length) ? _sign : !_sign;

		if (_values && _length)
		{
			int isGreater = absValueIsLarger(*this, other);

			if (isGreater)
				return isGreater > 1 ? _sign : !_sign;
		}

		return true;
	}

	return _values ? !_sign : _sign;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool longNum::operator <=(const Type other) const
{
	TRACE_CODE_FLOW("longNum::operator <=(const Type)");

#if defined _IS_LESSER_

	if (_sign != (other >= 0))
		return !_sign;

	if (_sign)
		return as_size_t() <= static_cast<size_t>(other);

	return as_size_t() >= static_cast<size_t>(-other);

#endif

	if (_values || _sign != (other >= 0))
		return !_sign;

	if (_sign)
		return _length <= static_cast<size_t>(other);

	return _length >= static_cast<size_t>(-other);
}

// -----------------------------------------------------------------------------------------------

template <>
bool longNum::operator <=(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator <=(const char *)");

	// TODO: optimize later
	return *this <= longNum(str);
}

// -----------------------------------------------------------------------------------------------

longNum longNum::operator +(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator +(const longNum &)");

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
	TRACE_CODE_FLOW("longNum::operator +(const Type)");

#if 0

	return std::move(*this + longNum(other));

#else

	// This version is only SLIGHTLY faster
	longNum res;
	res._sign = other >= 0;

	if (_sign == res._sign)
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

#endif
}

// -----------------------------------------------------------------------------------------------

// TODO: make this work
template <>
longNum longNum::operator +(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator +(const char *)");

	return longNum(0);
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(const longNum& other)
{
	TRACE_CODE_FLOW("longNum::operator +=(const longNum &)");

	if (_values)
	{
		if (other._values)
		{
			opPlusEqual_1(*this, other);
		}
		else
		{
			opPlusEqual_2(*this, other, other._length);
		}
	}
	else
	{
		if (other._values)
		{
			opPlusEqual_3(*this, other, _length);
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
	TRACE_CODE_FLOW("longNum::operator +=(longNum &&)");

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
			bool	   tmpSign = _sign;

			_length = other._length;
			_sign = other._sign;
			_values = other._values;

			other._length = tmpLength;
			other._sign = tmpSign;
			other._values = tmpValues;
		}
	}

	*this += other;

	return *this;
}

// -----------------------------------------------------------------------------------------------

// []	  += Type
// size_t += Type
// optimized
template <class Type>
longNum& longNum::operator +=(const Type other)
{
	TRACE_CODE_FLOW("longNum::operator +=(const Type)");

	Type* ptr = const_cast<Type*>(&other);

	if (_values)
	{
		if (_sign == (other >= 0))
		{
			// [] += Type
			MAKE_POSITIVE(*ptr);

			// Perform adding in place
			size_t i(0u);
			digitType carryOver(0);
			digitType* digit(_values);

			while (other)
			{
				*digit += other % BASE + carryOver;

				*ptr /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
				++i;
			}

			for (; carryOver && i < _length; ++i)
			{
				*digit += carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
				TRACE_MSG_IF1("Alloc (operator += <Type>)");

				digitType* data = new digitType[_length + 1u];

				memcpy(data, _values, sizeof(digitType) * _length);
				data[_length++] = carryOver;

				delete[] _values;
				_values = data;
			}
		}
		else
		{
			// [] -= Type
			MAKE_POSITIVE(*ptr);

			// Subtract numbers in place
			size_t cnt(0u), i(0u);
			digitType carryOver(0);
			digitType* digit(_values);

			while (other)
			{
				*digit -= (other % BASE + carryOver);

				*ptr /= BASE;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
				++i;
			}

			for (; (cnt || carryOver) && i < _length; ++i)
			{
				*digit -= carryOver;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
			}

			_length -= cnt;

			// Need to check if we can store the number as a size_t
			if (_length <= longNum_MAX_SIZE_T_LENGTH)
				convertToSizeT_ifPossible();
		}
	}
	else
	{
		if (_sign == (other >= 0))
		{
			// size_t += Type
			MAKE_POSITIVE(*ptr);

			// Add the numbers and keep the sign
			size_t length = _length + other;

#if defined _IS_LESSER_

			// Overflow: need to allocate memory
			if (length > longNum_MAX_VALUE)
			{
				TRACE_MSG_IF1("Alloc (operator += <Type>)");

				size_t MAX(longNum_MAX_VALUE), i(0u);
				digitType carryOver(1);

				digitType* data = new digitType[longNum_MAX_SIZE_T_LENGTH], * digit(data);

				// Imitate size_t overflow
				length -= (longNum_MAX_VALUE + 1);

				while (length)
				{
					*digit = MAX % BASE + length % BASE + carryOver;

					length /= BASE;
					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				while (MAX)
				{
					*digit = MAX % BASE + carryOver;

					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				_length = i;
				_values = data;
			}
			else
			{
				_length = length;
			}

#else

			// Overflow: need to allocate memory
			if (length < _length || length < other)
			{
				TRACE_MSG_IF1("Alloc (operator += <Type>)");

				size_t MAX(longNum_MAX_VALUE), i(0u);
				digitType carryOver(1);

				digitType* data = new digitType[longNum_MAX_SIZE_T_LENGTH], *digit(data);

				while (length)
				{
					*digit = MAX % BASE + length % BASE + carryOver;

					length /= BASE;
					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				while (MAX)
				{
					*digit = MAX % BASE + carryOver;

					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				_length = i;
				_values = data;
			}
			else
			{
				_length = length;
			}

#endif
		}
		else
		{
			// size_t -= Type
			MAKE_POSITIVE(*ptr);

			// Subtract lesser from greater and keep the greater's sign
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

template <>
longNum& longNum::operator +=(const char* str)
{
	TRACE_CODE_FLOW("longNum::operator +=(const char *)");

	// TODO: rewrite with in place operation and test if this is faster
	return *this += longNum(str);
}

// -----------------------------------------------------------------------------------------------

// operator += helper 1 ([] += [])
void longNum::opPlusEqual_1(longNum& n1, const longNum& n2) const
{
	TRACE_CODE_FLOW("longNum::opPlusEqual_1");

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
			size_t i(0u);
			digitType carryOver(0), *digit(n1._values);

			for (; i < n2._length; ++i)
			{
				*digit += n2._values[i] + carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			for (; carryOver && i < n1._length; ++i)
			{
				*digit += carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
				TRACE_MSG_IF1("Alloc (opPlusEqual_1)");

				digitType* data = new digitType[n1._length + 1u];

				memcpy(data, n1._values, sizeof(digitType) * n1._length);
				data[n1._length++] = carryOver;

				delete[] n1._values;
				n1._values = data;
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

			size_t i(0u), cnt(0u);
			digitType carryOver(0);

			if (isGreater > 1)
			{
				// n1 < n2
				pn1 = &n2;
				pn2 = &n1;
			}

			if (isGreater == 3)
			{
				// Will need another buffer to store the result
				TRACE_MSG_IF1("Alloc tmp buffer (opPlusEqual_1)");

				digitType* res = new digitType[pn1->_length];
				digitType* digit(res);;

				for (; i < pn2->_length; ++i)
				{
					*digit = pn1->_values[i] - (pn2->_values[i] + carryOver);

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				for (; i < pn1->_length; ++i)
				{
					*digit = pn1->_values[i] - carryOver;

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				delete[] n1._values;
				n1._values = res;
			}
			else
			{
				// No need for realloc. Subtract numbers in place
				digitType* digit(n1._values);

				for (; i < pn2->_length; ++i)
				{
					*digit = pn1->_values[i] - (pn2->_values[i] + carryOver);

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				// When cnt == 0 and carryOver == 0, we break out of the loop
				for (; (cnt || carryOver) && i < pn1->_length; ++i)
				{
					*digit = pn1->_values[i] - carryOver;

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}
			}

			n1._sign = pn1->_sign;							// Sign is determined by the greater number (in absolute terms)
			n1._length = pn1->_length - cnt;				// Adjust the length, so leading zeroes will be trimmed out

			// Value has reduced...
			if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
				n1.convertToSizeT_ifPossible();
		}
	}
#endif

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 2 ([] += size_t)
// optimized
void longNum::opPlusEqual_2(longNum &n1, const longNum &n2, const size_t n2_length) const
{
	TRACE_CODE_FLOW("longNum::opPlusEqual_2");

	// abs(n1) always > abs(n2)

	size_t i(0u);
	size_t *ptr = const_cast<size_t*>(&n2_length);

	if (n1._sign == n2._sign)
	{
		// Perform adding in place

		digitType carryOver(0), * digit(n1._values);

		while (*ptr)
		{
			*digit += *ptr % BASE + carryOver;

			*ptr /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
			++i;
		}

		for (; carryOver && i < n1._length; ++i)
		{
			*digit += carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
		}

		// If carryOver > 0, we need to reallocate and normalize
		if (carryOver)
		{
			TRACE_MSG_IF1("Alloc (opPlusEqual_2)");

			digitType* data = new digitType[n1._length + 1u];

			memcpy(data, n1._values, sizeof(digitType)* n1._length);
			data[n1._length++] = carryOver;

			delete[] n1._values;
			n1._values = data;
		}
	}
	else
	{
		// Subtract numbers in place

		size_t cnt(0u);
		digitType carryOver(0), *digit(n1._values);

		while (*ptr)
		{
			*digit -= (*ptr % BASE + carryOver);

			*ptr /= BASE;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
			++i;
		}

		for (; (cnt || carryOver) && i < n1._length; ++i)
		{
			*digit -= carryOver;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		n1._length -= cnt;

		// Need to check if we can store the number as a size_t
		if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
			n1.convertToSizeT_ifPossible();
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 3 (size_t += [])
// optimized
void longNum::opPlusEqual_3(longNum& n1, const longNum& n2, const size_t n1_length) const
{
	TRACE_CODE_FLOW("longNum::opPlusEqual_3");

	// abs(n1) always < abs(n2)

	size_t i(0u);
	size_t *ptr = const_cast<size_t*>(&n1_length);

	TRACE_MSG_IF1("Alloc (opPlusEqual_3)");

	if (n1._sign == n2._sign)
	{
		n1._values = new digitType[n2._length + 1u];
		digitType carryOver(0);
		digitType* digit(n1._values);

		while (*ptr)
		{
			*digit = n2._values[i] + *ptr % BASE + carryOver;

			*ptr /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
			++i;
		}

		for (; i < n2._length; ++i)
		{
			*digit = n2._values[i] + carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
		}

		if (carryOver)
		{
			n1._values[i++] = carryOver;
		}

		n1._length = i;
	}
	else
	{
		n1._values = new digitType[n2._length];
		size_t cnt(0u);
		digitType carryOver(0);
		digitType* digit(n1._values);

		while (*ptr)
		{
			*digit = n2._values[i] - (*ptr % BASE + carryOver);

			*ptr /= BASE;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
			++i;
		}

		for (; i < n2._length; ++i)
		{
			*digit = n2._values[i] - carryOver;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		n1._length = i - cnt;
		n1._sign = n2._sign;

		// Need to check if we can store the number as a size_t
		if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
			n1.convertToSizeT_ifPossible();
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator += helper 4 (size_t += size_t)
// optimized
void longNum::opPlusEqual_4(longNum& n1, const longNum& n2) const
{
	TRACE_CODE_FLOW("longNum::opPlusEqual_4");

	if (n1._sign == n2._sign)
	{
		// Add the numbers and keep the sign

		size_t length = n1._length + n2._length;

#if defined _IS_LESSER_

		// Overflow: need to allocate memory
		if (length > longNum_MAX_VALUE)
		{
			TRACE_MSG_IF1("Alloc (opPlusEqual_4)");

			n1._values = new digitType[longNum_MAX_SIZE_T_LENGTH];
			n1._length = longNum_MAX_SIZE_T_LENGTH;
			digitType* digit(n1._values);

			size_t MAX(longNum_MAX_VALUE);
			digitType carryOver(1);

			// Imitate size_t overflow
			length -= (longNum_MAX_VALUE + 1);

			while (length)
			{
				*digit = MAX % BASE + length % BASE + carryOver;

				length /= BASE;
				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			while (MAX)
			{
				*digit = MAX % BASE + carryOver;

				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}
		}
		else
		{
			n1._length = length;
		}

#else

		// Overflow: need to allocate memory
		if (length < n1._length || length < n2._length)
		{
			TRACE_MSG_IF1("Alloc (opPlusEqual_4)");

			n1._values = new digitType[longNum_MAX_SIZE_T_LENGTH];
			n1._length = longNum_MAX_SIZE_T_LENGTH;
			digitType *digit(n1._values);

			size_t MAX(longNum_MAX_VALUE);
			digitType carryOver(1);

			while (length)
			{
				*digit = MAX % BASE + length % BASE + carryOver;

				length /= BASE;
				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			while (MAX)
			{
				*digit = MAX % BASE + carryOver;

				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}
		}
		else
		{
			n1._length = length;
		}

#endif
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
		n1._sign = n1._length ? pn1->_sign : POS;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

longNum longNum::operator -(const longNum& other) const
{
	TRACE_CODE_FLOW("longNum::operator -(const longNum &)");

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
	TRACE_CODE_FLOW("longNum::operator -(const Type)");

#if 0

	return std::move(*this - longNum(other));

#else

	// This version is only SLIGHTLY faster
	longNum res;
	res._sign = other >= 0;

	if (_sign == res._sign)
	{
		// Templated version
		if (subtr2positive(*this, other, res))
			res._sign = !res._sign;
	}
	else
	{
		// Templated version
		add2positive(*this, other, res);
		res._sign = _sign;
	}

	return std::move(res);

#endif
}

// -----------------------------------------------------------------------------------------------

// TODO: make this work
template <>
longNum longNum::operator -(const char* str) const
{
	TRACE_CODE_FLOW("longNum::operator -(const char *)");

	return longNum(0);
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator -=(const longNum& other)
{
	TRACE_CODE_FLOW("longNum::operator -=(const longNum &)");

#if 0

	*this = (*this - other);

#else

	if (_values)
	{
		if (other._values)
		{
			opMinusEqual_1(*this, other);
		}
		else
		{
			opMinusEqual_2(*this, other, other._length);
		}
	}
	else
	{
		if (other._values)
		{
			opMinusEqual_3(*this, other, _length);
		}
		else
		{
			opMinusEqual_4(*this, other);
		}
	}

#endif

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator -=(longNum&& other) noexcept
{
	TRACE_CODE_FLOW("longNum::operator -=(longNum &&)");

	// (a -= b) <==> (a += -b)
	// And we don't care about 'other', as it is an RValue
	other._sign = !other._sign;

	return *this += std::move(other);
}

// -----------------------------------------------------------------------------------------------

// []	  -= Type
// size_t -= Type
template <class Type>
longNum& longNum::operator -=(const Type other)
{
	TRACE_CODE_FLOW("longNum::operator -=(const Type)");

	// For signed types, it is faster to just go with longNum(other)
	if (Type(-1) < Type(0))
	{
		return *this -= longNum(other);
	}

	Type* ptr = const_cast<Type*>(&other);

	if (_values)
	{
		size_t cnt(0u), i(0u);
		digitType carryOver(0);
		digitType* digit(_values);

		if (_sign)
		{
			// Subtract numbers in place

			while (*ptr)
			{
				*digit -= (*ptr % BASE + carryOver);

				*ptr /= BASE;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
				++i;
			}

			for (; (cnt || carryOver) && i < _length; ++i)
			{
				*digit -= carryOver;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
			}

			_length -= cnt;

			// Need to check if we can store the number as a size_t
			if (_length <= longNum_MAX_SIZE_T_LENGTH)
				convertToSizeT_ifPossible();
		}
		else
		{
			// Perform adding in place

			while (*ptr)
			{
				*digit += *ptr % BASE + carryOver;

				*ptr /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
				++i;
			}

			for (; carryOver && i < _length; ++i)
			{
				*digit += carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
				TRACE_MSG_IF1("Alloc (operator -= <Type>)");

				digitType* data = new digitType[_length + 1u];

				memcpy(data, _values, sizeof(digitType) * _length);
				data[_length++] = carryOver;

				delete[] _values;
				_values = data;
			}
		}
	}
	else
	{
		if (_sign)
		{
			// Subtract lesser from greater and keep the greater's sign

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
		else
		{
			// Add the numbers and keep the sign

			size_t length = _length + other;

#if defined _IS_LESSER_

			// Overflow: need to allocate memory
			if (length < _length || length < other)
			{
				TRACE_MSG_IF1("Alloc (operator -= <Type>)");

				size_t MAX(longNum_MAX_VALUE), i(0u);
				digitType carryOver(1);

				digitType* data = new digitType[longNum_MAX_SIZE_T_LENGTH], * digit(data);

				// Imitate size_t overflow
				length -= (longNum_MAX_VALUE + 1);

				while (length)
				{
					*digit = MAX % BASE + length % BASE + carryOver;

					length /= BASE;
					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				while (MAX)
				{
					*digit = MAX % BASE + carryOver;

					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				_length = i;
				_values = data;
			}
			else
			{
				_length = length;
			}

#else

			// Overflow: need to allocate memory
			if (length < _length || length < other)
			{
				TRACE_MSG_IF1("Alloc (operator -= <Type>)");

				size_t MAX(longNum_MAX_VALUE), i(0u);
				digitType carryOver(1);

				digitType* data = new digitType[longNum_MAX_SIZE_T_LENGTH], * digit(data);

				while (length)
				{
					*digit = MAX % BASE + length % BASE + carryOver;

					length /= BASE;
					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				while (MAX)
				{
					*digit = MAX % BASE + carryOver;

					MAX /= BASE;

					carryOver = *digit >= BASE ? 1 : 0;
					*digit++ = carryOver ? *digit - BASE : *digit;
					++i;
				}

				_length = i;
				_values = data;
			}
			else
			{
				_length = length;
			}

#endif
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <>
longNum& longNum::operator -=(const char* str)
{
	TRACE_CODE_FLOW("longNum::operator -=(const char *)");

	// TODO: rewrite with in place operation and test if this is faster
	return *this -= longNum(str);
}

// -----------------------------------------------------------------------------------------------

// operator -= helper 1 ([] -= [])
void longNum::opMinusEqual_1(longNum& n1, const longNum& n2) const
{
	TRACE_CODE_FLOW("longNum::opMinusEqual_1");

	if (n1._sign == n2._sign)
	{
		// In case the signs are the same, we subtract smaller number from the greater

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

			size_t i(0u), cnt(0u);
			digitType carryOver(0);

			if (isGreater > 1)
			{
				// n1 < n2
				pn1 = &n2;
				pn2 = &n1;

				n1._sign = !n1._sign;
			}

			if (isGreater == 3)
			{
				// Will need another buffer to store the result

				TRACE_MSG_IF1("Alloc tmp buffer (opMinusEqual_1)");

				digitType* res = new digitType[pn1->_length];
				digitType* digit(res);

				for (; i < pn2->_length; ++i)
				{
					*digit = pn1->_values[i] - (pn2->_values[i] + carryOver);

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				for (; i < pn1->_length; ++i)
				{
					*digit = pn1->_values[i] - carryOver;

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				delete[] n1._values;
				n1._values = res;
			}
			else
			{
				// n1 > n2: No need for realloc. Subtract numbers in place
				digitType* digit(n1._values);

				for (; i < pn2->_length; ++i)
				{
					*digit = pn1->_values[i] - (pn2->_values[i] + carryOver);

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}

				// When cnt == 0 and carryOver == 0, we break out of the loop
				for (; (cnt || carryOver) && i < pn1->_length; ++i)
				{
					*digit = pn1->_values[i] - carryOver;

					carryOver = *digit >= 0 ? 0 : 1;
					*digit += carryOver ? BASE : 0;
					cnt = *digit++ ? 0 : cnt + 1u;
				}
			}

			n1._length = pn1->_length - cnt;

			// Value has reduced...
			if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
				n1.convertToSizeT_ifPossible();
		}
	}
	else
	{
		// In case the signs are opposite, we add the numbers and keep the sign.
		// Also, there's a chance we'll be able to add the 2 numbers without reallocation
		if (n1._length < n2._length)
		{
			// Go with full alloc cycle
			n1 = std::move(n1 - n2);
		}
		else
		{
			// Perform adding in place
			size_t i(0u);
			digitType carryOver(0), * digit(n1._values);

			for (; i < n2._length; ++i)
			{
				*digit += n2._values[i] + carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			for (; carryOver && i < n1._length; ++i)
			{
				*digit += carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			// If carryOver > 0, we need to reallocate and normalize
			if (carryOver)
			{
				TRACE_MSG_IF1("Alloc (opMinusEqual_1)");

				digitType* data = new digitType[n1._length + 1u];

				memcpy(data, n1._values, sizeof(digitType) * n1._length);
				data[n1._length++] = carryOver;

				delete[] n1._values;
				n1._values = data;
			}
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator -= helper 2 ([] -= size_t)
void longNum::opMinusEqual_2(longNum& n1, const longNum& n2, const size_t n2_length) const
{
	TRACE_CODE_FLOW("longNum::opMinusEqual_2");

	// abs(n1) always > abs(n2)

	size_t i(0u);
	size_t* ptr = const_cast<size_t*>(&n2_length);

	if (n1._sign == n2._sign)
	{
		// Subtract numbers in place

		size_t cnt(0u);
		digitType carryOver(0), * digit(n1._values);

		while (*ptr)
		{
			*digit -= (*ptr % BASE + carryOver);

			*ptr /= BASE;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
			++i;
		}

		for (; (cnt || carryOver) && i < n1._length; ++i)
		{
			*digit -= carryOver;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		n1._length -= cnt;

		// Need to check if we can store the number as a size_t
		if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
			n1.convertToSizeT_ifPossible();
	}
	else
	{
		// Perform adding in place

		digitType carryOver(0), * digit(n1._values);

		while (*ptr)
		{
			*digit += *ptr % BASE + carryOver;

			*ptr /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
			++i;
		}

		for (; carryOver && i < n1._length; ++i)
		{
			*digit += carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
		}

		// If carryOver > 0, we need to reallocate and normalize
		if (carryOver)
		{
			TRACE_MSG_IF1("Alloc (opMinusEqual_2)");

			digitType* data = new digitType[n1._length + 1u];

			memcpy(data, n1._values, sizeof(digitType) * n1._length);
			data[n1._length++] = carryOver;

			delete[] n1._values;
			n1._values = data;
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator -= helper 3 (size_t -= [])
void longNum::opMinusEqual_3(longNum& n1, const longNum& n2, const size_t n1_length) const
{
	TRACE_CODE_FLOW("longNum::opMinusEqual_3");

	// abs(n1) always < abs(n2)

	size_t i(0u);
	size_t* ptr = const_cast<size_t*>(&n1_length);

	TRACE_MSG_IF1("Alloc (opMinusEqual_3)");

	if (n1._sign == n2._sign)
	{
		n1._values = new digitType[n2._length];
		size_t cnt(0u);
		digitType carryOver(0);
		digitType* digit(n1._values);

		while (*ptr)
		{
			*digit = n2._values[i] - (*ptr % BASE + carryOver);

			*ptr /= BASE;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
			++i;
		}

		for (; i < n2._length; ++i)
		{
			*digit = n2._values[i] - carryOver;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		n1._length = i - cnt;
		n1._sign = !n2._sign;

		// Need to check if we can store the number as a size_t
		if (n1._length <= longNum_MAX_SIZE_T_LENGTH)
			n1.convertToSizeT_ifPossible();
	}
	else
	{
		n1._values = new digitType[n2._length + 1u];
		digitType carryOver(0);
		digitType* digit(n1._values);

		while (*ptr)
		{
			*digit = n2._values[i] + *ptr % BASE + carryOver;

			*ptr /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
			++i;
		}

		for (; i < n2._length; ++i)
		{
			*digit = n2._values[i] + carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ = carryOver ? *digit - BASE : *digit;
		}

		if (carryOver)
		{
			n1._values[i++] = carryOver;
		}

		n1._length = i;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// operator -= helper 4 (size_t -= size_t)
void longNum::opMinusEqual_4(longNum& n1, const longNum& n2) const
{
	TRACE_CODE_FLOW("longNum::opMinusEqual_4");

	if (n1._sign == n2._sign)
	{
		// Subtract lesser from greater and keep the greater's sign

		const longNum* pn1 = &n1;
		const longNum* pn2 = &n2;

		if (n1._length < n2._length)
		{
			pn1 = &n2;
			pn2 = &n1;

			n1._sign = !n1._sign;
		}

		n1._length = pn1->_length - pn2->_length;
		n1._sign = n1._length ? n1._sign : POS;
	}
	else
	{
		// Add the numbers and keep the sign

		size_t length = n1._length + n2._length;

#if defined _IS_LESSER_

		// Overflow: need to allocate memory
		if (length > longNum_MAX_VALUE)
		{
			TRACE_MSG_IF1("Alloc (opMinusEqual_4)");

			n1._values = new digitType[longNum_MAX_SIZE_T_LENGTH];
			n1._length = longNum_MAX_SIZE_T_LENGTH;
			digitType* digit(n1._values);

			size_t MAX(longNum_MAX_VALUE);
			digitType carryOver(1);

			// Imitate size_t overflow
			length -= (longNum_MAX_VALUE + 1);

			while (length)
			{
				*digit = MAX % BASE + length % BASE + carryOver;

				length /= BASE;
				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			while (MAX)
			{
				*digit = MAX % BASE + carryOver;

				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}
		}
		else
		{
			n1._length = length;
		}

#else

		// Overflow: need to allocate memory
		if (length < n1._length || length < n2._length)
		{
			TRACE_MSG_IF1("Alloc (opMinusEqual_4)");

			n1._values = new digitType[longNum_MAX_SIZE_T_LENGTH];
			n1._length = longNum_MAX_SIZE_T_LENGTH;
			digitType* digit(n1._values);

			size_t MAX(longNum_MAX_VALUE);
			digitType carryOver(1);

			while (length)
			{
				*digit = MAX % BASE + length % BASE + carryOver;

				length /= BASE;
				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			while (MAX)
			{
				*digit = MAX % BASE + carryOver;

				MAX /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}
		}
		else
		{
			n1._length = length;
		}

#endif
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// This implementation is about 2 times faster than standard "return *this += longNum(1)"
longNum& longNum::operator ++()
{
	TRACE_CODE_FLOW("longNum::operator ++()");

	if (_values)
	{
		digitType* digit(_values);

		if (_sign)
		{
			// [] + 1

			// It is the same as within the for-loop, but without the cycle -- should be a bit faster
			if (*digit != BASE - 1)
			{
				++(*digit);
				return *this;
			}

			if (_values[1] != BASE - 1)
			{
				*digit++ = 0;
				++(*digit);
				return *this;
			}

			if (_values[2] != BASE - 1)
			{
				*digit++ = 0;
				*digit++ = 0;
				++(*digit);
				return *this;
			}

			{
				*digit++ = 0;
				*digit++ = 0;
				*digit++ = 0;

				for (size_t i = 3u; i < _length; ++i)
				{
					if (*digit != BASE - 1)
					{
						++(*digit);
						return *this;
					}

					*digit++ = 0;
				}

				TRACE_MSG_IF3("Alloc for ", _length + 1, " digits (operator++)");

				// In case we reached this point, the array is full of '0's
				// Will need to realloc: 999 + 1 = 1000
				delete[] _values;

				_values = new digitType[_length + 1u];
				memset(_values, 0, _length * sizeof(_values[0]));
				_values[_length] = 1;
				++_length;
			}
		}
		else
		{
			// [] - 1

			// It is the same as within the for-loop, but without the cycle -- should be a bit faster
			if (*digit != 0)
			{
				--(*digit);

				// Need to check if we can store the number as a size_t
				if (_length == longNum_MAX_SIZE_T_LENGTH)
					convertToSizeT_ifPossible();

				return *this;
			}

			if (_values[1] != 0)
			{
				*digit++ = BASE - 1;
				--(*digit);
				return *this;
			}

			if (_values[2] != 0)
			{
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;
				--(*digit);
				return *this;
			}

			{
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;

				for (size_t i = 3u; i < _length; ++i)
				{
					if (*digit != 0)
					{
						--(*digit);

						if (*digit == 0 && (i + 1u) == _length)
							--_length;

						return *this;
					}

					*digit++ = BASE - 1;
				}
			}
		}
	}
	else
	{
		if (_sign)
		{
			// size_t + 1

			_length++;

#if defined _IS_LESSER_
			// Imitate overflow:
			if (_length > longNum_MAX_VALUE)
				_length = 0u;

			fill_maxSizeT();
#endif

			if (_length == 0u)
			{
				// Overflow. Allocate the array
				TRACE_MSG_IF3("Alloc for ", longNum_MAX_SIZE_T_LENGTH, " digits (operator++)");

				_values = new digitType[longNum_MAX_SIZE_T_LENGTH];
				_length = longNum_MAX_SIZE_T_LENGTH;

				for (size_t i = 0; i < longNum_MAX_SIZE_T_LENGTH; ++i)
					_values[i] = maxSizeT[i];

				_values[0]++;
			}
		}
		else
		{
			// size_t - 1

			_sign = !--_length ? POS : _sign;
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator --()
{
	TRACE_CODE_FLOW("longNum::operator --()");

	if (_values)
	{
		digitType* digit(_values);

		if (_sign)
		{
			// It is the same as within the for-loop, but without the cycle -- should be a bit faster
			if (*digit != 0)
			{
				--(*digit);

				// Need to check if we can store the number as a size_t
				if (_length == longNum_MAX_SIZE_T_LENGTH)
					convertToSizeT_ifPossible();

				return *this;
			}

			if (_values[1] != 0)
			{
				*digit++ = BASE - 1;
				--(*digit);
				return *this;
			}

			if (_values[2] != 0)
			{
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;
				--(*digit);
				return *this;
			}

			{
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;
				*digit++ = BASE - 1;

				for (size_t i = 3u; i < _length; ++i)
				{
					if (*digit != 0)
					{
						--(*digit);

						if (*digit == 0 && (i + 1u) == _length)
							--_length;

						return *this;
					}

					*digit++ = BASE - 1;
				}
			}
		}
		else
		{
			// [] + 1

			// It is the same as within the for-loop, but without the cycle -- should be a bit faster
			if (*digit != BASE - 1)
			{
				++(*digit);
				return *this;
			}

			if (_values[1] != BASE - 1)
			{
				*digit++ = 0;
				++(*digit);
				return *this;
			}

			if (_values[2] != BASE - 1)
			{
				*digit++ = 0;
				*digit++ = 0;
				++(*digit);
				return *this;
			}

			{
				*digit++ = 0;
				*digit++ = 0;
				*digit++ = 0;

				for (size_t i = 3u; i < _length; ++i)
				{
					if (*digit != BASE - 1)
					{
						++(*digit);
						return *this;
					}

					*digit++ = 0;
				}

				TRACE_MSG_IF3("Alloc for ", _length + 1, " digits (operator--)");

				// In case we reached this point, the array is full of '0's
				// Will need to realloc: 999 + 1 = 1000
				delete[] _values;

				_values = new digitType[_length + 1u];
				memset(_values, 0, _length * sizeof(_values[0]));
				_values[_length] = 1;
				++_length;
			}
		}
	}
	else
	{
		if (_sign)
		{
			// size_t - 1

			if (_length--)
				return *this;

			_length = 1u;
			_sign = NEG;
		}
		else
		{
			// size_t + 1

			_length++;

#if defined _IS_LESSER_
			// Imitate overflow:
			if (_length > longNum_MAX_VALUE)
				_length = 0u;

			fill_maxSizeT();
#endif

			if (!_length)
			{
				// Overflow. Allocate the array
				TRACE_MSG_IF3("Alloc for ", longNum_MAX_SIZE_T_LENGTH, " digits (operator--)");

				_values = new digitType[longNum_MAX_SIZE_T_LENGTH];
				_length = longNum_MAX_SIZE_T_LENGTH;

				for (size_t i = 0; i < longNum_MAX_SIZE_T_LENGTH; ++i)
					_values[i] = maxSizeT[i];

				_values[0]++;
			}
		}
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum::operator bool() const
{
	TRACE_CODE_FLOW("longNum::operator bool()");

	return _values ? (_length != 1u || _values[0] != 0) : (_length != 0u);
}

// -----------------------------------------------------------------------------------------------

// Returns number as a string in a normal readable order
std::string longNum::get() const
{
	TRACE_CODE_FLOW("longNum::get()");

	if (_values)
	{
		// Return value stored as array
		digitType* digit(_values + _length - 1);
		std::string str;

		if (_sign)
		{
			str.resize(_length);

			for (size_t i = 0u; i < _length; ++i)
				str[i] = static_cast<char>(*digit-- + 48);
		}
		else
		{
			str.resize(_length + 1);
			str[0] = '-';

			for (size_t i = 0u; i < _length; ++i)
				str[i+1u] = static_cast<char>(*digit-- + 48);
		}

		return str;
	}

	// Return value stored in _length
	return std::to_string(_length);
}

// -----------------------------------------------------------------------------------------------

// Returns values as a size_t
// No sign is returned
// No check for size_t overflow is performed
size_t longNum::as_size_t() const
{
	TRACE_CODE_FLOW("longNum::as_size_t()");

	if (_values)
	{
		size_t res(0);

		for (size_t i = 0; i < _length; ++i)
		{
			res *= BASE;
			res += _values[_length - i - 1];
		}

		return res;
	}

	return _length;
}

// -----------------------------------------------------------------------------------------------

// operator +/- helper: Adds 2 positive longNums
void longNum::add2positive(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::add2positive()");

	if (n1._values)
	{
		if (n2._values)
		{
			// Add 2 positive numbers: [] + []
			add2positive_1(n1, n2, res);
		}
		else
		{
			// Add 2 positive numbers: [] + size_t
			add2positive_3(n1, n2._length, res);
		}
	}
	else
	{
		if (n2._values)
		{
			// Add 2 positive numbers: [] + size_t
			add2positive_3(n2, n1._length, res);
		}
		else
		{
			// Add 2 positive numbers: size_t + size_t
			add2positive_2(n1, n2, res);
		}
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive longNums: [] + []
void longNum::add2positive_1(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::add2positive_1");

	// The longest number will be the first
	const longNum* p1(&n1);
	const longNum* p2(&n2);

	if (n1._length < n2._length)
	{
		p1 = &n2;
		p2 = &n1;
	}

	TRACE_MSG_IF1("Alloc (add2positive_1)");

	digitType *data = new digitType[p1->_length + 1u];

	memcpy(data, p1->_values, sizeof(digitType) * p1->_length);

	size_t i(0u);
	digitType carryOver(0);
	digitType *digit(data);

	// 2 for-loops work slightly faster than a single one
	for (; i < p2->_length; ++i)
	{
		*digit += p2->_values[i] + carryOver;

		carryOver = (*digit >= BASE) ? 1 : 0;
		*digit++ += carryOver ? -BASE : 0;
	}

	for (; carryOver && i < p1->_length; ++i)
	{
		*digit += carryOver;

		carryOver = (*digit >= BASE) ? 1 : 0;
		*digit++ += carryOver ? -BASE : 0;
	}

	res._length = p1->_length;
	res._values = data;

	if (carryOver)
	{
		res._values[res._length++] = carryOver;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive numbers: size_t + size_t
void longNum::add2positive_2(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::add2positive_2");

	size_t len = n1._length + n2._length;

#if defined _IS_LESSER_
  #define OVERFLOW_CONDITION	len > longNum_MAX_VALUE
  #define IMITATE_OVERFLOW		len -= (longNum_MAX_VALUE + 1);
#else
  #define OVERFLOW_CONDITION	len < n1._length || len < n2._length
  #define IMITATE_OVERFLOW		;
#endif

	// Overflow: need to allocate memory
	if (OVERFLOW_CONDITION)
	{
		// Imitate size_t overflow
		IMITATE_OVERFLOW;

		TRACE_MSG_IF1("Alloc (add2positive_2)");

		digitType *data = new digitType[longNum_MAX_SIZE_T_LENGTH];

		size_t i(0u);
		digitType carryOver(1);
		digitType* digit(data);

		while (len)
		{
			*digit = maxSizeT[i] + len % BASE + carryOver;

			len /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ -= carryOver ? BASE : 0;
			++i;
		}

		for (; i < longNum_MAX_SIZE_T_LENGTH; ++i)
		{
			*digit = maxSizeT[i] + carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ -= carryOver ? BASE : 0;
		}

		res._values = data;
		res._length = longNum_MAX_SIZE_T_LENGTH;

		return;
	}

	res._length = len;

	return;

#undef OVERFLOW_CONDITION
#undef IMITATE_OVERFLOW
}

// -----------------------------------------------------------------------------------------------

// Add 2 positive numbers: [] + size_t
void longNum::add2positive_3(const longNum& n1, const size_t n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::add2positive_3");

	size_t* ptr = const_cast<size_t*>(&n2);

	TRACE_MSG_IF1("Alloc (add2positive_3)");
	res._length = n1._length;
	res._values = new digitType[n1._length + 1u];

	size_t i(0u);
	digitType carryOver(0), * digit(res._values);

	// Copy 'other' into 'this' and then perform in place operation
	// This is about 2.5 times faster
	// TODO: check if for a shorter arrays it is faster just to copy without memcpy
	memcpy(res._values, n1._values, sizeof(digitType) * n1._length);

	while (*ptr && ++i)
	{
		*digit += *ptr % BASE + carryOver;

		*ptr /= BASE;

		carryOver = *digit >= BASE ? 1 : 0;
		*digit++ += carryOver ? -BASE : 0;
	}

	for (; carryOver && i < n1._length; ++i)
	{
		*digit += carryOver;

		carryOver = *digit >= BASE ? 1 : 0;
		*digit++ = carryOver ? *digit - BASE : *digit;
	}

	if (carryOver)
	{
		res._values[res._length++] = carryOver;
	}

	return;
}

// -----------------------------------------------------------------------------------------------

// Pre-requirement: res._values is not allocated yet
template <class Type>
void longNum::add2positive(const longNum& n1, const Type n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::add2positive <Type>");

	size_t i(0u);
	Type* ptr = const_cast<Type*>(&n2);

	// #pragma warning (disable:4146)
	// This piece of code won't affect any unsigned types.
	// But when we pass unsigned type as template Type, the compiler ignores the fact that the value is compared with zero,
	// but still says we can't assign *tmp to -*tmp
	// So we disable this warning for good

	MAKE_POSITIVE(*ptr);

	// --- Add 2 positive numbers: [] + Type ---

	if (n1._values)
	{
		TRACE_MSG_IF1("Alloc (add2positive<Type>)");

		digitType carryOver(0);
		digitType *data = new digitType[n1._length + 1u];
		digitType* digit(data);

		// TODO: magic number 28 which is an experimental value. make it global constant later
		// With longer arrays, copying n1 and then adding in place is faster
		if (n1._length > 28u)
		{
			memcpy(data, n1._values, sizeof(digitType) * n1._length);

			while (n2 && ++i)
			{
				*digit += n2 % BASE + carryOver;

				*ptr /= BASE;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			for (; carryOver && i < n1._length; ++i)
			{
				*digit += carryOver;

				carryOver = *digit >= BASE ? 1 : 0;
				*digit++ = carryOver ? *digit - BASE : *digit;
			}

			res._length = n1._length;
			res._values = data;

			if (carryOver)
				res._values[res._length++] = carryOver;

			return;
		}


		digitType* n1digit(n1._values);
		digitType nRes(0);

		// With shorter arrays, we're doing simple adding 
		while (n2)
		{
			nRes = *n1digit++ + n2 % BASE + carryOver;

			*ptr /= BASE;

			carryOver = nRes >= BASE ? 1 : 0;
			data[i] = carryOver ? nRes - BASE : nRes;
			++i;
		}

		for (; i < n1._length; ++i)
		{
			nRes = *n1digit++ + carryOver;

			carryOver = nRes >= BASE ? 1 : 0;
			data[i] = carryOver ? nRes - BASE : nRes;
		}

		res._length = n1._length;
		res._values = data;

		if (carryOver)
			res._values[res._length++] = carryOver;

		return;
	}


	// --- Add 2 positive numbers: size_t + Type ---

#if defined _IS_LESSER_
  #define OVERFLOW_CONDITION	len > longNum_MAX_VALUE
  #define IMITATE_OVERFLOW		len -= (longNum_MAX_VALUE + 1);
#else
  #define OVERFLOW_CONDITION	len < n1._length || len < n2
  #define IMITATE_OVERFLOW		;
#endif

	size_t len = n1._length + n2;

	// Overflow: need to allocate memory
	if (OVERFLOW_CONDITION)
	{
		// Imitate size_t overflow
		IMITATE_OVERFLOW;

		TRACE_MSG_IF1("Alloc (add2positive<Type>)");

		digitType* data = new digitType[longNum_MAX_SIZE_T_LENGTH];

		size_t i(0u);
		digitType carryOver(1);
		digitType* digit(data);

		while (len)
		{
			*digit = maxSizeT[i] + len % BASE + carryOver;

			len /= BASE;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ -= carryOver ? BASE : 0;
			++i;
		}

		for (; i < longNum_MAX_SIZE_T_LENGTH; ++i)
		{
			*digit = maxSizeT[i] + carryOver;

			carryOver = *digit >= BASE ? 1 : 0;
			*digit++ -= carryOver ? BASE : 0;
		}

		res._values = data;
		res._length = longNum_MAX_SIZE_T_LENGTH;

		return;
	}

	res._length = len;

	return;

#undef OVERFLOW_CONDITION
#undef IMITATE_OVERFLOW
}

// -----------------------------------------------------------------------------------------------

// operator +/- helper: Subtracts 2 positive longNums
// Returns:
//	false : n1 >= n2
//	true  : n1 <  n2
bool longNum::subtr2positive(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::subtr2positive()");

	if (n1._values)
	{
		if (n2._values)
		{
			// Subtract 2 positive numbers: [] - []
			return subtr2positive_1(n1, n2, res);
		}
		else
		{
			// Subtract 2 positive numbers: [] - size_t
			return subtr2positive_3(n1, n2._length, res);
		}
	}
	else
	{
		if (n2._values)
		{
			// Subtract 2 positive numbers: [] - size_t
			return !subtr2positive_3(n2, n1._length, res);
		}
		else
		{
			// Subtract 2 positive numbers: size_t - size_t
			return subtr2positive_2(n1, n2, res);
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers: longNum([]) - longNum([])
bool longNum::subtr2positive_1(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::subtr2positive_1");

	bool bRes = true;

	// First, we need to determine which number's absolute value is greater
	int isGreater = absValueIsLarger(n1, n2);

	// The numbers are equal: just return 0
	if (!isGreater)
	{
		TRACE_MSG_IF1("longNum converted to size_t : 0");

		if (res._values)
			delete[] res._values;

		res._values = nullptr;
		res._length = 0;
		res._sign = POS;

		return false;
	}

	const longNum* pn1(&n2);
	const longNum* pn2(&n1);

	if (isGreater == 1)
	{
		pn1 = &n1;
		pn2 = &n2;
		bRes = false;
	}

	TRACE_MSG_IF1("Alloc (subtr2positive_1)");

	size_t cnt(0u), i(0u);
	digitType carryOver(0);
	digitType* data = new digitType[pn1->_length];
	digitType* digit(data);
	digitType* pn2digit(pn2->_values);

	memcpy(data, pn1->_values, sizeof(digitType) * pn1->_length);

	for (; i < pn2->_length; ++i)
	{
		*digit -= (*pn2digit++ + carryOver);

		carryOver = *digit >= 0 ? 0 : 1;
		*digit += carryOver ? BASE : 0;
		cnt = *digit++ ? 0 : cnt + 1u;
	}

	for (; (carryOver || cnt) && i < pn1->_length; ++i)
	{
		*digit -= carryOver;

		carryOver = *digit >= 0 ? 0 : 1;
		*digit += carryOver ? BASE : 0;
		cnt = *digit++ ? 0 : cnt + 1u;
	}

	res._values = data;
	res._length = pn1->_length - cnt;		// Adjust the length, so leading zeroes will be trimmed out
	res._sign = pn1->_sign;

	// Need to check if we can store the number as a size_t
	if (res._length <= longNum_MAX_SIZE_T_LENGTH)
		res.convertToSizeT_ifPossible();

	return bRes;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers: longNum(size_t) - longNum(size_t)
bool longNum::subtr2positive_2(const longNum& n1, const longNum& n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::subtr2positive_2");

	if (n1._length < n2._length)
	{
		res._length = n2._length - n1._length;
		res._sign = n2._sign;
		return true;
	}

	res._length = n1._length - n2._length;
	res._sign = res._length ? n1._sign : POS;

	return false;
}

// -----------------------------------------------------------------------------------------------

// Subtract 2 positive numbers: longNum([]) - longNum(size_t)
// If the resulting number can be stored as a size_t, we'll have to store it like that
bool longNum::subtr2positive_3(const longNum& n1, const size_t n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::subtr2positive_3");

	size_t *ptr = const_cast<size_t*>(&n2);

	TRACE_MSG_IF1("Alloc (subtr2positive_3)");

	size_t cnt(0u), i(0u);
	digitType carryOver(0);
	digitType *data = new digitType[n1._length];
	digitType* digit(data);

	memcpy(data, n1._values, sizeof(digitType) * n1._length);

	while (n2 && ++i)
	{
		*digit -= (n2 % BASE + carryOver);

		*ptr /= BASE;

		carryOver = *digit >= 0 ? 0 : 1;
		*digit += carryOver ? BASE : 0;
		cnt = *digit++ ? 0 : cnt + 1u;
	}

	for (; (carryOver || cnt) && i < n1._length; ++i)
	{
		*digit -= carryOver;

		carryOver = *digit >= 0 ? 0 : 1;
		*digit += carryOver ? BASE : 0;
		cnt = *digit++ ? 0 : cnt + 1u;
	}

	res._values = data;
	res._length = n1._length - cnt;
	res._sign = n1._sign;

	// Need to check if we can store the number as a size_t
	if (res._length <= longNum_MAX_SIZE_T_LENGTH)
		res.convertToSizeT_ifPossible();

	return false;
}

// -----------------------------------------------------------------------------------------------

// Subtracts 2 positive numbers
// Returns:
//	false : n1 >= n2
//	true  : n1 <  n2
// Knowin issues:
//  1. If n1 is an array, n2 must not be greater than n1. In this case the result will be wrong. This is possible when _IS_LESSER_ is defined
template <class Type>
bool longNum::subtr2positive(const longNum& n1, const Type n2, longNum& res) const
{
	TRACE_CODE_FLOW("longNum::subtr2positive <Type>");

	Type* ptr = const_cast<Type*>(&n2);

	// #pragma warning (disable:4146)
	MAKE_POSITIVE(*ptr);

	// --- Subtract 2 positive numbers: [] - Type ---

	if (n1._values)
	{
		TRACE_MSG_IF1("Alloc (subtr2positive<Type>)");

		size_t cnt(0u), i(0u);

		digitType carryOver(0);
		digitType* data = new digitType[n1._length];
		digitType* digit(data);
		digitType* n1digit(n1._values);

		// TODO: Empirically gained magic number. To const
		// With longer arrays, copying n1 and then adding in place is faster
		if (n1._length > 21u)
		{
			memcpy(data, n1._values, sizeof(digitType) * n1._length);

			while (n2 && ++i)
			{
				*digit -= (n2 % BASE + carryOver);

				*ptr /= BASE;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
			}

			for (; (cnt || carryOver) && i < n1._length; ++i)
			{
				*digit -= carryOver;

				carryOver = *digit >= 0 ? 0 : 1;
				*digit += carryOver ? BASE : 0;
				cnt = *digit++ ? 0 : cnt + 1u;
			}

			res._length = n1._length - cnt;
			res._values = data;
			res._sign = n1._sign;

			// Need to check if we can store the number as a size_t
			if (res._length <= longNum_MAX_SIZE_T_LENGTH)
				res.convertToSizeT_ifPossible();

			return false;
		}


		// With shorter arrays, we're doing simple adding 
		while (n2 && ++i)
		{
			*digit = *n1digit++ - (n2 % BASE + carryOver);

			*ptr /= BASE;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		for (; i < n1._length; ++i)
		{
			*digit = *n1digit++ - carryOver;

			carryOver = *digit >= 0 ? 0 : 1;
			*digit += carryOver ? BASE : 0;
			cnt = *digit++ ? 0 : cnt + 1u;
		}

		res._length = n1._length - cnt;
		res._values = data;
		res._sign = n1._sign;					// Sign is determined by the greater number

		if (res._length <= longNum_MAX_SIZE_T_LENGTH)
			res.convertToSizeT_ifPossible();

		return false;
	}


	// --- Subtract 2 positive numbers: size_t - Type ---

	if (n1._length < n2)
	{
		res._length = n2 - n1._length;
		return true;
	}

	res._length = n1._length - n2;
	res._sign = res._length ? n1._sign : POS;

	return false;
}

// -----------------------------------------------------------------------------------------------

// Operator > for absolute values of 2 longNum([]) numbers
// Returns:
// n1  > n2	-- 1
// n1  < n2	-- 2	-- n2.length = n1.length
// n1  < n2	-- 3	-- n2.length > n1.length
// n1 == n2	-- 0
//
// Let the array be digitType arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
// The number it represents is 987654321
// We're going to reintepret groups of digits as larger type and compare thoses larger type numbers
// sizeof(short)	  = 2
// sizeof(long long ) = 8
// Therefore, we'll be able to compare 8/2 = 4-digit chunks at a time
// The first chunk will be '9876', the second will be '5432', and so on
// This effectively speeds up the process up to 4 times
int longNum::absValueIsLarger(const longNum& n1, const longNum& n2) const
{
	TRACE_CODE_FLOW("longNum::absValueIsLarger()");

	if (n1._length == n2._length)
	{
		long long* n1digitLong = reinterpret_cast<long long*>(n1._values + n1._length);
		long long* n2digitLong = reinterpret_cast<long long*>(n2._values + n1._length);

		for (size_t i = 0; i < n1._length / ratio_ll_to_digitType; ++i)
		{
			n1digitLong--;
			n2digitLong--;

			if (*n1digitLong != *n2digitLong)
				return (*n1digitLong > *n2digitLong) ? 1 : 2;
		}

		if (n1._length % ratio_ll_to_digitType)
		{
			digitType* n1digit(n1._values + n1._length % ratio_ll_to_digitType);
			digitType* n2digit(n2._values + n1._length % ratio_ll_to_digitType);

			for (int i = 0; i < n1._length % ratio_ll_to_digitType; ++i)
			{
				n1digit--;
				n2digit--;

				if (*n1digit != *n2digit)
					return (*n1digit > *n2digit) ? 1 : 2;
			}
		}

		// At this point we know the 2 numbers are equal
		return 0;
	}

	if (n1._length > n2._length)
		return 1;

	return 3;
}

// -----------------------------------------------------------------------------------------------

// Tries to store the number as size_t
// Pre-condition: _values is allocated, _length is holding the actual length, not value
// _length MUST be <= longNum_MAX_SIZE_T_LENGTH!
void longNum::convertToSizeT_ifPossible()
{
	TRACE_CODE_FLOW("longNum::convertToSizeT_ifPossible()");

#if defined _IS_LESSER_

	fill_maxSizeT();
		
#else

	// max size_t = 18446744073709551615

#endif

	auto doTheCalc = [&](size_t i) -> void
	{
		size_t len(0u);

		while (i--)
			len = len * BASE + _values[i];

		delete[] _values;
		_values = nullptr;
		_length = len;
		_sign = len ? _sign : POS;

		TRACE_MSG_IF2("longNum converted to size_t: ", len);
	};

	if (_length == longNum_MAX_SIZE_T_LENGTH)
	{
		if (_values[longNum_MAX_SIZE_T_LENGTH-1] > maxSizeT[longNum_MAX_SIZE_T_LENGTH-1])
			return;

		static size_t shortLen = longNum_MAX_SIZE_T_LENGTH / ratio_ll_to_digitType;

		long long* n1digitLong = reinterpret_cast<long long*>( _values + longNum_MAX_SIZE_T_LENGTH);
		long long* n2digitLong = reinterpret_cast<long long*>(maxSizeT + longNum_MAX_SIZE_T_LENGTH);

		for (size_t i = 0; i < shortLen; ++i)
		{
			n1digitLong--;
			n2digitLong--;

			if (*n1digitLong < *n2digitLong)
				break;

			if (*n1digitLong > *n2digitLong)
				return;
		}

#if defined _IS_LESSER_

		static size_t shortLenRemainder = longNum_MAX_SIZE_T_LENGTH % ratio_ll_to_digitType;

		if (shortLenRemainder)
		{
			digitType* n1digit(	_values + shortLenRemainder);
			digitType* n2digit(maxSizeT + shortLenRemainder);

			for (int i = 0; i < shortLenRemainder; ++i)
			{
				n1digit--;
				n2digit--;

				if (*n1digit < *n2digit)
					break;

				if (*n1digit > *n2digit)
					return;
			}
		}
#endif

		doTheCalc(longNum_MAX_SIZE_T_LENGTH);
		return;
	}

	doTheCalc(_length);

	return;
}

// -----------------------------------------------------------------------------------------------

void longNum::flipSign()
{
	TRACE_CODE_FLOW("longNum::flipSign()");

	_sign = !_sign;
}

// -----------------------------------------------------------------------------------------------

bool longNum::getSign() const
{
	TRACE_CODE_FLOW("longNum::getSign()");

	return _sign;
}

// -----------------------------------------------------------------------------------------------

size_t longNum::getLength() const
{
	TRACE_CODE_FLOW("longNum::getLength()");

	return _length;
}

// -----------------------------------------------------------------------------------------------

longNum::digitType* longNum::getValues() const
{
	TRACE_CODE_FLOW("longNum::getValues()");

	return _values;
}

// -----------------------------------------------------------------------------------------------

bool longNum::isMalformed() const
{
	TRACE_CODE_FLOW("longNum::isMalformed()");

#if defined _IS_LESSER_

	if (_values && as_size_t() <= longNum_MAX_VALUE)
		return true;

	if (!_values && as_size_t() > longNum_MAX_VALUE)
		return true;

#endif

	if (_values && _length < longNum_MAX_SIZE_T_LENGTH)
		return true;

	if(_length == 0 && _sign != POS)
		return true;

	return false;
};

// -----------------------------------------------------------------------------------------------

void longNum::fill_maxSizeT()
{
#if defined _IS_LESSER_

	static bool isOk(false);

	if (!isOk)
	{
		size_t num(longNum_MAX_VALUE), i(0);
		while (num)
		{
			maxSizeT[i++] = num % BASE;
			num /= BASE;
		}

		isOk = true;
	}

#endif

	return;
}

// -----------------------------------------------------------------------------------------------

#endif

#if defined _TRACE_
  #undef _TRACE_
#endif

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

#define runTest(doRun, func) if(doRun) { func(); return 0; }

int testLesser(const long N)
{
#if !defined _IS_LESSER_

	runTest(1, testConstructor);
	runTest(0, testConvertToSizeT_ifPossible);
	runTest(0, testGet);
	runTest(0, testOperatorMinusMinus);
	runTest(0, testOperatorPlusPlus);
	runTest(0, testOperatorMinusEqualsTemplated);
	runTest(0, testOperatorPlusEqualsTemplated);
	runTest(0, testOperatorPlusEquals);
	runTest(0, testOperatorMinus);
	runTest(0, testOperatorMinusTemplated);
	runTest(0, testOperatorPlusTemplated);
	runTest(0, testAbsValueIsLarger);

#endif

	// zzz

	if(0)
	{
		// ERROR 4 in operator - : malformed longNum : -3333 - -2880

		long l1 = -3333;
		long l2 = -2380;

		longNum n1(l1), n2(l2);

		longNum asdasd = n1 - n2;

		std::cout << asdasd.get() << std::endl;

		std::cout << l1 - l2 << std::endl;

		if( asdasd.getValues())
			std::cout << "has values" << std::endl;

		if (asdasd.isMalformed())
			std::cout << "is malformed" << std::endl;

		return 0;

	}



	int percentage = 0;
	long maxValNeg = long(-longNum_MAX_VALUE);
	long maxValPos = long(+longNum_MAX_VALUE);

	auto getPercentage = [](const long N, const long i)
	{
		long MAX = 2 * N + 1;
		return (100 * (N + i)) / MAX;
	};

	// --------------------------------------------------------------------------
	// --------------------------------------------------------------------------

	std::cout << " --- " << percentage << "%" << std::endl;

	for (long i = -N; i <= N; i++)
	{
		longNum n0_i(i);

		// Allocation Check
		{
			bool fail(false);
			long ii(i < 0 ? -i : i);

			if (n0_i.getSign() != (i >= 0))
				fail = true;

			auto*  ptr = n0_i.getValues();
			size_t len = n0_i.getLength();

			if (ii <= longNum_MAX_VALUE)
			{
				if (ptr || len != ii)
					fail = true;
			}
			else
			{
				if (!ptr || len == ii)
					fail = true;
			}

			if (fail)
			{
				std::cout << " -- ERROR 1 in allocation check : " << i << std::endl;
				return 1;
			}
		}

		// --------------------------------------------------------------------------

		// Constructor(longNum &other)
		{
			longNum n1(i);
			longNum n2(n1);

			if (n2 != n1)
			{
				std::cout << " -- ERROR 1 in Constructor(longNum &other) : " << i << std::endl;
				return 1;
			}

			if (n2 != n0_i)
			{
				std::cout << " -- ERROR 2 in Constructor(longNum &other) : " << i << std::endl;
				return 1;
			}

			if (n1 != n0_i)
			{
				std::cout << " -- ERROR 3 in Constructor(longNum &other) : const values changed" << std::endl;
				return 1;
			}

			if (n1.isMalformed() || n2.isMalformed())
			{
				std::cout << " -- ERROR 4 in Constructor(longNum &other) : is Malformed " << i << std::endl;
				return 1;
			}
		}

		// --------------------------------------------------------------------------

		// operator =
		{
			longNum n1(i), n3;

			n3 = n1;

			if (n3 != n1)
			{
				std::cout << " -- ERROR 1 in operator = : " << i << std::endl;
				return 1;
			}

			if (n3.isMalformed())
			{
				std::cout << " -- ERROR 2 in operator = : is Malformed " << i << std::endl;
				return 1;
			}

			n3 = i;

			if (n3 != i)
			{
				std::cout << " -- ERROR 3 in operator = : " << i << std::endl;
				return 1;
			}

			if (n3.isMalformed())
			{
				std::cout << " -- ERROR 4 in operator = : is Malformed " << i << std::endl;
				return 1;
			}

			if (n1 != n0_i)
			{
				std::cout << " -- ERROR 5 in operator = : const values changed : " << i << std::endl;
				return 1;
			}

			if (n1.getLength() != n0_i.getLength() || n3.getLength() != n0_i.getLength())
			{
				std::cout << " -- ERROR 6 in operator = : wrong length : " << i << std::endl;
				return 1;
			}

			if (bool(n1.getValues()) != bool(n0_i.getValues()) || bool(n3.getValues()) != bool(n0_i.getValues()))
			{
				std::cout << " -- ERROR 7 in operator = : wrong values : " << i << std::endl;
				return 1;
			}
		}

		// --------------------------------------------------------------------------

		for (long j = -N; j <= N; j++)
		{
			// operator ==
			{
				longNum n1(i), n2(j);

				if ((i == j) != (n1 == n2))
				{
					std::cout << " -- ERROR 1 in operator == : " << i << " == " << j << std::endl;
					return 1;
				}

				if ((i == j) != (n1 == j))
				{
					std::cout << " -- ERROR 2 in operator == : " << i << " == " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator == : const values changed (" << i << " == " << j << ")" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator !=
			{
				longNum n1(i), n2(j);

				if ((i != j) != (n1 != n2))
				{
					std::cout << " -- ERROR 1 in operator != : " << i << " != " << j << std::endl;
					return 1;
				}

				if ((i != j) != (n1 != j))
				{
					std::cout << " -- ERROR 2 in operator != : " << i << " != " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator != : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator >
			{
				longNum n1(i), n2(j);

				if ((i > j) != (n1 > n2))
				{
					std::cout << " -- ERROR 1 in operator > : " << i << " > " << j << std::endl;
					return 1;
				}

				if ((i > j) != (n1 > j))
				{
					std::cout << " -- ERROR 2 in operator > : " << i << " > " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator > : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator >=
			{
				longNum n1(i), n2(j);

				if ((i >= j) != (n1 >= n2))
				{
					std::cout << " -- ERROR 1 in operator >= : " << i << " > " << j << std::endl;
					return 1;
				}

				if ((i >= j) != (n1 >= j))
				{
					std::cout << " -- ERROR 2 in operator >= : " << i << " > " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator >= : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator <
			{
				longNum n1(i), n2(j);

				if ((i < j) != (n1 < n2))
				{
					std::cout << " -- ERROR 1 in operator < : " << i << " < " << j << std::endl;
					return 1;
				}

				if ((i < j) != (n1 < j))
				{
					std::cout << " -- ERROR 2 in operator < : " << i << " < " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator < : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator <=
			{
				longNum n1(i), n2(j);

				if ((i <= j) != (n1 <= n2))
				{
					std::cout << " -- ERROR 1 in operator <= : " << i << " <= " << j << std::endl;
					return 1;
				}

				if ((i <= j) != (n1 <= j))
				{
					std::cout << " -- ERROR 2 in operator <= : " << i << " <= " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator <= : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator +
			{
				longNum n1(i), n2(j), n3(n1 + n2), n4;

				if (n3 != (i + j))
				{
					std::cout << " -- ERROR 1 in operator + : " << i << " + " << j << std::endl;
					return 1;
				}

				// Must skip the case where j is larger than max allowed value
				// In real world this case won't be a problem, as a [] is always greater than any built-in type
				if (j >= maxValNeg && j <= maxValPos)
				{
					n4 = n1 + j;

					if (!(n4 == (i + j)))
					{
						std::cout << " -- ERROR 2 in operator + : " << i << " + " << j << std::endl;
						return 1;
					}
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator + : const values changed" << std::endl;
					return 1;
				}

				if (n3.isMalformed() || n4.isMalformed())
				{
					std::cout << " -- ERROR 4 in operator + : malformed longNum : " << i << " + " << j << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator +=
			{
				longNum n1(i), n2(j);

				// operator +=
				n1 += n2;

				if (n1 != i + j)
				{
					std::cout << " -- ERROR 1 in operator += : " << i << " += " << j << " ::: Expected " << (i + j) << ", got " << n1.get() << std::endl;
					return 1;
				}

				if (n2 != j)
				{
					std::cout << " -- ERROR 2 in operator += : const values changed" << std::endl;
					return 1;
				}

				// operator += -- move semantics
				n2 += longNum(i);

				if (n2 != (i + j))
				{
					std::cout << " -- ERROR 3 in operator += : " << j << " += " << i << " ::: Expected " << (i + j) << ", got " << n2.get() << std::endl;
					return 1;
				}

				if (n1.isMalformed() || n2.isMalformed())
				{
					std::cout << " -- ERROR 4 in operator += : malformed longNum : " << i << " += " << j << std::endl;
					return 1;
				}

				// Must skip the case where i or j is larger than max allowed value
				// In real world this case won't be a problem, as a [] is always greater than any built-in type
				if (i >= maxValNeg && i <= maxValPos && j >= maxValNeg && j <= maxValPos)
				{
					// operator += -- templated
					n1 += i;
					n1 += j;

					if (n1 != i + i + j + j)
					{
						std::cout << " -- ERROR 5 in operator += " << i << ", " << j << std::endl;
						return 1;
					}
				}

				if (n1.isMalformed())
				{
					std::cout << " -- ERROR 6 in operator += : malformed longNum" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator -
			{
				longNum n1(i), n2(j), n3(n1 - n2), n4;

				if (n3 != i - j)
				{
					std::cout << " -- ERROR 1 in operator - : " << i << " - " << j << " ::: Expected " << (i - j) << ", got " << n3.get() << std::endl;
					return 1;
				}

				// Must skip the case where j is larger than max allowed value
				// In real world this case won't be a problem, as a [] is always greater than any built-in type
				if (j >= maxValNeg && j <= maxValPos)
				{
					n4 = n1 - j;

					if (!(n4 == (i - j)))
					{
						std::cout << " -- ERROR 2 in operator - : " << i << " - " << j << std::endl;
						return 1;
					}
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 3 in operator - : const values changed" << std::endl;
					return 1;
				}

				if (n3.isMalformed() || n4.isMalformed())
				{
					std::cout << " -- ERROR 4 in operator - : malformed longNum : " << i << " - " << j << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator -=
			{

				longNum n1(i), n2(j);

				// operator -=
				n1 -= n2;

				if (n1 != i - j)
				{
					std::cout << " -- ERROR 1 in operator -= : " << i << " -= " << j << " ::: Expected " << (i - j) << ", got " << n1.get() << std::endl;
					return 1;
				}

				if (n2 != j)
				{
					std::cout << " -- ERROR 2 in operator -= : const values changed" << std::endl;
					return 1;
				}

				// operator -= -- move semantics
				n2 -= longNum(i);

				if (n2 != j - i)
				{
					std::cout << " -- ERROR 3 in operator -= " << std::endl;
					return 1;
				}

				if (n1.isMalformed() || n2.isMalformed())
				{
					std::cout << " -- ERROR 4 in operator += : malformed longNum : " << i << " += " << j << std::endl;
					return 1;
				}

				// Must skip the case where i or j is larger than max allowed value
				// In real world this case won't be a problem, as a [] is always greater than any built-in type
				if (i >= maxValNeg && i <= maxValPos && j >= maxValNeg && j <= maxValPos)
				{
					// operator -= -- templated
					n1 -= i;
					n1 -= j;

					if (n1 != i - i - j - j)
					{
						std::cout << " -- ERROR 5 in operator -= : " << i << " -= " << j << " ::: Expected " << (i - j) << ", got " << n1.get() << std::endl;
						return 1;
					}
				}

				if (n1.isMalformed())
				{
					std::cout << " -- ERROR 6 in operator -= : malformed longNum" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator ++()
			{
				longNum n1(i);

				++n1;

				if (n1 != i + 1)
				{
					std::cout << " -- ERROR 1 in operator ++() : " << i << "++ != " << i + 1 << " ( is " << n1.get() << ")" << std::endl;
					return 1;
				}

				if (n1.isMalformed())
				{
					std::cout << " -- ERROR 2 in operator ++() : longNum(" << i << ")++ is malformed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------

			// operator --()
			{

			}

			// --------------------------------------------------------------------------

			// operator bool()
			{
				longNum n1(i), n2(j);

				if ((n1 && n2) != (i && j) || (n1 || n2) != (i || j))
				{
					std::cout << " -- ERROR 1 in operator bool() : " << i << " bool " << j << std::endl;
					return 1;
				}

				if (!n1 != !i)
				{
					std::cout << " -- ERROR 2 in operator bool() : " << i << " bool " << j << std::endl;
					return 1;
				}

				if (!n2 != !j)
				{
					std::cout << " -- ERROR 3 in operator bool() : " << i << " bool " << j << std::endl;
					return 1;
				}

				if (n1 != i || n2 != j)
				{
					std::cout << " -- ERROR 4 in operator bool() : const values changed" << std::endl;
					return 1;
				}
			}

			// --------------------------------------------------------------------------
/*
			// std::string get() const
			{
				longNum n1(i), n2(j);

				if (std::to_string(i) != n1.get() || std::to_string(j) != n2.get())
				{
					std::cout << " -- ERROR 1 in std::string get()" << std::endl;
					return 1;
				}
			}
*/
			// --------------------------------------------------------------------------
		}

		int prct = getPercentage(N, i);
		if (prct != percentage && (prct % 10 == 0 || prct % 10 == 5))
		{
			percentage = prct;
			std::cout << " --- " << percentage << "%" << std::endl;
		}
	}

	std::cout << " --- 100%" << std::endl;

	return 0;
}

// -----------------------------------------------------------------------------------------------

void testOperatorMinus()
{
	longNum res,
		n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
		n1("53285760128475643665508346502"),
		n2("18446744083709551615"),
		n3(123456789);

	long long minval = -(size_t(-1) / 2);
	long long maxval = +(size_t(-1) / 2) - 293456789813;

#if 0
	size_t num1 = 293456789813;
	size_t num2 = 293456789813;

	// 54.7 -- 32.1
	for (size_t i = 0; i < 39999999; ++i)
	{
		std::string str1 = std::to_string(num1);
		std::string str2 = std::to_string(num2);

		longNum ln1(str1 + str2);
		longNum ln2(str2 + str1 + str2);

		res = n0 - ln1;		// 32.8 -- 22.14
		res = n0 - ln2;

		res = n1 - ln1;		// 24.8 -- 19.47
		res = n1 - ln2;

		res = n2 - ln1;		// 24.8 -- 19.16
		res = n2 - ln2;

		num1 *= 17;
		num2 *= 27;
	}
	
#if 0
	if( res == "40888887866675555555333342222222000008888888666755555553333334222222200000000888888866667551428799901912146039290692758869354128635924167316120260458498189188")
		std::cout << " ------------ ok -------------" << std::endl;
#endif
	//std::cout << res.get() << std::endl;

#endif

#if 1
	size_t num1 = 293456789813;
	size_t num2 = 293456789813;

	// 23.183 -- 17.343 -- 16.896
	for (size_t i = 0; i < 9999999; ++i)
	{
		std::string str1 = std::to_string(num1);
		std::string str2 = std::to_string(num2);

		str1 = str1 + str1 + str1 + str1 + str1;
		str2 = str2 + str2 + str2 + str2 + str2;

		longNum ln1(str1);
		longNum ln2(str2);

		res = ln1 - ln2;
		res = ln2 - ln1;

		str2 = str2 + str2 + str2;

		longNum ln3(str2);

		res = ln3 - ln2;
		res = ln3 - ln1;

		res = ln2 - ln3;
		res = ln1 - ln3;

		num1 *= 17;
		num2 *= 27;
	}

#endif

#if 0
	{
		longNum nnn("99999999999999999999999999");

		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		res = n0;

		// 17.534  -- 17.22 -- 7.198
		for (size_t i = 0; i < 49999999; ++i)
		{
			longNum ln1(num1);
			longNum ln2(num1);

			res = res + ln1;
			res = res + ln2;

			num1 *= 17;
			num1 *= 27;
		}

		std::cout << res.get() << std::endl;
	}
#endif

#if 0
	{
		// size_t += size_t -- both signs
		size_t num1 = 293456789813;
		size_t num2 = 293456789817;

		// 38.705 -- 36.6 -- 33.053 -- 32.172
		for (size_t i = 0; i < 999999999; ++i)
		{
			longNum ln1(num1);
			longNum ln2(num2);

			res = ln1 + ln2;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}

#endif
	return;
}

void testOperatorPlusTemplated()
{
	longNum res,
		n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
		n1("53285760128475643665508346502"),
		n2("18446744083709551615"),
		n3(123456789);

	long long minval = -(size_t(-1) / 2);
	long long maxval = +(size_t(-1) / 2);

	if (1)
	{
		size_t N = 99999999;

		long long incr = maxval / N;
		long long num = 0;

		for(size_t i = 0; i < N; ++i)
		{
			longNum nnn(num);

			res = (n0 - nnn);
			res = (n1 - nnn);
			res = (n2 - nnn);

			num += incr;
		}

		//std::cout << res.get() << std::endl;

		return;
	}

	for (long long i = minval; i < maxval; i += 293456789813)
	{
		res = (n0 + i);
		res = (n1 + i);
		res = (n2 + i);
		res = (n3 + i);
	}

	size_t incr(223456789871);

	for (size_t i = size_t(-1); i > incr; i -= incr)
	{
		res = (n0 + i);
		res = (n1 + i);
		res = (n2 + i);
		res = (n3 + i);
	}

	return;
}

void testOperatorMinusTemplated()
{
	longNum res,
		n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
		n1("53285760128475643665508346502"),
		n2("18446744083709551615"),
		n3(123456789);

	long long minval = -(size_t(-1) / 2);
	long long maxval = +(size_t(-1) / 2) - 293456789813;

	for (long long i = minval; i < maxval; i += 293456789813)
	{
		res = (n0 - i);
		res = (n1 - i);
		res = (n2 - i);
		res = (n3 - i);
	}

	size_t incr(223456789871);

	for (size_t i = size_t(-1); i > incr; i -= incr)
	{
		res = (n0 - i);
		res = (n1 - i);
		res = (n2 - i);
		res = (n3 - i);
	}

	return;
}

void testOperatorPlusEquals()
{
	int mode = 5;

	longNum res, n2("18446744083709551615");

	// [] += [] -- same sign
	if (mode == 1)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		res = n2;

		// 47.846 -- 27.614
		for (size_t i = 0; i < 19999999; ++i)
		{
			std::string str1 = std::to_string(num1);
			std::string str2 = std::to_string(num2);

			str1 = str1 + str1 + str1 + str1 + str1;
			str2 = str2 + str2 + str2 + str2 + str2;

			longNum ln1(str1);
			longNum ln2(str2);

			str2 = str2 + str2 + str2;
			longNum ln3(str2);

			res += ln1;
			res += ln2;
			res += ln3;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}

	// [] += [] -- diff sign
	if (mode == 2)
	{
		longNum n0("-511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
				n1("-511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");

		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 31.833 -- 22.046 -- 20.794

		for (size_t i = 0; i < 19999999; ++i)
		{
			std::string str1 = std::to_string(num1);
			std::string str2 = std::to_string(num2);

			str1 = str1 + str1 + str1 + str1 + str1;
			str2 = str2 + str2 + str2 + str2 + str2;

			longNum ln1(str1);
			longNum ln2(str2);

			n0 += ln1;
			ln2 += n1;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}

	// [] += size_t -- both signs
	if (mode == 3)
	{
		longNum n0("-511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
				n1("+511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");

		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 34.123 -- 21.922 -- 17.89 -- 13.625 -- 5.354

		for (size_t i = 0; i < 59999999; ++i)
		{
			longNum ln1(num1);
			longNum ln2(num2);

			n0 += ln1;
			n1 += ln2;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}

	// size_t += [] -- both signs
	if (mode == 4)
	{
		longNum n0("-511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
				n1("+511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");

		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 48.47 -- 25.519 -- 23.994 -- 23.64

		for (size_t i = 0; i < 59999999; ++i)
		{
			longNum ln1(num1);
			longNum ln2(num2);

			ln1 += n0;
			ln2 += n1;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}

	// size_t += size_t -- both signs
	if (mode == 5)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 45.051 -- 39.291

		for (size_t i = 0; i < 999999999; ++i)
		{
			longNum ln1(num1);
			longNum ln2(num2);
			longNum ln3(num1);
			longNum ln4(num2); ln4.flipSign();

			ln1 += ln2;
			ln3 += ln4;

			num1 *= 17;
			num2 *= 27;
		}

		return;
	}
}

void testOperatorPlusEqualsTemplated()
{
	longNum n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
			n1("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");

	long long incr = 193456789813;
	long long minval = -(size_t(-1) / 2);
	long long maxval = +(size_t(-1) / 2) - incr;

	size_t num1 = 293456789813;
	size_t num2 = 293456789813;

	// 30.774 -- 5.832 -- 5.143

	for (long long i = 0; i < maxval; i += incr)
	{
		n0 += +i;
		n1 += -i;

		longNum ln1(num1);
		longNum ln2(num2);

		ln1 += +i;
		ln2 += -i;

		num1 *= 17;
		num2 *= 27;
	}

	return;
}

void testOperatorMinusEquals()
{
	// opMinusEqual_1
	if (1)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 11.856 -- only base
		// 53.97  /  74.838
		for (size_t i = 0; i < 29999999; ++i)
		{
			std::string str1 = std::to_string(num1);
			std::string str2 = std::to_string(num2);

			str1 = str1 + str1 + str1 + str1 + str1;
			str2 = str2 + str2 + str2 + str2 + str2;

			std::string str3 = str2 + str2 + str2;
#if 1
			longNum ln1(str1);
			longNum ln2(str2);
			longNum ln3(str3);

			ln3 -= ln1;
			ln2 -= ln3;
			ln2 -= ln1;
			ln1.flipSign();
			ln1 -= ln3;
#endif
			num1 *= 17;
			num2 *= 27;
		}
	}

	// opMinusEqual_2
	if (0)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;
		size_t num3 = 293456789813;

		// 9.597 -- only base
		// 19.14  /  27.882
		for (size_t i = 0; i < 29999999; ++i)
		{
			std::string str1 = std::to_string(num1);
			std::string str2 = std::to_string(num2);

			str1 = str1 + str1 + str1 + str1 + str1;
			str2 = str2 + str2 + str2 + str2 + str2;
#if 1
			longNum ln1(str1);
			longNum ln2(str2);
			longNum ln3(num3);

			ln1 -= ln3;
			ln3.flipSign();
			ln2 -= ln3;
#endif
			num1 *= 17;
			num2 *= 27;
			num3 *= 43;
		}
	}

	// opMinusEqual_3
	if (0)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 4.9 -- only base
		// 17.3  /  19.73
		for (size_t i = 0; i < 29999999; ++i)
		{
			std::string str1 = std::to_string(num1);
			str1 = str1 + str1 + str1 + str1 + str1;
#if 1
			longNum ln1(str1);
			longNum ln2(num2);
			longNum ln3(num2);
			ln3.flipSign();

			ln2 -= ln1;
			ln3 -= ln1;
#endif
			num1 *= 17;
			num2 *= 27;
		}
	}

	// opMinusEqual_4
	if (0)
	{
		size_t num1 = 293456789813;
		size_t num2 = 293456789813;

		// 1.428  /  1.628
		for (size_t i = 0; i < 29999999; ++i)
		{
#if 1
			longNum ln1(num1);
			longNum ln2(num1);
			longNum ln3(num2);
			longNum ln4(num2);	ln4.flipSign();

			ln1 -= ln3;
			ln2 -= ln4;
#endif
			num1 *= 17;
			num2 *= 27;
		}
	}

	return;
}

void testOperatorMinusEqualsTemplated()
{
	longNum n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");
	longNum n1("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");
	longNum res;

	size_t num = 499999999;

	// longNum_long -= size_t
	// new ver is faster
	if (1)
	{
		//n0.flipSign();

		// n0 > 0: 20.901  /  19.914
		// n0 < 0: 15.8    /  14.96

		size_t type(0);
		size_t incr = (size_t(-1) / num);

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type += incr;
		}

		return;
	}

	// longNum_short -= size_t
	// new ver is faster
	if (0)
	{
		size_t type(0);
		size_t incr = (size_t(-1) / num);

		for (size_t i = 0; i < 10u * (num + 1); i++)
		{
			longNum n2(size_t(-1));					// 19.04  /  12.651
//			longNum n2(987654321);					// 19.04  /  12.684

//			longNum n2(-1);							// 18.94  /  12.69
//			n2.flipSign();

//			longNum n2(987654321);					// 17.4   /  12.6
//			n2.flipSign();

			n2 -= type;
			type += incr;
		}

		return;
	}

	// longNum_long -= long long
	if (0)
	{
		long long maxval = size_t(-1) / 2;
		long long type(0);
		long long incr = maxval / num;

		//n0.flipSign();

		// 20.642  / 19.605

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type += incr;							// 20.788  /  22.019
			//type -= incr;							// 15.789  /  18.445

			// n0 < 0, type > 0:					// 15.632  /  18.806
			// n0 < 0, type < 0:					// 20.091  /  21.997
		}

		return;
	}

	// longNum_short -= long long
	if (0)
	{
		long long maxval = size_t(-1) / 2;
		long long type(0);
		long long incr = maxval / num;

		for (size_t i = 0; i < num + 1; i++)
		{
			longNum n0(maxval);

			n0 -= type;
			type += incr;							// 2.183  /  1.37
			//type -= incr;							// 15.789  /  18.445

			// n0 < 0, type > 0:					// 15.632  /  18.806
			// n0 < 0, type < 0:					// 20.091  /  21.997
		}

		return;
	}

	// long > 0
	if (0)
	{
		long maxval = 2147483647;
		long type(0);
		long incr = maxval / long(num);

		// old 11.8 -- new 12.0

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type += incr;
		}

		return;
	}

	// long < 0
	if (0)
	{
		long maxval = 2147483647;
		long type(0);
		long incr = maxval / long(num);

		// old 9.42 -- new 8.71

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type -= incr;
		}

		return;
	}

	// int > 0
	if (0)
	{
		int maxval = 2147483647;
		int type(0);
		int incr = maxval / int(num);

		// old 11.781 -- new 12.157

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type += incr;
		}

		return;
	}

	// int < 0
	if (0)
	{
		int maxval = 2147483647;
		int type(0);
		int incr = maxval / int(num);

		// old 8.386 -- new 9.412

		for (size_t i = 0; i < num + 1; i++)
		{
			n0 -= type;
			type -= incr;
		}

		return;
	}

	return;
}

void testOperatorPlusPlus()
{
	// -size_t to +size_t
	if(0)
	{
		longNum n(4999999999);
		n.flipSign();

		// 18.073 -- 15.561
		for (size_t i = 0; i < 9999999999; ++i)
		{
			++n;
		}

		return;
	}

	// [] to []
	if (1)
	{
		longNum n("-998446744073709551717");

		// n > 0: 16.011 -- 15.150
		// n < 0: 17.535 -- 16.245
		for (size_t i = 0; i < 9999999999; ++i)
		{
			++n;
		}

		return;
	}

	return;
}

void testOperatorMinusMinus()
{
#if 0

	longNum nnn(size_t(-3));
	nnn.flipSign();

	for (int i = 0; i < 6; i++)
	{
		std::cout << nnn.get();
		--nnn;
		std::cout << " ---> " << nnn.get() << std::endl;
	}

	return;

#endif

	if (1)
	{
		//longNum nn(-1);
		longNum nn("998446744073709551717");

		// 
		for (size_t i = 0; i < 9999999999; ++i)
		{
			--nn;
		}

		return;

		longNum n(4999999999);

		// 
		for (size_t i = 0; i < 9999999999; ++i)
		{
			--n;
		}

		return;
	}
}

void testGet()
{
	//longNum nn(size_t(-3));
	longNum nn("998446744073709551717");

	//nn.flipSign();

	std::cout << " ---> '" << nn.get() << "'" << std::endl;
	//return;

	// 13.481 -- 

	for (size_t i = 0; i < 299999999; ++i)
	{
		nn.get();
	}
}

void testAbsValueIsLarger()
{
#if 0
#if 0
	longNum nn1("18446744083709551615");
	longNum nn2("18446744083709551615");

	int aaa = nn1.abs(nn2);

	std::cout << std::endl;
	std::cout << " res = " << aaa << std::endl;

	return;
#endif

	longNum
		n0("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502"),
		n1("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346503"),
		n2("53285760128475643665508346502"),
		n3("18446744083709551615");

	size_t res = 0;

	// 36.25 -- 29.69
	// 41.5 -- 8.3
	for (size_t i = 0; i < 299999999; ++i)
	{
		size_t len = n0.getLength();

		n0.getValues()[1] = 6;
		n1.getValues()[1] = 6;

		res += n0.abs(n1);

		res += n1.abs(n2);

		res += n2.abs(n1);

		res += n2.abs(n3);

		res += n0.abs(n0);

	}

	std::cout << res << std::endl;
#endif
}

void testConvertToSizeT_ifPossible()
{
	longNum res, n1("18446744073709551616"), n2("38446744073709551616"), n3("78446744073709551616");

/*
	res = n1;
	res.getValues()[0] = 3;
	res.aaa();
	std::cout << res.get() << std::endl;
	return;
*/

	// 46.128 -- 36.15
	// 26.571 -- 24.7
	//for (size_t i = 1; i < 999999999; ++i)
	for (size_t i = 1; i < 599999999; ++i)
	{
		res = n1;
		res.getValues()[0] = 3;
		res.aaa();

		res = n2;
		res.getValues()[0] = 3;
		res.aaa();

		res = n3;
		res.getValues()[0] = 3;
		res.aaa();
	}
}

void testConstructor()
{
	longNum n1(-1);
	longNum n2(+1);
	longNum n3(+2);
	longNum n4("18446744073709551619");
	longNum n5("184467440737095516193");
	longNum n6("184467440737095516197");

	longNum n7("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502");
	longNum n8("511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346503");

	size_t bbb(0);

	// 34.358 -- 
	for (volatile size_t i = 1; i < 199999999; ++i)
	{
		bbb += n1 <= n2;
		bbb += n2 <= n1;

		bbb += n2 <= n3;
		bbb += n3 <= n2;
		bbb += n4 <= n5;
		bbb += n5 <= n4;
		bbb += n5 <= n6;
		bbb += n6 <= n5;
		bbb += n7 <= n8;
		bbb += n8 <= n7;
	}

	std::cout << bbb;

	return;

#if 0
	longNum n1("18446744073709551616");
	std::cout << n1.get() << std::endl;

	if (n1 == "18446744073709551616")
	{
		std::cout << std::endl;
		std::cout << "-= OK =-" << std::endl;
		std::cout << std::endl;
	}

	return;
#endif

	const char* str1 = "+511111111111222222222222333333333333444444444445555555555555566666666666666677777777777788888888888899999999990000000000003285760128475643665508346502";
	const char* str2 = "18446744073709551619";
	const char* str3 = "18446744073709551613";
	const char* str4 = "38446744073709551613";
	const char* str5 = "1844674407370955161";

//	longNum nnn1("18446744073709551619");
//	return;

	// nnn1,	199999999	:: 31.811 -- 28.479 -- 25.474 -- 25.3 -- 25.156
	// nnn1-4,	99999999	:: 47.435 -- 47.173 -- 46.7 -- 25.97 -- 25.462 -- 24.906 -- 24.8
	for (size_t i = 1; i < 99999999; ++i)
	{
		longNum nnn1(str1);
		longNum nnn2(str2);
		longNum nnn3(str3);
		longNum nnn4(str4);
		longNum nnn5(str5);
	}
}


