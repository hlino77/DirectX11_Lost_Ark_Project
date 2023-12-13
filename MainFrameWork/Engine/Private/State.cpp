#include "State.h"
#include "StateMachine.h"

CState::CState(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController)
	: m_strStateName(strStateName), m_pStateMachine(pMachine), m_pController(pController)
{
}

void CState::Free()
{
	__super::Free();
}
