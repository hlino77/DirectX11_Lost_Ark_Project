#include "stdafx.h"
#include "..\Public\State_WR_Identity_Attack_2.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"

CState_WR_Identity_Attack_2::CState_WR_Identity_Attack_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Identity_Attack_2::Initialize()
{
	m_Attack_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity1_1_02", 2.f);
	if (m_Attack_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Identity_Attack_2::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Identity_Attack_2::Tick_State_NoneControl;

	/* 일반공격 프레임 */
	m_AttackFrames.push_back(7);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_WR_Identity_Attack_2::Enter_State()
{
	m_bEffect = false;
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_2, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_WR_Identity_Attack_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Identity_Attack_2::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_WR_Identity_Attack_2::Tick_State_Control(_float fTimeDelta)
{
	Effect_Rage_Attack_2();

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= (_int)iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CController_WR*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		25 > iAnimFrame &&
		15 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_2))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 25 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Identity_Attack_3"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Attack_3"));
		}
	}
	else if (true == m_pController->Is_Run() && 25 < iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_WR_Identity_Attack_2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
	Effect_Rage_Attack_2();
}

void CState_WR_Identity_Attack_2::Effect_Rage_Attack_2()
{
	if (!m_bEffect)
	{
		m_bEffect = true;

		auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
		TRAIL_START(TEXT("Slayer_Rage_Attack_2"), func)
	}
}

CState_WR_Identity_Attack_2* CState_WR_Identity_Attack_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Identity_Attack_2* pInstance = new CState_WR_Identity_Attack_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Identity_Attack_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Identity_Attack_2::Free()
{
	__super::Free();
}
