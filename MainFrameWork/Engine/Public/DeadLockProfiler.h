#pragma once
#include "Base.h"
#include <stack>
#include <map>
#include <vector>
#include <unordered_map>
#include <set>
#include <mutex>

BEGIN(Engine)

class ENGINE_DLL DeadLockProfiler : public CBase
{
	DECLARE_SINGLETON(DeadLockProfiler)
private:
	DeadLockProfiler();
	~DeadLockProfiler() = default;

public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	unordered_map<const char*, int32>	m_NameToId;
	unordered_map<int32, const char*>	m_IdToName;
	map<int32, set<int32>>				m_LockHistory;

	mutex m_Lock;

private:
	vector<int32>	m_DiscoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			m_DiscoveredCount = 0; // 노드가 발견된 순서
	vector<bool>	m_Finished; // Dfs(i)가 종료 되었는지 여부
	vector<int32>	m_Parent;
};

END