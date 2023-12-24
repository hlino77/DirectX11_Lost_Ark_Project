#include "stdafx.h"
#include "..\Public\State_WR_Run.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Run::CState_WR_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Run::Initialize()
{
	m_Run_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_battle_1", 1.0f);
	if (m_Run_Normal == -1)
		return E_FAIL;

	m_Run_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity1", 1.0f);
	if (m_Run_Identity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Run::Enter_State()
{
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_Run_Identity, 0.2f, 0, 0, 1.8f);
		m_iRun = m_Run_Identity;
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos(), 4.f);
	}
	else
	{
		m_pPlayer->Reserve_Animation(m_Run_Normal, 0.2f, 0, 0);
		m_iRun = m_Run_Normal;
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}

	
}

void CState_WR_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Run::Exit_State()
{
}

void CState_WR_Run::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
	{
		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("WR_Identity"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
		else
			m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			if (m_iRun == m_Run_Normal)
			{
				m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
			}
			else if (m_iRun == m_Run_Identity)
			{
				m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos(), 4.f);
			}
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}

	if (m_iRun == m_Run_Identity && false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_Run_Normal, 0.2f, 0, 0);
		m_iRun = m_Run_Normal;
	}
}

void CState_WR_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	if (m_iRun == m_Run_Normal)
	{
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}
	else if (m_iRun == m_Run_Identity)
	{
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos(), 4.f);
	}
	
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Run* CState_WR_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Run* pInstance = new CState_WR_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Run::Free()
{
	__super::Free();
}
