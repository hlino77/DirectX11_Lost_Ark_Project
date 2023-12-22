#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Shot.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_TargetDown_Shot::CState_GN_TargetDown_Shot(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Shot::Initialize()
{
	m_iTargetDown_Shot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_03", 1.0f);
	if (m_iTargetDown_Shot == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Shot::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Shot::Tick_State_NoneControl;

	m_SkillFrames.push_back(1);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_TargetDown_Shot::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iTargetDown_Shot, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
}

void CState_GN_TargetDown_Shot::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Shot::Exit_State()
{
}

void CState_GN_TargetDown_Shot::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iTargetDown_Shot))
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey, m_pPlayer->Get_TargetPos());
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iTargetDown_Shot))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Loop"));
	}
}

void CState_GN_TargetDown_Shot::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_TargetDown_Shot* CState_GN_TargetDown_Shot::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Shot* pInstance = new CState_GN_TargetDown_Shot(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Shot::Free()
{
	__super::Free();
}
