#include "stdafx.h"
#include "..\Public\State_WDR_FullSwing_Fail.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Effect_Particle.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_FullSwing_Fail::CState_WDR_FullSwing_Fail(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_FullSwing_Fail::Initialize()
{
	m_iFullSwing_Fail = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_fullswing_fail", 1.2f);
	if (m_iFullSwing_Fail == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_FullSwing_Fail::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_FullSwing_Fail::Tick_State_NoneControl;

	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(24);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_FullSwing_Fail::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFullSwing_Fail, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_eEffectState = EFFECTSTATE::TRAIL1;
	m_Trail1.clear();
	m_Trail2.clear();
	m_Effects.clear();
}

void CState_WDR_FullSwing_Fail::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_FullSwing_Fail::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	m_pController->Set_SkillSuccess(m_eSkillSelectKey, false);
}

void CState_WDR_FullSwing_Fail::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFullSwing_Fail);

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFullSwing_Fail))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (m_eEffectState == EFFECTSTATE::TRAIL1 && iAnimFrame >= 11)
	{
		Effect_TrailStart();
		m_eEffectState = EFFECTSTATE::TRAIL2;
	}

	if (m_eEffectState == EFFECTSTATE::TRAIL2 && iAnimFrame >= 30)
	{
		Effect_TrailStart2();
		m_eEffectState = EFFECTSTATE::TRAILEND1;
	}

	if (m_eEffectState == EFFECTSTATE::TRAILEND1 && iAnimFrame >= 34)
	{
		Effect_TrailEnd1();
		m_eEffectState = EFFECTSTATE::EFFECTEND;
	}

	if (m_eEffectState == EFFECTSTATE::EFFECTEND && iAnimFrame >= 39)
	{
		Effect_End();
		m_eEffectState = EFFECTSTATE::STATEEND;
	}

	Update_Effect();

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	if (90 <= iAnimFrame)
	{
		if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
		{
			m_pPlayer->Set_State(TEXT("WDR_Identity"));
		}
		else if (true == m_pController->Is_Run())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
}

void CState_WDR_FullSwing_Fail::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFullSwing_Fail);

	if (m_eEffectState == EFFECTSTATE::TRAIL1 && iAnimFrame >= 11)
	{
		Effect_TrailStart();
		m_eEffectState = EFFECTSTATE::TRAIL2;
	}

	if (m_eEffectState == EFFECTSTATE::TRAIL2 && iAnimFrame >= 30)
	{
		Effect_TrailStart2();
		m_eEffectState = EFFECTSTATE::TRAILEND1;
	}

	if (m_eEffectState == EFFECTSTATE::TRAILEND1 && iAnimFrame >= 34)
	{
		Effect_TrailEnd1();
		m_eEffectState = EFFECTSTATE::EFFECTEND;
	}

	if (m_eEffectState == EFFECTSTATE::EFFECTEND && iAnimFrame >= 39)
	{
		Effect_End();
		m_eEffectState = EFFECTSTATE::STATEEND;
	}

	Update_Effect();
}

void CState_WDR_FullSwing_Fail::Effect_TrailStart()
{
	vector<CEffect*> Effects;
	auto func = bind(&CPlayer::Load_WorldMatrix, m_pPlayer, placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("FullSwingTrail1"), func, m_Trail1);

	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START_OUTLIST(L"FullSwingSmoke", &tDesc, m_Effects);
}

void CState_WDR_FullSwing_Fail::Effect_TrailStart2()
{
	vector<CEffect*> Effects;
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("FullSwingTrail2"), func, m_Trail2);
}

void CState_WDR_FullSwing_Fail::Effect_TrailEnd1()
{
	for (auto& Trail : m_Trail1)
	{
		dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(1.0f);
	}
	m_Trail1.clear();
}

void CState_WDR_FullSwing_Fail::Effect_TrailEnd2()
{
	for (auto& Trail : m_Trail2)
	{
		dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(1.0f);
	}
	m_Trail2.clear();
}

void CState_WDR_FullSwing_Fail::Effect_End()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START(L"FullSwingEndDecal", &tDesc);

	for (auto& Effect : m_Effects)
	{
		dynamic_cast<CEffect_Particle*>(Effect)->ParticleEnd();
	}
	m_Effects.clear();

	Effect_TrailEnd2();

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.5f, 10.0f);
}

void CState_WDR_FullSwing_Fail::Update_Effect()
{
	for (auto& Effect : m_Effects)
	{
		Effect->Update_Pivot(m_pPlayer->Get_TransformCom()->Get_WorldMatrix());
	}
}

CState_WDR_FullSwing_Fail* CState_WDR_FullSwing_Fail::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_FullSwing_Fail* pInstance = new CState_WDR_FullSwing_Fail(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_FullSwing_Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_FullSwing_Fail::Free()
{
	__super::Free();
}
