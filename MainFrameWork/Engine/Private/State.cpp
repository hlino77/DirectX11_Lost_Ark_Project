#include "State.h"
#include "StateMachine.h"

CState::CState(const wstring& strStateName)
	: m_strStateName(strStateName)
{

}

CState::CState(const CState& rhs, CStateMachine* pMachine)
	: m_strStateName(rhs.m_strStateName)
	, m_pStateMachine(pMachine)
{

}	


void CState::Free()
{
	__super::Free();
}
