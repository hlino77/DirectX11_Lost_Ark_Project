#include "stdafx.h"
#include "..\Public\State_GN_Dash_2.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Dash_2::CState_GN_Dash_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Dash_2::Initialize()
{
	m_iDash_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1_03", 1.2f);
	if (m_iDash_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Dash_2::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Dash_2::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Dash_2::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash_2, 0.1f, 0, 0, 2.5f);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Dash_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Dash_2::Exit_State()
{
}

void CState_GN_Dash_2::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash_2))
		m_pPlayer->Set_State(TEXT("Dash_2_End"));
}

void CState_GN_Dash_2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Dash_2* CState_GN_Dash_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Dash_2* pInstance = new CState_GN_Dash_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Dash_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Dash_2::Free()
{
	__super::Free();
}
