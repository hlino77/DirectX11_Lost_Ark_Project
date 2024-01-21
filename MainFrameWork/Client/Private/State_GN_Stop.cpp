#include "stdafx.h"
#include "..\Public\State_GN_Stop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Stop::CState_GN_Stop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Stop::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Stop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Stop::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Stop::Enter_State()
{
	m_pPlayer->Set_AnimationSpeed(0.f);
}

void CState_GN_Stop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Stop::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.f);
}

void CState_GN_Stop::Tick_State_Control(_float fTimeDelta)
{
}

void CState_GN_Stop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Stop* CState_GN_Stop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Stop* pInstance = new CState_GN_Stop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Stop::Free()
{
	__super::Free();
}
