#include "stdafx.h"
#include "..\Public\State_GN_QuickStep_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"

CState_GN_QuickStep_End::CState_GN_QuickStep_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_QuickStep_End::Initialize()
{
	m_iQuickStep_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hallucination_02", 1.2f);
	if (m_iQuickStep_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_QuickStep_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_QuickStep_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(4);
	m_SkillFrames.push_back(8);
	m_SkillFrames.push_back(-1);


	m_EffectFrames.push_back(EFFECTFRAMEDESC(4, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(8, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC());

	return S_OK;
}

void CState_GN_QuickStep_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iEffectCnt = 0;

	m_pPlayer->Reserve_Animation(m_iQuickStep_End, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_QuickStep_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_QuickStep_End::Exit_State()
{
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_QuickStep_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_End);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		matWorld.Translation(static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation());
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("QuickStepBullet"), &desc)

		m_iEffectCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iQuickStep_End))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (30 <= iAnimFrame)
	{
		if (true == m_pController->Is_Run())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Run"));
		}
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
}

void CState_GN_QuickStep_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_End);

	if (m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		matWorld.Translation(static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation());
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("QuickStepBullet"), &desc)

			m_iEffectCnt++;
	}
}

CState_GN_QuickStep_End* CState_GN_QuickStep_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_QuickStep_End* pInstance = new CState_GN_QuickStep_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_QuickStep_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_QuickStep_End::Free()
{
	__super::Free();
}
