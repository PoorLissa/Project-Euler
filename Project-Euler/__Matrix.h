#pragma once

#include <iostream>
#include <initializer_list>

// -----------------------------------------------------------------------------------------------

template <class Type>
class myMatrix {

	public:

		myMatrix(size_t, size_t, Type = 0);
		myMatrix(size_t, size_t, Type*);
		myMatrix(size_t, size_t, std::initializer_list<Type>);
		myMatrix(const myMatrix &);
		myMatrix(	   myMatrix &&) noexcept;
		~myMatrix();

		myMatrix&	operator = (const myMatrix & );
		myMatrix&	operator = (	  myMatrix &&) noexcept;

		myMatrix	operator + (const myMatrix &) const;
		myMatrix	operator - (const myMatrix &) const;
		myMatrix	operator * (const myMatrix &) const;
		bool		operator ==(const myMatrix &) const;
		bool		operator !=(const myMatrix &) const;

		Type& at(size_t, size_t) const;

		template <class NUMERIC> myMatrix	operator * (const NUMERIC) const;
		template <class NUMERIC> myMatrix&	operator *=(const NUMERIC);

		void print() const;

	private:
		size_t	_Cols, _Rows;

		std::unique_ptr<Type[]> _data;
};

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>::myMatrix(size_t rows, size_t columns, Type val) : _Cols(columns), _Rows(rows)
{
	_data = std::make_unique<Type[]>(_Cols * _Rows);

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = val;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>::myMatrix(const myMatrix<Type> &other) : _Cols(other._Cols), _Rows(other._Rows)
{
	_data = std::make_unique<Type[]>(_Cols * _Rows);

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = other._data[i];
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>::myMatrix(size_t rows, size_t columns, Type *data) : _Cols(columns), _Rows(rows), _data(nullptr)
{
	_data = std::make_unique<Type[]>(_Cols * _Rows);

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = *data++;
}

// -----------------------------------------------------------------------------------------------

// Example: myMatrix<float> m(2, 3, {1, 2, 3, 4, 5, 6});
template <class Type>
myMatrix<Type>::myMatrix(size_t rows, size_t columns, std::initializer_list<Type> args) : _Cols(columns), _Rows(rows), _data(nullptr)
{
	if (args.size() != _Cols * _Rows)
		throw std::invalid_argument("Matrix size does not match the size of initializer list");

	_data = std::make_unique<Type[]>(_Cols * _Rows);

	Type* dataIter = _data.get();

	for (auto iter = args.begin(); iter != args.end(); ++iter)
		*dataIter++ = *iter;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>::myMatrix(myMatrix<Type> && other) noexcept
{
	_Cols = other._Cols;
	_Rows = other._Rows;
	_data  = std::move(other._data);

	other._Cols = 0u;
	other._Rows = 0u;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>::~myMatrix()
{
	_Cols = 0u;
	_Rows = 0u;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
void myMatrix<Type>::print() const
{
	size_t cnt(0u);

	for (size_t i = 0; i < _Cols * _Rows; i++)
	{
		std::cout << " " << _data[i] << " ";

		if (++cnt == _Cols)
		{
			std::cout << std::endl;
			cnt = 0u;
		}
	}
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>& myMatrix<Type>::operator = (const myMatrix<Type>& other)
{
	if (this != &other)
	{
		_Cols = other._Cols;
		_Rows = other._Rows;

		_data = std::make_unique<Type[]>(_Cols * _Rows);

		for (size_t i = 0u; i < _Cols * _Rows; i++)
			_data[i] = other._data[i];
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type>& myMatrix<Type>::operator = (myMatrix<Type>&& other) noexcept
{
	if (this != &other)
	{
		// Steal the data
		_Rows = std::move(other._Rows);
		_Cols = std::move(other._Cols);
		_data = std::move(other._data);

		// Leave the other object in empty state
		other._Rows = 0u;
		other._Cols = 0u;
	}

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type> myMatrix<Type>::operator + (const myMatrix<Type> &other) const
{
	myMatrix<Type> res(_Rows, _Cols);

	if (_Cols == other._Cols && _Rows == other._Rows)
		for (size_t i = 0; i < _Cols * _Rows; i++)
			res._data[i] = _data[i] + other._data[i];

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
myMatrix<Type> myMatrix<Type>::operator - (const myMatrix<Type> &other) const
{
	myMatrix<T> res(_Rows, _Cols);

	if (_Cols == other._Cols && _Rows == other._Rows)
		for (size_t i = 0; i < _Cols * _Rows; i++)
			res._data[i] = _data[i] - other._data[i];

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
template <class NUMERIC>
myMatrix<Type> myMatrix<Type>::operator * (const NUMERIC n) const
{
	myMatrix<Type> res(_Rows, _Cols);

	for (size_t i = 0; i < _Cols * _Rows; i++)
		res._data[i] = static_cast<T>(_data[i] * n);

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

template <class Type>
template <class NUMERIC>
myMatrix<Type>& myMatrix<Type>::operator *=(const NUMERIC n)
{
	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] *= n;

	return *this;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool myMatrix<Type>::operator ==(const myMatrix<Type>& other) const
{
	if (_Cols != other._Cols || _Rows != other._Rows)
		return false;

	for (size_t i = 0u; i < _Rows * _Cols; i++)
		if (_data[i] != other._data[i])
			return false;

	return true;
}

// -----------------------------------------------------------------------------------------------

template <class Type>
bool myMatrix<Type>::operator !=(const myMatrix<Type>& other) const
{
	return !(*this == other);
}

// -----------------------------------------------------------------------------------------------

// Matrix multiplication [A x B] is possible only when A._Cols == B._Rows
// Resulting matrix size will be [B._Cols x A._Rows]
template <class Type>
myMatrix<Type> myMatrix<Type>::operator * (const myMatrix<Type>& other) const
{
	if(_Cols != other._Rows)
		throw "Incompatible matrices";

//	std::cout << " A[" <<	    _Rows << " x " <<	    _Cols << "]" << std::endl;
//	std::cout << " B[" << other._Rows << " x " << other._Cols << "]" << std::endl;

	myMatrix<Type> res(_Rows, other._Cols, 0.0f);

	Type* m1 = _data.get();
	Type* m2 = other._data.get();
	Type* m3 = res._data.get();

	for (size_t i = 0; i < res._Rows; i++)
	{
		for (size_t j = 0; j < res._Cols; j++)
		{
			Type* row_i = m1 + _Cols * i;
			Type* col_j = m2 + j;

			for (size_t k = 0; k < _Cols; k++)
			{
				*m3 += (*row_i * *col_j);

				row_i++;
				col_j += other._Cols;
			}

			m3++;
		}
	}

//	std::cout << " C[" << res._Rows << " x " << res._Cols << "]" << std::endl;

	return std::move(res);
}

// -----------------------------------------------------------------------------------------------

// Accesses the matrix element at [ i ][ j ], (i.e. at i-th Row, j-th Column)
template <class Type>
Type& myMatrix<Type>::at(size_t i, size_t j) const
{
	return *(_data.get() + i * _Cols + j);
}

// -----------------------------------------------------------------------------------------------
