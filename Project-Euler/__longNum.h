#pragma once

// Fast copying via the use of hardware
// Mb check this out later
// https://codereview.stackexchange.com/questions/5520/copying-80-bytes-as-fast-as-possible

// Karatsuba multiplication
// https://habr.com/ru/post/124258/

#include <string>

// -----------------------------------------------------------------------------------------------

#pragma warning (disable:4146)		// Search for 'pragma warning (disable:4146)' to find code parts that use this pragma
#pragma warning (disable:4018)

// -----------------------------------------------------------------------------------------------

//#define _TRACE_
//#define _IS_LESSER_							// Needed only for testing

#if !defined _IS_LESSER_

  #define longNum_MAX_SIZE_T_LENGTH				20u
  #define longNum_MAX_SIZE_T_LENGTH_PLUS_ONE	21u
  #define longNum_MAX_SIZE_T_ORDER				1e19
  #define longNum_MAX_VALUE						size_t(-1)

#else

  #pragma warning (disable:4244)
  #pragma warning (disable:4267)
/*
  #define longNum_MAX_SIZE_T_LENGTH				4u
  #define longNum_MAX_SIZE_T_LENGTH_PLUS_ONE	5u
  #define longNum_MAX_SIZE_T_ORDER				1e4
  #define longNum_MAX_VALUE						1234		// max value x 2 should have the same number of digits (123 x 2 = 246 -- ok, 501 x 2 = 1002 -- fail!)
*/

  constexpr size_t longNum_MAX_SIZE_T_LENGTH			= 3u;
  constexpr size_t longNum_MAX_SIZE_T_LENGTH_PLUS_ONE	= longNum_MAX_SIZE_T_LENGTH + 1u;
  constexpr size_t longNum_MAX_SIZE_T_ORDER				= 1e3;
  constexpr size_t longNum_MAX_VALUE					= 452;		// max value x 2 should have the same number of digits (123 x 2 = 246 -- ok, 501 x 2 = 1002 -- fail!)

#endif

// -----------------------------------------------------------------------------------------------

constexpr bool POS = true;
constexpr bool NEG = false;

// -----------------------------------------------------------------------------------------------

void	aaa();
int		testLesser(const long);
void	testOperatorMinus();
void	testOperatorMinusTemplated();
void	testOperatorPlusTemplated();
void	testOperatorPlusEquals();
void	testOperatorPlusEqualsTemplated();
void	testOperatorMinusEquals();

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
		longNum(const long long);
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
		std::string		get()		const;
		size_t			as_size_t() const;
		size_t			getLength() const;
		digitType*		getValues() const;
		bool			getSign()	const;


		// Other
		void flipSign();
		bool isMalformed() const;

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
		void opPlusEqual_2(longNum&, const longNum&, const size_t) const;							// operator += helper 2
		void opPlusEqual_3(longNum&, const longNum&, const size_t) const;							// operator += helper 3
		void opPlusEqual_4(longNum&, const longNum&) const;											// operator += helper 4

		void opMinusEqual_1(longNum&, const longNum&) const;										// operator -= helper 1
		void opMinusEqual_2(longNum&, const longNum&, const size_t) const;							// operator -= helper 2
		void opMinusEqual_3(longNum&, const longNum&, const size_t) const;							// operator -= helper 3
		void opMinusEqual_4(longNum&, const longNum&) const;										// operator -= helper 4

	private:
		// Do NOT change order! All the constructors rely on the definition order
		bool		_sign;
		size_t		_length;
		digitType* _values;
};

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
