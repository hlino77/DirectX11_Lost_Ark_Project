#include "stdafx.h"
#include "..\Public\State_WR_BrutalImpact_End.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_BrutalImpact_End::CState_WR_BrutalImpact_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_BrutalImpact_End::Initialize()
{
	m_BrutalImpact_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_brutalimpact_03", 1.f);
	if (m_BrutalImpact_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_BrutalImpact_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_BrutalImpact_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(23);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WR_BrutalImpact_End::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_BrutalImpact_End, 0.1f, 0, 0, 1.f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_BrutalImpact_End, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_BrutalImpact_End, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_BrutalImpact_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_BrutalImpact_End::Exit_State()
{
	m_pController->Set_SkillSuccess(m_eSkillSelectKey, false);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_BrutalImpact_End::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_BrutalImpact_End))
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_BrutalImpact_End))
		m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End_2"));

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_BrutalImpact_End, 1.f);
}

void CState_WR_BrutalImpact_End::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_BrutalImpact_End, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_BrutalImpact_End* CState_WR_BrutalImpact_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_BrutalImpact_End* pInstance = new CState_WR_BrutalImpact_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_BrutalImpact_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_BrutalImpact_End::Free()
{
	__super::Free();
}
