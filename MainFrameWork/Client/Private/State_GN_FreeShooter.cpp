#include "stdafx.h"
#include "..\Public\State_GN_FreeShooter.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_FreeShooter::CState_GN_FreeShooter(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_FreeShooter::Initialize()
{
	m_iFreeShoter = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_thefreeshooter", 1.2f);
	if (m_iFreeShoter == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_FreeShooter::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_FreeShooter::Tick_State_NoneControl;

	m_SkillFrames.push_back(8);
	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(50);
	m_SkillFrames.push_back(63);
	m_SkillFrames.push_back(78);
	m_SkillFrames.push_back(100);
	m_SkillFrames.push_back(101);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_FreeShooter::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFreeShoter, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::SHOT, m_eSkillSelectKey);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, true);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_FreeShooter::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_FreeShooter::Exit_State()
{
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_FreeShooter::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter))
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFreeShoter))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (110 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter))
		m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity())
	{
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
	}
}

void CState_GN_FreeShooter::Tick_State_NoneControl(_float fTimeDelta)
{
	if (110 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter))
		m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_FreeShooter* CState_GN_FreeShooter::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_FreeShooter* pInstance = new CState_GN_FreeShooter(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_FreeShooter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_FreeShooter::Free()
{
	__super::Free();
}
