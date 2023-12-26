#include "stdafx.h"
#include "..\Public\State_WR_BrutalImpact_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_BrutalImpact_Loop::CState_WR_BrutalImpact_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_BrutalImpact_Loop::Initialize()
{
	m_iBrutalImpact_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_brutalimpact_02", 1.f);
	if (m_iBrutalImpact_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_BrutalImpact_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_BrutalImpact_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 2.f;

	m_fSkillSuccessTime_Max = 2.f;
	m_fSkillSuccessTime_Min = 1.0f;

	return S_OK;
}

void CState_WR_BrutalImpact_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iBrutalImpact_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);

	m_fSkillTimeAcc = 0.f;
}

void CState_WR_BrutalImpact_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_BrutalImpact_Loop::Exit_State()
{
}

void CState_WR_BrutalImpact_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
		m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_fSkillSuccessTime_Max >= m_fSkillTimeAcc)
		{
			m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
			m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
		}
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);
}

void CState_WR_BrutalImpact_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_BrutalImpact_Loop* CState_WR_BrutalImpact_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_BrutalImpact_Loop* pInstance = new CState_WR_BrutalImpact_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_BrutalImpact_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_BrutalImpact_Loop::Free()
{
	__super::Free();
}
