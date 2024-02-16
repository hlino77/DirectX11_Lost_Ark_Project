#include "State.h"
#include "StateMachine.h"
#include "Sound_Manager.h"

CState::CState(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController)
	: m_strStateName(strStateName), m_pStateMachine(pMachine), m_pController(pController)
{
}

void CState::StopStateSound()
{
	if (0 == m_SoundFrames.size())
		return;

	for (size_t i = 0; i < m_SoundFrames.size() -1 ; i++)
	{
		if (-1 == m_SoundFrames[i].iFrame) continue;

		CSound_Manager::GetInstance()->Stop_Channel_Sound(m_SoundFrames[i].strName);
	}
}

void CState::Free()
{
	__super::Free();
}
