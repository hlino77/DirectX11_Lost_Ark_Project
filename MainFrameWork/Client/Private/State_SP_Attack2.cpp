#include "stdafx.h"
#include "..\Public\State_SP_Attack2.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_SP_Attack2::CState_SP_Attack2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Attack2::Initialize()
{
	m_Attack_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_02", 1.2f);
	if (m_Attack_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Attack2::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Attack2::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(5);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Attack2::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_2, 0.2f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_SP*>(m_pController)->Set_Attack_Desc(0);
}

void CState_SP_Attack2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Attack2::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_SP_Attack2::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		10 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2) &&
		3 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_2))
		m_pPlayer->Set_State(TEXT("Idle"));

	_uint iIdentity = static_cast<CController_SP*>(m_pController)->Is_SP_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != iIdentity)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_Moon"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_Sun"));
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
	else if (true == m_IsAttackContinue && 10 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_3"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (10 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
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

void CState_SP_Attack2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Attack2* CState_SP_Attack2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Attack2* pInstance = new CState_SP_Attack2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Attack2::Free()
{
	__super::Free();
}
