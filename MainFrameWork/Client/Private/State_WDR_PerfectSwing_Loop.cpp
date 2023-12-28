#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_PerfectSwing_Loop::CState_WDR_PerfectSwing_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PerfectSwing_Loop::Initialize()
{
	m_iPerfectSwing_Loop_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_loop_1", 1.2f);
	if (m_iPerfectSwing_Loop_1 == -1)
		return E_FAIL;

	m_iPerfectSwing_Loop_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_loop_2", 1.2f);
	if (m_iPerfectSwing_Loop_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PerfectSwing_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PerfectSwing_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 1.6f;
	m_fSkillSuccessTime_Min = 0.8f;
	m_fSkillSuccessTime_Max = 1.6f;

	return S_OK;
}

void CState_WDR_PerfectSwing_Loop::Enter_State()
{
	m_fSkillTimeAcc = 0;

	m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_1;
	m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);
}

void CState_WDR_PerfectSwing_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Loop::Exit_State()
{
}

void CState_WDR_PerfectSwing_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Success"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_1)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Fail"));
		
		}
		else if (m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_2)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Success"));
		}
	}

	if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_1)
	{
		m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_2;
		m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);
	}


	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_PerfectSwing_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_PerfectSwing_Loop* CState_WDR_PerfectSwing_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PerfectSwing_Loop* pInstance = new CState_WDR_PerfectSwing_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PerfectSwing_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PerfectSwing_Loop::Free()
{
	__super::Free();
}
