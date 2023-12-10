#include "ThreadManager.h"
#include "Engine_Defines.h"


IMPLEMENT_SINGLETON(ThreadManager)
/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
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
}

void ThreadManager::InitTLS()
{
	static atomic<uint32> SThreadId = 1;
	MTLS tTLS;
	tTLS.LThreadId = SThreadId.fetch_add(1);

	Push_TLS(tTLS);
}

void ThreadManager::DestroyTLS()
{

}


void ThreadManager::Push_TLS(MTLS tTLS)
{
	std::lock_guard<mutex> lock(_lock);


	m_TLS.emplace(this_thread::get_id(), tTLS);
}

ThreadManager::MTLS& ThreadManager::Get_TLS(thread::id ThreadID)
{
	std::lock_guard<mutex> lock(_lock);

	return m_TLS[ThreadID];
}
