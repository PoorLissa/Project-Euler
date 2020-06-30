#pragma once

#include <functional>
#include <mutex>
#include <iostream>



class myThreadLoop {

	typedef std::function<void(size_t)> func_type_old;

	typedef std::function<void(size_t, void*)> func_type;

	public:

		enum mutexNames { MUTEX_DATA, MUTEX_CONSOLE };

		myThreadLoop(size_t threadsNum, bool isSilent = true) : _threadsNum(threadsNum), _isSilent(isSilent)
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
	myThreadLoop_2 th(3);
	std::mutex& m = th.getMutex(myThreadLoop_2::MUTEX_CONSOLE);

	auto mainFunc = [&](size_t i, size_t id, size_t data)
	{
		{
			std::lock_guard<std::mutex> doLock(m);
				std::cout << " -- th[" << id << "] says : i = " << i << ", data = " << data << std::endl;
		}
	};

	th.exec(mainFunc, 7, 11, 12345);

	========================================================================================================

	myThreadLoop_2 th(3);
	std::mutex& m = th.getMutex(myThreadLoop_2::MUTEX_CONSOLE);

	auto mainFunc = [&](size_t i, size_t id)
	{
		size_t zzz = i * i;

		if (zzz > 1000000)
		{
			th.doStop();

			std::lock_guard<std::mutex> doLock(m);
				std::cout << " -- th[" << id << "] says : i = " << i << ", data = " << i << std::endl;

			std::lock_guard<std::mutex> lck(th.getMutex(myThreadLoop_2::MUTEX_DATA));
				res = i;
		}
	};

	th.exec(mainFunc, 1, 555*555);

	if (th.isFound())
		std::cout << "\n res = " << res << std::endl;
*/

class myThreadLoop_2 {

	typedef std::function<void()> func_type;

	public:

		enum mutexNames { MUTEX_DATA, MUTEX_CONSOLE };

		myThreadLoop_2(size_t threadsNum, bool isSilent = true) : _threadsNum(threadsNum), _isSilent(isSilent)
		{
		}

		std::mutex&		getMutex	(const mutexNames name)	{ return _mutex[name];	 }
		void			doStop		()						{ _doStop = true;		 }
		bool			isFound		() const				{ return _doStop;		 }
		size_t			getActive	() const				{ return _activeThreads; }

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

						& myThreadLoop_2::threadFunc<_funcType, _ARGS...>, this, id, min, max, _func, std::forward<_ARGS>(_args)...

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
