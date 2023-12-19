#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <stack>
#include <map>
#include "Base.h"
#include "Lock.h"


BEGIN(Engine)
/*------------------
	ThreadManager
-------------------*/

class ENGINE_DLL ThreadManager : public CBase
{
	DECLARE_SINGLETON(ThreadManager)


private:
	ThreadManager();
	~ThreadManager();

public:

	void	Launch(function<void(void)> callback);
	void	Join();

	void DestroyTLS();

private:
	mutex			_lock;
	vector<thread>	_threads;

	map<int, thread::id> m_ThreadInfo;
};

END