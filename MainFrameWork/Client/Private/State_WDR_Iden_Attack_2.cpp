#include "stdafx.h"
#include "..\Public\State_WDR_Iden_Attack_2.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_Iden_Attack_2::CState_WDR_Iden_Attack_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Iden_Attack_2::Initialize()
{
	m_Attack_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_gravity_1_02", 1.4f);
	if (m_Attack_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Iden_Attack_2::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Iden_Attack_2::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(15);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_Iden_Attack_2::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_2, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_WDR*>(m_pController)->Set_Attack_Desc(2);

}

void CState_WDR_Iden_Attack_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Iden_Attack_2::Exit_State()
{
	m_IsAttackContinue = false;
	m_IsSkillContinue = false;
}

void CState_WDR_Iden_Attack_2::Tick_State_Control(_float fTimeDelta)
{

	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_iAttackCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_AttackMessage();

		Effect_Shot();
	}

	if (true == m_pController->Is_Attack() &&
		31 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2) &&
		21 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_IsAttackContinue = true;
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity() &&
		31 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2) &&
		21 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_IsAttackContinue = false;
		m_IsSkillContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_2))
	{
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Idle"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Idle"));
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
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Iden_Skill"));
	}
	else if (true == m_IsSkillContinue && 31 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Skill"));
		}
	}
	else if (true == m_IsAttackContinue && 31 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Attack_3"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (31 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
}

void CState_WDR_Iden_Attack_2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_iAttackCnt++;
		Effect_Shot();
	}
}

void CState_WDR_Iden_Attack_2::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(L"AttGravity2", &desc);

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 100.0f, 0.5f, 10.0f);
}



CState_WDR_Iden_Attack_2* CState_WDR_Iden_Attack_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Iden_Attack_2* pInstance = new CState_WDR_Iden_Attack_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Iden_Attack_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Iden_Attack_2::Free()
{
	__super::Free();
}
