#pragma once

#include <vector>
#include <iostream>

// -----------------------------------------------------------------------------------------------

class A {

	int _a;

public:

	A(int a) : _a(a) { std::cout << " Constructor A(" << _a << ")" << std::endl; }
	~A() { std::cout << " Destructor  A(" << _a << ")" << std::endl; }
};

// -----------------------------------------------------------------------------------------------

class zzz_Base {

private:
	std::vector<A*> _vec;

public:

	zzz_Base() {}
	virtual ~zzz_Base()
	{
#if 1
		std::vector<A*>& v = _vec;
		std::vector<A*>::iterator iter;

		for (iter = v.begin(); iter != v.end(); ++iter)
		{
			if (*iter)
			{
				delete* iter;
				*iter = nullptr;
			}
		}
#endif
	}

	std::vector<A*> getVec() {
		return _vec;
	}

	void add(A* ptr) {
		_vec.push_back(ptr);
	}

	template <class Type>
	void add2()
	{
		_vec.push_back(new Type(123));
	}
};

// -----------------------------------------------------------------------------------------------

class zzz_Derived : public zzz_Base {

public:
	zzz_Derived() : zzz_Base() {}
	virtual ~zzz_Derived()
	{
#if 0
		std::vector<A*> vec = getVec();
		std::vector<A*>::iterator iter;

		for (iter = vec.begin(); iter != vec.end(); ++iter)
		{
			if (*iter)
			{
				delete* iter;
				*iter = nullptr;
			}
		}
#endif
	}

	void populate()
	{
		add(new A(1));
		add(new A(2));
		add(new A(3));

		add2<A>();

		std::cout << " -- done populating" << std::endl;
	}
};

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

void runTest_1()
{
	{
		std::shared_ptr<A> ptr1(new A(1));
	}

	{
		std::vector< std::shared_ptr<A> > vec;
		std::vector< std::auto_ptr<A> > vec2;
		{
			std::shared_ptr<A> ptr2(new A(2));
			vec.push_back(ptr2);

			std::auto_ptr<A> ptr3(new A(3));

			vec2.emplace_back(ptr3);

			std::cout << " -- leaving the scope..." << std::endl;
		}
	}

	zzz_Derived ddd;

	ddd.populate();
	
	return;
}
