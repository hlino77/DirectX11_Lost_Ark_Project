#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <stack>
#include <map>
#include "Base.h"
#include "Lock.h"
#include <queue>


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
	void	ReserveManager(_uint iJobThreadCount);

	void	Launch(function<void(void)> callback);
	void	Join();

	void DestroyTLS();


	// job 을 추가한다.
	void EnqueueJob(std::function<void()> job);



private:
	// 총 Worker 쓰레드의 개수.
	_uint m_iJobThreadCount;
	// Worker 쓰레드를 보관하는 벡터.
	std::vector<std::thread> m_JobThreads;
	// 할일들을 보관하는 job 큐.
	std::queue<std::function<void()>> m_Jobs;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable m_CV_Jobs;
	std::mutex m_MutexJob;


	bool stop_all;

	// Worker 쓰레드
	void WorkerThread();


private:
	mutex			_lock;
	vector<thread>	_threads;

	map<int, thread::id> m_ThreadInfo;
};

END