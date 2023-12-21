#include "stdafx.h"
#include "..\Public\State_GN_Dash_2_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Dash_2_End::CState_GN_Dash_2_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Dash_2_End::Initialize()
{
	m_iDash_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1_04", 1.2f);
	if (m_iDash_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Dash_2_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Dash_2_End::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Dash_2_End::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash_End, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
}

void CState_GN_Dash_2_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Dash_2_End::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);
}

void CState_GN_Dash_2_End::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash_End))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Dash_2_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Dash_2_End* CState_GN_Dash_2_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Dash_2_End* pInstance = new CState_GN_Dash_2_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Dash_2_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Dash_2_End::Free()
{
	__super::Free();
}
