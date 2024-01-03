#include "stdafx.h"
#include "..\Public\State_GN_Run_Identity.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Run_Identity::CState_GN_Run_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Run_Identity::Initialize()
{
	m_Run_H = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity3", 1.0f);
	if (m_Run_H == -1)
		return E_FAIL;

	m_Run_S = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity1", 1.0f);
	if (m_Run_S == -1)
		return E_FAIL;

	m_Run_L = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity2", 1.0f);
	if (m_Run_L == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Run_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Run_Identity::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Run_Identity::Enter_State()
{
	if (m_pPlayer->Is_Control())
	{
		_int iChaneWeaponIndex = m_pPlayer->Get_WeaponIndex() - 1;

		if (iChaneWeaponIndex < 0)
			iChaneWeaponIndex = CPlayer_Controller_GN::GN_IDENTITY::_END - 1;

		m_pPlayer->Set_WeaponIndex(iChaneWeaponIndex);
	}

	CPlayer_Controller_GN::GN_IDENTITY eChangeIdentity = (CPlayer_Controller_GN::GN_IDENTITY)m_pPlayer->Get_WeaponIndex();

	switch (eChangeIdentity)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_Run = m_Run_H;
		m_pPlayer->Reserve_Animation(m_Run, 0.2f, 0, 12);
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_Run = m_Run_S;
		m_pPlayer->Reserve_Animation(m_Run, 0.2f, 0, 12);
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_Run = m_Run_L;
		m_pPlayer->Reserve_Animation(m_Run, 0.2f, 0, 12);
		break;
	}

	static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_IdentityMessage(eChangeIdentity);
	m_pPlayer->Set_Weapon_RenderState(eChangeIdentity);
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Run_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Run_Identity::Exit_State()
{
}

void CState_GN_Run_Identity::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Run())
	{
		m_pPlayer->Set_State(TEXT("Run"));
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Run_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Run_Identity* CState_GN_Run_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Run_Identity* pInstance = new CState_GN_Run_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : State_GN_Run_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Run_Identity::Free()
{
	__super::Free();
}
