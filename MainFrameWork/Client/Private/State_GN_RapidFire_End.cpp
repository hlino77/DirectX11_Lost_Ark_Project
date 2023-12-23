#include "stdafx.h"
#include "..\Public\State_GN_RapidFire_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_RapidFire_End::CState_GN_RapidFire_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_RapidFire_End::Initialize()
{
	m_iRapidFire_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_rapidfire", 1.2f);
	if (m_iRapidFire_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_RapidFire_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_RapidFire_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(7);

	m_SkillFrames.push_back(9);
	m_SkillFrames.push_back(11);

	m_SkillFrames.push_back(16);
	m_SkillFrames.push_back(18);

	m_SkillFrames.push_back(23);
	m_SkillFrames.push_back(25);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_RapidFire_End::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iRapidFire_End, 0.2f, 2, 0, 1.8f);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_RapidFire_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_RapidFire_End::Exit_State()
{
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_RapidFire_End::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iRapidFire_End))
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iRapidFire_End))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_RapidFire_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_RapidFire_End* CState_GN_RapidFire_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_RapidFire_End* pInstance = new CState_GN_RapidFire_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_RapidFire_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_RapidFire_End::Free()
{
	__super::Free();
}
