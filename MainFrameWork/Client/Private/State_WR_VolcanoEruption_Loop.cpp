#include "stdafx.h"
#include "..\Public\State_WR_VolcanoEruption_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_VolcanoEruption_Loop::CState_WR_VolcanoEruption_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_VolcanoEruption_Loop::Initialize()
{
	m_iVolcano_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hell_blades_loop", 1.f);
	if (m_iVolcano_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_VolcanoEruption_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_VolcanoEruption_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(0);
	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(20);
	m_SkillFrames.push_back(25);
	m_SkillFrames.push_back(30);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(40);
	m_SkillFrames.push_back(45);
	m_SkillFrames.push_back(50);
	m_SkillFrames.push_back(-1);

	m_fSkillEndTime = 2.f;

	m_fSkillSuccessTime_Min = 1.f;
	m_fSkillSuccessTime_Max = 1.8f;

	return S_OK;
}

void CState_WR_VolcanoEruption_Loop::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iVolcano_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Loop, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_fSkillTimeAcc = 0.f;
}

void CState_WR_VolcanoEruption_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_VolcanoEruption_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_VolcanoEruption_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iVolcano_Loop))
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_WR_VolcanoEruption_Fail"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_fSkillSuccessTime_Max >= m_fSkillTimeAcc)
		{
			m_pPlayer->Set_State(TEXT("Skill_WR_VolcanoEruption_Success"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Skill_WR_VolcanoEruption_Fail"));
		}
	}

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}


	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Loop, 1.f);
}

void CState_WR_VolcanoEruption_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Loop, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_VolcanoEruption_Loop* CState_WR_VolcanoEruption_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_VolcanoEruption_Loop* pInstance = new CState_WR_VolcanoEruption_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_VolcanoEruption_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_VolcanoEruption_Loop::Free()
{
	__super::Free();
}
