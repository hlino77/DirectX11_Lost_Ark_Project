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
public:
	typedef struct ManagerTLS
	{
		uint32 LThreadId = 0;
		stack<int32>	LLockStack;
		SendBufferChunkRef LSendBufferChunk;
	}MTLS;


	DECLARE_SINGLETON(ThreadManager)


private:
	ThreadManager();
	~ThreadManager();

public:

	void	Launch(function<void(void)> callback);
	void	Join();

	void InitTLS();
	void DestroyTLS();


public:

	void					Push_TLS(MTLS tTLS);
	MTLS&					Get_TLS(thread::id ThreadID);
	map<int, thread::id>&	Get_ThreadInfos() { return m_ThreadInfo; }

private:
	mutex			_lock;
	vector<thread>	_threads;

	map<int, thread::id> m_ThreadInfo;
	unordered_map<thread::id, MTLS>	m_TLS;
};

END