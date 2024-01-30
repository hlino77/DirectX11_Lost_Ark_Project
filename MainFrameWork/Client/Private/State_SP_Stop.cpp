#include "stdafx.h"
#include "..\Public\State_SP_Stop.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"

CState_SP_Stop::CState_SP_Stop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Stop::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Stop::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Stop::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Stop::Enter_State()
{
	m_pPlayer->Set_AnimationSpeed(0.f);
	m_pPlayer->Set_SuperiorArmorState(true);
}

void CState_SP_Stop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Stop::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.f);
	m_pPlayer->Set_SuperiorArmorState(false);
}

void CState_SP_Stop::Tick_State_Control(_float fTimeDelta)
{

}

void CState_SP_Stop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Stop* CState_SP_Stop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Stop* pInstance = new CState_SP_Stop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Stop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Stop::Free()
{
	__super::Free();
}
