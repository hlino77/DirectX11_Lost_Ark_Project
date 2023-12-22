#include "stdafx.h"
#include "..\Public\State_WR_Idle.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Idle::CState_WR_Idle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Idle::Initialize()
{
	m_iIdle = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1", 1.0f);
	if (m_iIdle == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Idle::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Idle::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Idle::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
}

void CState_WR_Idle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Idle::Exit_State()
{
}

void CState_WR_Idle::Tick_State_Control(_float fTimeDelta)
{

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_Hit())
	{
		m_pPlayer->Set_State(TEXT("Hit"));
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

		m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_WR_Idle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Idle* CState_WR_Idle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Idle* pInstance = new CState_WR_Idle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Idle::Free()
{
	__super::Free();
}
