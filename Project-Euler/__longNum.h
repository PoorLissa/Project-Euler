#pragma once

#include <string>

// -----------------------------------------------------------------------------------------------

class longNum {

		typedef short digitType;
		static const digitType BASE = 10u;

	public:

		longNum() : _values(nullptr), _length(0) { ; }
	   ~longNum();


		// Constructors
		longNum(const char *);
		longNum(const std::string& str) : longNum(str.c_str()) { ; }
		longNum(longNum  &);
		longNum(longNum &&) noexcept;
		longNum(size_t);


		// Operators
		longNum &	operator = (longNum  &);
		longNum &	operator = (longNum &&) noexcept;
		longNum		operator + (const longNum &);
		longNum		operator + (size_t);
		longNum &	operator +=(longNum &);
		longNum &	operator +=(longNum &&) noexcept;
		longNum &	operator +=(size_t);


		// Getters
		std::string get() const;

	private:

		digitType*	_values;
		size_t		_length;

	public:
		static size_t cnt;
};

size_t longNum::cnt = 0;

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

longNum::longNum(const char *str) : _length(strlen(str))
{
#if defined _DEBUG
	std::cout << " ---> Alloc" << std::endl;
#endif

	_values = new digitType[_length];

	for (size_t i = 0; i < _length; i++)
	{
		_values[_length - i - 1] = (str[i] - 48);
	}
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(longNum& other) : _length(other._length)
{
#if defined _DEBUG
	std::cout << " ---> Alloc" << std::endl;
#endif

	_values = new digitType[_length];
	memcpy(_values, other._values, sizeof(digitType) * _length);
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(longNum&& other) noexcept
{
	_length = std::move(other._length);
	_values = std::move(other._values);

	other._length = 0;
	other._values = nullptr;
}

// -----------------------------------------------------------------------------------------------

longNum::longNum(size_t num) : _length(0u)
{
	size_t i = 0, tmp = num;

	while (tmp && ++_length)
		tmp /= BASE;

#if defined _DEBUG
	std::cout << " ---> Alloc" << std::endl;
#endif

	_values = new digitType[_length];

	while (num)
	{
		digitType digit = num % BASE;
		_values[i++] = digit;
		num /= BASE;
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

longNum longNum::operator +(const longNum &other)
{
	longNum res;

	// The longest number will be the first
	const longNum* p1(this);
	const longNum* p2(&other);

	if (_length < other._length)
	{
		p1 = &other;
		p2 = this;
	}

#if defined _DEBUG
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

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

longNum longNum::operator +(size_t other)
{
	return std::move(*this + longNum(other));
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(longNum &other)
{
	// There's a chance we'll be able to add the 2 numbers without reallocation

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
			#if defined _DEBUG
				std::cout << " ---> Alloc" << std::endl;
			#endif

			digitType *newVal = new digitType[_length + 1u];

			memcpy(newVal, _values, sizeof(digitType) * _length);

			newVal[_length++] = carryOver;

			delete[] _values;
			_values = newVal;
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

		_length = other._length;
		_values = std::move(other._values);

		other._length = tmpLength;
		other._values = std::move(tmpValues);
	}

	*this += other;

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator +=(size_t other)
{
	*this += longNum(other);

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(longNum &other)
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

		if(!_values)
		{
			#if defined _DEBUG
				std::cout << " ---> Alloc" << std::endl;
			#endif

			_values = new digitType[_length];
		}

		memcpy(_values, other._values, sizeof(digitType)*_length);
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

longNum& longNum::operator =(longNum && other) noexcept
{
	if (this != &other)
	{
		if (_values)
			delete[] _values;

		// Steal the data
		_length = std::move(other._length);
		_values = std::move(other._values);

		// Leave the other object in empty state
		other._length = 0;
		other._values = nullptr;
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

// Return number as a string in a normal readable order
std::string longNum::get() const
{
	std::string str;

	if (_values)
	{
		str.reserve(_length);

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
