#include "stdafx.h"
#include "..\Public\State_SP_StandDash.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"

CState_SP_StandDash::CState_SP_StandDash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_StandDash::Initialize()
{
	m_iStandDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_standup_1", 1.f);
	if (m_iStandDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_StandDash::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_StandDash::Tick_State_NoneControl;


	return S_OK;
}

void CState_SP_StandDash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iStandDash, 0.1f, 0, 0, 1.2f);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();

}

void CState_SP_StandDash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_StandDash::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);
}

void CState_SP_StandDash::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStandDash))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_SP_StandDash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_StandDash* CState_SP_StandDash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_StandDash* pInstance = new CState_SP_StandDash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_StandDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_StandDash::Free()
{
	__super::Free();
}
