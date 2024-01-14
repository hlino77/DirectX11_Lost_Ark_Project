#include "stdafx.h"
#include "..\Public\State_MG_StandDash.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"

CState_MG_StandDash::CState_MG_StandDash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_StandDash::Initialize()
{
	m_iStandDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_standup_1", 1.f);
	if (m_iStandDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_StandDash::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_StandDash::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_StandDash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iStandDash, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_MG_StandDash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_StandDash::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);
}

void CState_MG_StandDash::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStandDash))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_MG_StandDash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_StandDash* CState_MG_StandDash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_StandDash* pInstance = new CState_MG_StandDash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_StandDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_StandDash::Free()
{
	__super::Free();
}
