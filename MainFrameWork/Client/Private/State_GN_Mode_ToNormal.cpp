#include "stdafx.h"
#include "..\Public\State_GN_Mode_ToNormal.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Mode_ToNormal::CState_GN_Mode_ToNormal(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Mode_ToNormal::Initialize()
{
	m_iMode_ToNormal_H = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_normal_3", 1.0f);
	if (m_iMode_ToNormal_H == -1)
		return E_FAIL;

	m_iMode_ToNormal_S = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_normal_1", 1.0f);
	if (m_iMode_ToNormal_S == -1)
		return E_FAIL;

	m_iMode_ToNormal_L = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_normal_2", 1.0f);
	if (m_iMode_ToNormal_L == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Mode_ToNormal::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Mode_ToNormal::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Mode_ToNormal::Enter_State()
{
	CPlayer_Controller_GN::GN_IDENTITY eIden = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();

	switch (eIden)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_iMode_ToNormal = m_iMode_ToNormal_H;
		m_pPlayer->Reserve_Animation(m_iMode_ToNormal_H, 0.1f, 0, 0);
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_iMode_ToNormal = m_iMode_ToNormal_S;
		m_pPlayer->Reserve_Animation(m_iMode_ToNormal_S, 0.1f, 0, 0);
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_iMode_ToNormal = m_iMode_ToNormal_L;
		m_pPlayer->Reserve_Animation(m_iMode_ToNormal_L, 0.1f, 0, 0);
		break;
	}

	static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_IdentityMessage(eIden);
	m_pController->Get_StopMessage();
}

void CState_GN_Mode_ToNormal::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Mode_ToNormal::Exit_State()
{
}

void CState_GN_Mode_ToNormal::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iMode_ToNormal))
		m_pPlayer->Set_State(TEXT("Idle_Normal"));
}

void CState_GN_Mode_ToNormal::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Mode_ToNormal* CState_GN_Mode_ToNormal::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Mode_ToNormal* pInstance = new CState_GN_Mode_ToNormal(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Mode_ToNormal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Mode_ToNormal::Free()
{
	__super::Free();
}
