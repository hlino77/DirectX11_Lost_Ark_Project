#include "stdafx.h"
#include "..\Public\State_WR_WildStomp.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "Model.h"

CState_WR_WildStomp::CState_WR_WildStomp(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_WildStomp::Initialize()
{
	m_iWildStomp = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_mountaincrash", 1.f);
	if (m_iWildStomp == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_WildStomp::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_WildStomp::Tick_State_NoneControl;

	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WR_WildStomp::Enter_State()
{
	m_iSkillCnt = 0;
	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iWildStomp, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildStomp, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildStomp, 1.f);

	m_pPlayer->Get_WR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_WR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_WildStomp::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_WildStomp::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_WildStomp::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iWildStomp);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (false == m_bEffectStart && 14 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_WildStomp"), &desc)

		m_pPlayer->Get_Camera()->Set_RadialBlur(0.1f, matPivot.Translation(), 0.25f, 0.04f);

		m_bEffectStart = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iWildStomp))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (25 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iWildStomp))
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
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

			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
				m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
			else
				m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
		{
			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			{
				if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
					m_pPlayer->Set_TargetPos(vClickPos);
				else
					m_pPlayer->Set_TargetPos(Vec3());

				m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
			}
			else
			{
				m_pPlayer->Set_State(TEXT("WR_Identity"));
			}
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

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildStomp, 1.f);
}

void CState_WR_WildStomp::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildStomp, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_WildStomp* CState_WR_WildStomp::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_WildStomp* pInstance = new CState_WR_WildStomp(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_WildStomp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_WildStomp::Free()
{
	__super::Free();
}
