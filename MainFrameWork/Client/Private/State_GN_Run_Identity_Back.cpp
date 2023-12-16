#include "stdafx.h"
#include "..\Public\State_GN_Run_Identity_Back.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Run_Identity_Back::CState_GN_Run_Identity_Back(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Run_Identity_Back::Initialize()
{
	m_Run_H = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity3", 1.0f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Run_H);
	if (m_Run_H == -1)
		return E_FAIL;

	m_Run_S = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity1", 1.0f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Run_S);
	if (m_Run_S == -1)
		return E_FAIL;

	m_Run_L = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity2", 1.0f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Run_L);
	if (m_Run_L == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Run_Identity_Back::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Run_Identity_Back::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Run_Identity_Back::Enter_State()
{
	CPlayer_Controller_GN::GN_IDENTITY eIden = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();	

	CPlayer_Controller_GN::GN_IDENTITY eChangeIden;
	switch (eIden)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_pPlayer->Reserve_Animation(m_Run_S, 0.2f, 0, 12);
		m_Run = m_Run_S;
		eChangeIden = CPlayer_Controller_GN::GN_IDENTITY::SHOT;
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_pPlayer->Reserve_Animation(m_Run_L, 0.2f, 0, 12);
		m_Run = m_Run_L;
		eChangeIden = CPlayer_Controller_GN::GN_IDENTITY::LONG;
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_pPlayer->Reserve_Animation(m_Run_H, 0.2f, 0, 12);
		m_Run = m_Run_H;
		eChangeIden = CPlayer_Controller_GN::GN_IDENTITY::HAND;
		break;
	}

	static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_IdentityMessage(eChangeIden);
	m_pPlayer->Set_Weapon_RenderState(eChangeIden);
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Run_Identity_Back::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Run_Identity_Back::Exit_State()
{
}

void CState_GN_Run_Identity_Back::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pController->Get_MoveMessage(vClickPos);
		}

		m_pPlayer->Set_State(TEXT("Run"));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		CPlayer_Controller_GN::GN_IDENTITY eIden = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();

		switch (eIden)
		{
		case Client::CPlayer_Controller_GN::HAND:
			m_pPlayer->Set_State(TEXT("Attack_Hand_1"));
			break;
		case Client::CPlayer_Controller_GN::SHOT:
			m_pPlayer->Set_State(TEXT("Attack_Shot_1"));
			break;
		case Client::CPlayer_Controller_GN::LONG:
			m_pPlayer->Set_State(TEXT("Attack_Long_1"));
			break;
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Run_Identity_Back::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Run_Identity_Back* CState_GN_Run_Identity_Back::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Run_Identity_Back* pInstance = new CState_GN_Run_Identity_Back(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : State_GN_Run_Identity_Back");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Run_Identity_Back::Free()
{
	__super::Free();
}
