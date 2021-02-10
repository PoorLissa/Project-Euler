#pragma once

#include <iostream>



template <class T>
class myMatrix {

	public:

		myMatrix(size_t, size_t, T = 0);
		myMatrix(size_t, size_t, T*);
		myMatrix(const myMatrix &);
		myMatrix(	   myMatrix &&) noexcept;
		~myMatrix();

		myMatrix operator + (const myMatrix<T> &) const;
		myMatrix operator - (const myMatrix<T> &) const;
		myMatrix operator * (const myMatrix<T> &) const;

		template <class U> myMatrix operator * (const U) const;

		void print() const;

	private:
		size_t _Cols, _Rows;
		T* _data;
};

template <class T>
myMatrix<T>::myMatrix(size_t x, size_t y, T val) : _Cols(x), _Rows(y)
{
	_data = new T[_Cols * _Rows];

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = val;
}

template <class T>
myMatrix<T>::myMatrix(const myMatrix<T> &other) : _Cols(other._Cols), _Rows(other._Rows)
{
	_data = new T[_Cols * _Rows];

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = other._data[i];
}

template <class T>
myMatrix<T>::myMatrix(size_t x, size_t y, T *data) : _Cols(x), _Rows(y)
{
	_data = new T[_Cols * _Rows];

	for (size_t i = 0; i < _Cols * _Rows; i++)
		_data[i] = *data++;
}

template <class T>
myMatrix<T>::myMatrix(myMatrix<T>&& other) noexcept
{
	_Cols = other._Cols;
	_Rows = other._Rows;
	_data  = std::move(other._data);

	other._Cols = 0u;
	other._Rows = 0u;
	other._data = nullptr;
}

template <class T>
myMatrix<T>::~myMatrix()
{
	if (_data)
	{
		delete[] _data;
		_data = nullptr;
	}
}

template <class T>
void myMatrix<T>::print() const
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

template <class T>
myMatrix<T> myMatrix<T>::operator + (const myMatrix<T> &other) const
{
	myMatrix<T> res(_Cols, _Rows);

	if (_Cols == other._Cols && _Rows == other._Rows)
		for (size_t i = 0; i < _Cols, _Rows; i++)
			res._data[i] = _data[i] + other._data[i];

	return std::move(res);
}

template <class T>
myMatrix<T> myMatrix<T>::operator - (const myMatrix<T> &other) const
{
	myMatrix<T> res(_Cols, _Rows);

	if (_Cols == other._Cols && _Rows == other._Rows)
		for (size_t i = 0; i < _Cols, _Rows; i++)
			res._data[i] = _data[i] - other._data[i];

	return std::move(res);
}

template <class T>
template <class U>
myMatrix<T> myMatrix<T>::operator * (const U n) const
{
	myMatrix<T> res(_Cols, _Rows);

	for (size_t i = 0; i < _Cols * _Rows; i++)
		res._data[i] = static_cast<T>(_data[i] * n);

	return std::move(res);
}

// Matrix multiplication [A x B] is possible only when A._Cols == B._Rows
// Resulting matrix size will be [B._Cols x A._Rows]
template <class Type>
myMatrix<Type> myMatrix<Type>::operator * (const myMatrix<Type>& other) const
{
	if(_Cols != other._Rows)
		throw "Incompatible matrices";

	std::cout << " A[" <<	    _Cols << " x " <<	    _Rows << "]" << std::endl;
	std::cout << " B[" << other._Cols << " x " << other._Rows << "]" << std::endl;

	myMatrix<Type> res(_Rows, other._Cols, 0.0f);

	//myMatrix<Type> res(other._Cols, _Rows, 0.0f);

	Type* m1 = _data;
	Type* m2 = other._data;
	Type* m3 = res._data;

	for (size_t i = 0; i < res._Cols; i++)
	{
		for (size_t j = 0; j < res._Rows; j++)
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

	std::cout << " C[" << res._Cols << " x " << res._Rows << "]" << std::endl;

	return std::move(res);
}




























