#include "DeadLockProfiler.h"
#include "Lock.h"
#include "ThreadManager.h"
#include "Engine_Defines.h"

/*--------------------
	DeadLockProfiler
---------------------*/

IMPLEMENT_SINGLETON(DeadLockProfiler)

DeadLockProfiler::DeadLockProfiler()
{
}

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(m_Lock);
	
	TLSDESC* pTLS = nullptr;

	if(GetTLS(&pTLS) == false)
		return;

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = m_NameToId.find(name);
	if (findIt == m_NameToId.end())
	{
		lockId = static_cast<int32>(m_NameToId.size());
		m_NameToId[name] = lockId;
		m_IdToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면
	if (pTLS->LLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인한다.
		const int32 prevId = pTLS->LLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = m_LockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	pTLS->LLockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(m_Lock);

	TLSDESC* pTLS = nullptr;

	if (GetTLS(&pTLS) == false)
		return;

	if (pTLS->LLockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = m_NameToId[name];
	if (pTLS->LLockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	pTLS->LLockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(m_NameToId.size());
	m_DiscoveredOrder = vector<int32>(lockCount, -1);
	m_DiscoveredCount = 0;
	m_Finished = vector<bool>(lockCount, false);
	m_Parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다.
	m_DiscoveredOrder.clear();
	m_Finished.clear();
	m_Parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if (m_DiscoveredOrder[here] != -1)
		return;

	m_DiscoveredOrder[here] = m_DiscoveredCount++;

	// 모든 인접한 정점을 순회한다.
	auto findIt = m_LockHistory.find(here);
	if (findIt == m_LockHistory.end())
	{
		m_Finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (m_DiscoveredOrder[there] == -1)
		{
			m_Parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there는 here의 후손이다. (순방향 간선)
		if (m_DiscoveredOrder[here] < m_DiscoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다. (역방향 간선)
		if (m_Finished[there] == false)
		{
			printf("%s -> %s\n", m_IdToName[here], m_IdToName[there]);

			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", m_IdToName[m_Parent[now]], m_IdToName[now]);
				now = m_Parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	m_Finished[here] = true;
}
