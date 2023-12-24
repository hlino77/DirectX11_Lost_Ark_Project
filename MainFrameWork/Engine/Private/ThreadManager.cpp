#include "ThreadManager.h"
#include "Engine_Defines.h"



IMPLEMENT_SINGLETON(ThreadManager)
/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	//InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::ReserveManager(_uint iJobThreadCount)
{
	m_iJobThreadCount = iJobThreadCount;
	m_JobThreads.reserve(m_iJobThreadCount);
	for (size_t i = 0; i < m_iJobThreadCount; ++i)
	{
		m_JobThreads.emplace_back([this]() { this->WorkerThread(); });
	}
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			//InitTLS();
			callback();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();


	stop_all = true;
	m_CV_Jobs.notify_all();

	for (auto& t : m_JobThreads) {
		t.join();
	}
}


void ThreadManager::DestroyTLS()
{

}

void ThreadManager::EnqueueJob(std::function<void()> job)
{
	if (stop_all) 
	{
		throw std::runtime_error("ThreadPool 사용 중지됨");
	}
	{
		std::lock_guard<std::mutex> lock(m_MutexJob);
		m_Jobs.push(std::move(job));
	}
	m_CV_Jobs.notify_one();
}

void ThreadManager::WorkerThread()
{
	while (true) {
		std::unique_lock<std::mutex> lock(m_MutexJob);

		m_CV_Jobs.wait(lock, [this]() { return !this->m_Jobs.empty() || stop_all; });

		if (stop_all && this->m_Jobs.empty()) {
			return;
		}

		// 맨 앞의 job 을 뺀다.
		std::function<void()> job = std::move(m_Jobs.front());
		m_Jobs.pop();
		lock.unlock();

		// 해당 job 을 수행한다 :)
		job();
	}
}



