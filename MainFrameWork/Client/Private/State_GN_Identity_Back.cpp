#include "stdafx.h"
#include "..\Public\State_GN_Identity_Back.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Identity_Back::CState_GN_Identity_Back(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Identity_Back::Initialize()
{
	m_iIdentity_HandtoShot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity2", 1.5f);
	if (-1 == m_iIdentity_HandtoShot)
		return E_FAIL;

	m_iIdentity_ShottoLong = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity3", 1.5f);
	if (-1 == m_iIdentity_ShottoLong)
		return E_FAIL;

	m_iIdentity_LongtoHand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity1", 1.5f);
	if (-1 == m_iIdentity_LongtoHand)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Identity_Back::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Identity_Back::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Identity_Back::Enter_State()
{
	if (m_pPlayer->Is_Control())
	{
		_int iChaneWeaponIndex = m_pPlayer->Get_WeaponIndex() + 1;

		if (iChaneWeaponIndex >= CPlayer_Controller_GN::GN_IDENTITY::_END)
			iChaneWeaponIndex = 0;

		m_pPlayer->Set_WeaponIndex(iChaneWeaponIndex);
	}

	CPlayer_Controller_GN::GN_IDENTITY eIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();
	CPlayer_Controller_GN::GN_IDENTITY eChangeIdentity = (CPlayer_Controller_GN::GN_IDENTITY)m_pPlayer->Get_WeaponIndex();

	if (CPlayer_Controller_GN::GN_IDENTITY::HAND == eChangeIdentity)
	{
		m_iIdentity = m_iIdentity_LongtoHand;
		m_pPlayer->Reserve_Animation(m_iIdentity, 0.1f, 0, 0);
	}
	else if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == eChangeIdentity)
	{
		m_iIdentity = m_iIdentity_HandtoShot;
		m_pPlayer->Reserve_Animation(m_iIdentity, 0.1f, 0, 0);
	}
	else if (CPlayer_Controller_GN::GN_IDENTITY::LONG == eChangeIdentity)
	{
		m_iIdentity = m_iIdentity_ShottoLong;
		m_pPlayer->Reserve_Animation(m_iIdentity, 0.1f, 0, 0);
	}

	static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_IdentityMessage(eChangeIdentity);
	m_pPlayer->Set_Weapon_RenderState(eChangeIdentity);
	m_pController->Get_StopMessage();
}

void CState_GN_Identity_Back::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Identity_Back::Exit_State()
{
}

void CState_GN_Identity_Back::Tick_State_Control(_float fTimeDelta)
{
	if (false == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity))
		return;

	m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Identity_Back::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Identity_Back* CState_GN_Identity_Back::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Identity_Back* pInstance = new CState_GN_Identity_Back(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Identity_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Identity_Back::Free()
{
	__super::Free();
}
