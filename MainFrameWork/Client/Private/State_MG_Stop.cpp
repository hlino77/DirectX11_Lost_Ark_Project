#include "stdafx.h"
#include "..\Public\State_MG_Stop.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"

CState_MG_Stop::CState_MG_Stop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Stop::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Stop::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Stop::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_Stop::Enter_State()
{
	m_pPlayer->Set_AnimationSpeed(0.f);
}

void CState_MG_Stop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Stop::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.f);
}

void CState_MG_Stop::Tick_State_Control(_float fTimeDelta)
{
	
}

void CState_MG_Stop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Stop* CState_MG_Stop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Stop* pInstance = new CState_MG_Stop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Stop::Free()
{
	__super::Free();
}
