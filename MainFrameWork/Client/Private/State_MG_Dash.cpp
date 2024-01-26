#include "stdafx.h"
#include "..\Public\State_MG_Dash.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"

CState_MG_Dash::CState_MG_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Dash::Initialize()
{
	m_iDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_soundlllusion", 1.2f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Dash::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_Dash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash, 0.1f, 0, 0, 1.2f);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();

}

void CState_MG_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Dash::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);
}

void CState_MG_Dash::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_MG_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Dash* CState_MG_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Dash* pInstance = new CState_MG_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Dash::Free()
{
	__super::Free();
}
