#pragma once

#include <functional>
#include <mutex>
#include <iostream>

/*
	myThreadLoop th(3);														// Will use 3 threads

	struct data {															// Arbitrary external data
		int  a;
	};

	auto mainFunc = [&](size_t i, size_t id, data &d, size_t &res)			// The function to call repeatedly
	{
		size_t iterNo = (i - id) / th.getThreadsTotal();					// Get this thread's current iteration number

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
		void			doStop			()						{ _doStopAll = true;	 }			// Stop all the threads
		void			doStop			(size_t &id)			{ id = size_t(-1);		 }			// Stop current thread. Bound function must receive id param as a reference in this case!
		bool			isFound			() const				{ return _doStopAll;	 }
		size_t			getActive		() const				{ return _activeThreads; }
		size_t			getThreadsTotal	() const				{ return _threadsNum;	 }
		void			setThreadsNo	(size_t newNumber)											// Change number of threads
		{
			if (!_activeThreads)
				_threadsNum = newNumber;
		}

		template<class _funcType, class ..._ARGS>
		void exec(_funcType _func, size_t min, size_t max, _ARGS&&... _args)
		{
			std::vector<std::shared_ptr<std::thread>> vecThreads;

			_doStopAll = false;
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
			// Back up the thread's id, as it may be changed by boundFunc in order to break out of the loop early
			size_t ID = id;

			auto boundFunc = std::bind(externFunc, std::placeholders::_1, std::placeholders::_2, std::forward<_ARGS>(_args)...);

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread [" << id << "] has started" << std::endl;
			}

			// Calling main external function in the loop
			if(min < max)
			{
				// From min to max
				for (size_t i = min + id; i <= max; i += _threadsNum)
				{
					if (_doStopAll)
						break;

					boundFunc(i, ID);

					if (ID == -1)
					{
						if (!_isSilent)
						{
							std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
								std::cout << " -- thread [" << id << "] is stopped from within the bound func" << std::endl;
						}

						break;
					}
				}
			}
			else
			{
				// From max to min backwards
				for (size_t i = min - id; i >= max && i <= min; i -= _threadsNum)
				{
					if (_doStopAll)
						break;

					boundFunc(i, ID);

					if (ID == -1)
					{
						if (!_isSilent)
						{
							std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
								std::cout << " -- thread [" << id << "] is stopped from within the bound func" << std::endl;
						}

						break;
					}
				}
			}

			if (!_isSilent)
			{
				std::lock_guard<std::mutex> doLock(_mutex[MUTEX_CONSOLE]);
					std::cout << " -- thread [" << id << "] has finished" << std::endl;
			}

			{
				std::lock_guard<std::mutex> doLock(mutexInternal);
					_activeThreads--;
			}
		}

	private:

		size_t		_threadsNum, _activeThreads;
		std::mutex	_mutex[2], mutexInternal;
		bool		_doStopAll;
		bool		_isSilent;
};
