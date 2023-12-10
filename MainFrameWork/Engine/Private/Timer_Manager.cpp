#include "../Public/Timer_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_float CTimer_Manager::Compute_TimeDelta(const wstring & strTimerTag)
{
	CTimer*		pTimer = Find_Timer(strTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	/* 타임델타를 계싼하고 리턴한다. */
	return pTimer->Compute_TimeDelta();	
}

HRESULT CTimer_Manager::Add_Timer(const wstring& strTimerTag)
{
	/* map은 중복된 키를 허용하지 않느다.*/
	CTimer*		pTimer = Find_Timer(strTimerTag);

	/* 추가하고자하는 strTimerTag에 해당하는 Pair데이터가 이미 추가되어있었다. */
	if (nullptr != pTimer)
		return E_FAIL;

	m_Timers.insert({ strTimerTag, CTimer::Create() });

	return S_OK;
}




CTimer * CTimer_Manager::Find_Timer(const wstring& strTimerTag) const
{
	/* 맵에서 제공해주는 Find함수는 이진탐색을 수행한다. */
	auto	iter = m_Timers.find(strTimerTag);

	if (iter == m_Timers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free(void)
{
	__super::Free();

	for (auto& Pair : m_Timers)
	{
		Safe_Release(Pair.second);
	}
	m_Timers.clear();
}