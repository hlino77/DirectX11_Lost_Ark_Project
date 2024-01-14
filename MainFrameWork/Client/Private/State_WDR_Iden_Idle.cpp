#include "stdafx.h"
#include "..\Public\State_WDR_Iden_Idle.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"

CState_WDR_Iden_Idle::CState_WDR_Iden_Idle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Iden_Idle::Initialize()
{
	m_Idle_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1", 1.0f);
	if (m_Idle_Identity == -1)
		return E_FAIL;


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Iden_Idle::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Iden_Idle::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_Iden_Idle::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Idle_Identity, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
}

void CState_WDR_Iden_Idle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Iden_Idle::Exit_State()
{
}

void CState_WDR_Iden_Idle::Tick_State_Control(_float fTimeDelta)
{
	if (false == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Iden_Skill"));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Iden_Attack_1"));
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

void CState_WDR_Iden_Idle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Iden_Idle* CState_WDR_Iden_Idle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Iden_Idle* pInstance = new CState_WDR_Iden_Idle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Iden_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Iden_Idle::Free()
{
	__super::Free();
}
