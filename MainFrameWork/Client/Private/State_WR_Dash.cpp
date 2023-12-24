#include "stdafx.h"
#include "..\Public\State_WR_Dash.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Dash::CState_WR_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Dash::Initialize()
{
	m_iDash_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_dash", 1.0f);
	if (m_iDash_Normal == -1)
		return E_FAIL;

	m_iDash_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_dash_identity1", 1.0f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Dash::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Dash::Enter_State()
{
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_iDash_Identity, 0.2f, 0, 0, 2.f);
		m_iDash = m_iDash_Identity;
	}
	else
	{
		m_pPlayer->Reserve_Animation(m_iDash_Normal, 0.2f, 0, 0, 1.8f);
		m_iDash = m_iDash_Normal;
	}

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_WR_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Dash::Exit_State()
{
}

void CState_WR_Dash::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Dash* CState_WR_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Dash* pInstance = new CState_WR_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Dash::Free()
{
	__super::Free();
}
