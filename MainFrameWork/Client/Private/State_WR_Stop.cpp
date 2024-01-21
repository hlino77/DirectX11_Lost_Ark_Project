#include "stdafx.h"
#include "..\Public\State_WR_Stop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Stop::CState_WR_Stop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Stop::Initialize()
{

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Stop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Stop::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Stop::Enter_State()
{
	m_pPlayer->Set_AnimationSpeed(0.f);
}

void CState_WR_Stop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Stop::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.f);
}

void CState_WR_Stop::Tick_State_Control(_float fTimeDelta)
{
	
}

void CState_WR_Stop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Stop* CState_WR_Stop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Stop* pInstance = new CState_WR_Stop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Stop::Free()
{
	__super::Free();
}
