#include "stdafx.h"
#include "..\Public\State_GN_Run.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "Esther.h"

CState_GN_Run::CState_GN_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Run::Initialize()
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
		m_TickFunc = &CState_GN_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Run::Enter_State()
{
	CPlayer_Controller_GN::GN_IDENTITY eIden = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();

	switch (eIden)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_pPlayer->Reserve_Animation(m_Run_H, 0.1f, 0, 0);
		m_Run = m_Run_H;
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_pPlayer->Reserve_Animation(m_Run_S, 0.1f, 0, 0);
		m_Run = m_Run_S;
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_pPlayer->Reserve_Animation(m_Run_L, 0.1f, 0, 0);
		m_Run = m_Run_L;
		break;
	}
	
	m_pController->Get_MoveSpeedMessage(3.f);
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_GN_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Run::Exit_State()
{
}

void CState_GN_Run::Tick_State_Control(_float fTimeDelta)
{
	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_EstherSkill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		switch (m_pController->Get_EstherType())
		{
		case (_uint)CEsther::ESTHERTYPE::SA:
			m_pPlayer->Set_State(TEXT("Esther_Silian"));
			break;
		case (_uint)CEsther::ESTHERTYPE::WY:
			m_pPlayer->Set_State(TEXT("Esther_Way"));
			break;
		case (_uint)CEsther::ESTHERTYPE::BT:
			m_pPlayer->Set_State(TEXT("Esther_Bahuntur"));
			break;
		}
	}
	else if (0 != iIdentity)
	{
		if (1 == iIdentity)
		{
			m_pPlayer->Set_State(TEXT("Identity_GN_Run"));
		}
		if (2 == iIdentity)
		{
			m_pPlayer->Set_State(TEXT("Identity_GN_Run_Back"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

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
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pController->Get_MoveMessage(vClickPos);
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Run* CState_GN_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Run* pInstance = new CState_GN_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Run::Free()
{
	__super::Free();
}
