#include "stdafx.h"
#include "..\Public\State_WDR_Run.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"

CState_WDR_Run::CState_WDR_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Run::Initialize()
{
	m_iRun = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_battle_1", 1.0f);
	if (m_iRun == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_Run::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iRun, 0.1f, 0, 0);


	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_WDR_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Run::Exit_State()
{
}

void CState_WDR_Run::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Iden_Skill"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("WDR_Identity"));
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
		
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Attack_1"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
		{
			if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
			{
				m_pPlayer->Set_State(TEXT("Iden_Idle"));
			}
			else
			{
				m_pPlayer->Set_State(TEXT("Idle"));
			}
		}
	}
}

void CState_WDR_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Run* CState_WDR_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Run* pInstance = new CState_WDR_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Run::Free()
{
	__super::Free();
}
