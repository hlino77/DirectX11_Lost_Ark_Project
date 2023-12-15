#include "stdafx.h"
#include "..\Public\State_GN_Identity.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Identity::CState_GN_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Identity::Initialize()
{
	m_iIdentity_HandtoShot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity2", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_HandtoShot);
	if (-1 == m_iIdentity_HandtoShot)
		return E_FAIL;

	m_iIdentity_HandtoLong = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_HandtoLong);
	if (-1 == m_iIdentity_HandtoLong)
		return E_FAIL;

	m_iIdentity_ShottoHand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_ShottoHand);
	if (-1 == m_iIdentity_ShottoHand)
		return E_FAIL;

	m_iIdentity_ShottoLong = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_ShottoLong);
	if (-1 == m_iIdentity_ShottoLong)
		return E_FAIL;

	m_iIdentity_LongtoHand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_LongtoHand);
	if (-1 == m_iIdentity_LongtoHand)
		return E_FAIL;

	m_iIdentity_LongtoShot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdentity_LongtoShot);
	if (-1 == m_iIdentity_LongtoShot)
		return E_FAIL;


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Identity::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Identity::Enter_State()
{
	CPlayer_Controller_GN::GN_IDENTITY ePreIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_PreIdentity();
	CPlayer_Controller_GN::GN_IDENTITY eIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();

	if (CPlayer_Controller_GN::GN_IDENTITY::HAND == ePreIdentity)
	{
		if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == eIdentity)
		{
			m_iIdentity = m_iIdentity_HandtoShot;
		}
		if (CPlayer_Controller_GN::GN_IDENTITY::LONG == eIdentity)
		{
			m_iIdentity = m_iIdentity_HandtoLong;
		}
	}
	else if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == ePreIdentity)
	{
		if (CPlayer_Controller_GN::GN_IDENTITY::HAND == eIdentity)
		{
			m_iIdentity = m_iIdentity_ShottoHand;
		}
		if (CPlayer_Controller_GN::GN_IDENTITY::LONG == eIdentity)
		{
			m_iIdentity = m_iIdentity_ShottoLong;
		}
	}
	else if (CPlayer_Controller_GN::GN_IDENTITY::LONG == ePreIdentity)
	{
		if (CPlayer_Controller_GN::GN_IDENTITY::HAND == eIdentity)
		{
			m_iIdentity = m_iIdentity_LongtoHand;
		}
		if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == eIdentity)
		{
			m_iIdentity = m_iIdentity_LongtoShot;
		}
	}
	
	m_pPlayer->Set_Weapon_RenderState(eIdentity);
	m_pController->Get_StopMessage();
}

void CState_GN_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Identity::Exit_State()
{
}

void CState_GN_Identity::Tick_State_Control(_float fTimeDelta)
{
	if (false == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity))
		return;

	m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Identity* CState_GN_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Identity* pInstance = new CState_GN_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Identity::Free()
{
	__super::Free();
}
