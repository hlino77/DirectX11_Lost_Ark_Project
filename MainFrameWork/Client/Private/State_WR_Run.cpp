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
	m_iRun = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_battle_1", 1.0f);
	if (m_iRun == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Run::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iRun, 0.1f, 0, 0);

	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
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

void CState_WR_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
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
