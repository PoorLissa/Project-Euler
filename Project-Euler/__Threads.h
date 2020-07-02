#pragma once

#include <functional>
#include <mutex>
#include <iostream>



class myThreadLoop_old {

	typedef std::function<void(size_t)> func_type_old;

	typedef std::function<void(size_t, void*)> func_type;

	public:

		enum mutexNames { MUTEX_DATA, MUTEX_CONSOLE };

		myThreadLoop_old(size_t threadsNum, bool isSilent = true) : _threadsNum(threadsNum), _isSilent(isSilent)
		{
		}

		std::mutex &	getMutex(const mutexNames name)			{ return _mutex[name];	}
		void			doStop()								{ _doStop = true;		}

		template<class _funcType, class _SHARED, class _THREAD>
		void exec_old(_funcType _func, size_t min, size_t max, _SHARED &_sharedData, _THREAD &_threadData)
		{
			std::vector<std::shared_ptr<std::thread>> vecThreads;
			_doStop = false;

			for (size_t id = 0; id < _threadsNum; id++)
			{
				_threadData.id = id;

				vecThreads.emplace_back(
					std::make_shared<std::thread>
					(
						& myThreadLoop::threadFunc_old, this, id, min, max, std::bind(_func, std::ref(_sharedData), _threadData, std::placeholders::_1)
					));
			}

			for (auto th : vecThreads)
				th->join();
		}


		template<class _funcType, class _SHARED_DATA, class _THREAD_DATA>
		void exec(_funcType _func, size_t min, size_t max, _SHARED_DATA& _sharedData, _THREAD_DATA& _threadData)
		{
			std::vector<std::shared_ptr<std::thread>> vecThreads;
			_doStop = false;

			for (size_t id = 0; id < _threadsNum; id++)
			{
				vecThreads.emplace_back(
					std::make_shared<std::thread>
					(
						& myThreadLoop::threadFunc<_THREAD_DATA>, this, id, min, max,
							std::cref(_threadData), std::bind(_func, std::ref(_sharedData), std::placeholders::_2, std::placeholders::_1)
					));
			}

			for (auto th : vecThreads)
				th->join();
		}

	private:

		void threadFunc_old(size_t id, size_t min, size_t max, func_type_old externFunc)
		{
			if(!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread " << id << " has started" << std::endl;
			}

			// Calling main external function in the loop
			for (size_t i = min + id; i < max; i += _threadsNum)
			{
				if (_doStop)
					break;

				externFunc(i);
			}

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread " << id << " has finished" << std::endl;
			}
		}

		template<class _THREAD_DATA>
		void threadFunc(size_t id, size_t min, size_t max, const _THREAD_DATA &_threadData, func_type externFunc)
		{
			// Create this thread's own copy of this data:
			_THREAD_DATA thDataCopy = _threadData;
			thDataCopy.id = id;

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread " << id << " has started" << std::endl;
			}

			// Calling main external function in the loop
			for (size_t i = min + id; i < max; i += _threadsNum)
			{
				if (_doStop)
					break;

				externFunc(i, &thDataCopy);
			}

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread " << id << " has finished" << std::endl;
			}
		}

	private:

		size_t		_threadsNum;
		std::mutex	_mutex[2];
		bool		_doStop;
		bool		_isSilent;
};

// -----------------------------------------------------------------------------------------------------------------------

/*
	myThreadLoop th(3);														// Will use 3 threads

	struct data {															// Arbitrary external data
		int  a;
	};

	auto mainFunc = [&](size_t i, size_t id, data &d, size_t &res)			// The function to call repeatedly
	{
		size_t iterNo = (i - id) / th.getNumThreads();						// Get this thread's current iteration number

		if (iterNo % 100 == 0)												// Print out something
		{
			std::lock_guard<std::mutex> lockConsole(th.getMutex(myThreadLoop::MUTEX_CONSOLE));
				std::cout << " -- th[" << id << "] says : i = " << std::setw(4) << std::right << i << "; iterations passed: " << std::setw(3) << std::right << iterNo << std::endl;
		}

		if (iterNo % 200 == 0)												// Modify external data
		{
			std::lock_guard<std::mutex> lockData(th.getMutex(myThreadLoop::MUTEX_DATA));
				d.a = 1;
		}

		if (iterNo == 878)													// Stop on condition
		{
			th.doStop();

			std::lock_guard<std::mutex> lockConsole(th.getMutex(myThreadLoop::MUTEX_CONSOLE));
				std::cout << std::endl;
				std::cout << " -- th[" << id << "] says : i = " << i << "; Stop Condition Reached" << std::endl;
				std::cout << std::endl;

			std::lock_guard<std::mutex> lockData(th.getMutex(myThreadLoop::MUTEX_DATA));
				res = i;
		}
	};

	size_t res;
	data dat;

	// Referenced data must be wrapped in std::ref in order to avoid calling several copy constructors
	th.exec(mainFunc, 1, 3000, std::ref(dat), std::ref(res));

	// Optional: check if the execution has been stopped on condition
	if (th.isFound())
		std::cout << "\n Res was found: res = " << res << std::endl;
*/

class myThreadLoop {

	typedef std::function<void()> func_type;

	public:

		enum mutexNames { MUTEX_DATA, MUTEX_CONSOLE };

		myThreadLoop(size_t threadsNum, bool isSilent = true) : _threadsNum(threadsNum), _isSilent(isSilent)
		{
		}

		std::mutex&		getMutex		(const mutexNames name)	{ return _mutex[name];	 }
		void			doStop			()						{ _doStop = true;		 }
		bool			isFound			() const				{ return _doStop;		 }
		size_t			getActive		() const				{ return _activeThreads; }
		size_t			getNumThreads	() const				{ return _threadsNum;	 }

		template<class _funcType, class ..._ARGS>
		void exec(_funcType _func, size_t min, size_t max, _ARGS&&... _args)
		{
			std::vector<std::shared_ptr<std::thread>> vecThreads;

			_doStop = false;
			_activeThreads = _threadsNum;

			for (size_t id = 0; id < _threadsNum; id++)
			{
				vecThreads.emplace_back(
					std::make_shared<std::thread> (

						& myThreadLoop::threadFunc<_funcType, _ARGS...>, this, id, min, max, _func, std::forward<_ARGS>(_args)...

					));
			}

			for (auto th : vecThreads)
				th->join();
		}

	private:

		template <class _funcType, class ... _ARGS>
		void threadFunc(size_t id, size_t min, size_t max, _funcType externFunc, _ARGS&&... _args)
		{
			auto boundFunc = std::bind(externFunc, std::placeholders::_1, std::placeholders::_2, std::forward<_ARGS>(_args)...);

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread [" << id << "] has started" << std::endl;
			}

			// Calling main external function in the loop
			for (size_t i = min + id; i <= max; i += _threadsNum)
			{
				if (_doStop)
					break;

				boundFunc(i, id);
			}

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread [" << id << "] has finished" << std::endl;
			}

			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_DATA]);
					_activeThreads--;
			}
		}

	private:

		size_t		_threadsNum, _activeThreads;
		std::mutex	_mutex[2];
		bool		_doStop;
		bool		_isSilent;
};
