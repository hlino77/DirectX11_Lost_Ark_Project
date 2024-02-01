#include "stdafx.h"
#include "..\Public\State_GN_Dash.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Dash::CState_GN_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Dash::Initialize()
{
	m_iDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1_01", 1.2f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Dash::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Dash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_GN_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Dash::Exit_State()
{
	m_bDashContinue = false;
}

void CState_GN_Dash::Tick_State_Control(_float fTimeDelta)
{
	if (4 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash) && true == m_pController->Is_Dash())
	{
		m_bDashContinue = true;
	}

	if (true == m_bDashContinue && true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash_2"));
	}
	else if (false == m_bDashContinue && true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
		m_pPlayer->Set_State(TEXT("Dash_End"));	
}

void CState_GN_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Dash* CState_GN_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Dash* pInstance = new CState_GN_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Dash::Free()
{
	__super::Free();
}
