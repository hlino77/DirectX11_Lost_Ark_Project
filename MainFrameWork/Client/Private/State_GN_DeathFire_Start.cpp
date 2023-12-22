#include "stdafx.h"
#include "..\Public\State_GN_DeathFire_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_DeathFire_Start::CState_GN_DeathFire_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_DeathFire_Start::Initialize()
{
	m_iDeathFire_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deathfire_start", 1.2f);
	if (m_iDeathFire_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_DeathFire_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_DeathFire_Start::Tick_State_NoneControl;

	m_SkillFrames.push_back(21);
	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(23);
	m_SkillFrames.push_back(24);
	m_SkillFrames.push_back(25);

	m_SkillFrames.push_back(30);
	m_SkillFrames.push_back(31);
	m_SkillFrames.push_back(32);
	m_SkillFrames.push_back(33);
	m_SkillFrames.push_back(34);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(36);
	m_SkillFrames.push_back(37);
	m_SkillFrames.push_back(38);

	m_SkillFrames.push_back(42);
	m_SkillFrames.push_back(43);
	m_SkillFrames.push_back(44);
	m_SkillFrames.push_back(45);
	m_SkillFrames.push_back(46);
	m_SkillFrames.push_back(47);
	m_SkillFrames.push_back(48);
	m_SkillFrames.push_back(49);
	m_SkillFrames.push_back(50);
	m_SkillFrames.push_back(51);
	m_SkillFrames.push_back(52);
	m_SkillFrames.push_back(53);
	m_SkillFrames.push_back(54);
	m_SkillFrames.push_back(55);
	m_SkillFrames.push_back(56);
	m_SkillFrames.push_back(57);
	m_SkillFrames.push_back(58);

	m_SkillFrames.push_back(62);
	m_SkillFrames.push_back(63);
	m_SkillFrames.push_back(65);
	m_SkillFrames.push_back(66);
	m_SkillFrames.push_back(67);

	m_SkillFrames.push_back(72);
	m_SkillFrames.push_back(73);
	m_SkillFrames.push_back(74);
	m_SkillFrames.push_back(75);
	m_SkillFrames.push_back(76);
	m_SkillFrames.push_back(77);
	m_SkillFrames.push_back(78);

	m_SkillFrames.push_back(90);
	m_SkillFrames.push_back(91);
	m_SkillFrames.push_back(92);
	m_SkillFrames.push_back(93);
	m_SkillFrames.push_back(94);
	m_SkillFrames.push_back(95);
	m_SkillFrames.push_back(96);
	m_SkillFrames.push_back(97);
	m_SkillFrames.push_back(98);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_DeathFire_Start::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iDeathFire_Start, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_DeathFire_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_DeathFire_Start::Exit_State()
{
}

void CState_GN_DeathFire_Start::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeathFire_Start))
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDeathFire_Start))
		m_pPlayer->Set_State(TEXT("Skill_GN_DeathFire_Success"));
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_DeathFire_Success"));
	}
}

void CState_GN_DeathFire_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_DeathFire_Start* CState_GN_DeathFire_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_DeathFire_Start* pInstance = new CState_GN_DeathFire_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_DeathFire_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_DeathFire_Start::Free()
{
	__super::Free();
}
