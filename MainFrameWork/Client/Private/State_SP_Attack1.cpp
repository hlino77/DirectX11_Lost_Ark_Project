#include "stdafx.h"
#include "..\Public\State_SP_Attack1.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_SP_Attack1::CState_SP_Attack1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Attack1::Initialize()
{
	m_Attack_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_03", 1.2f);
	if (m_Attack_1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Attack1::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Attack1::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(7);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Attack1::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_1, 0.2f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_SP*>(m_pController)->Set_Attack_Desc(0);
}

void CState_SP_Attack1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Attack1::Exit_State()
{
	m_IsAttackContinue = false;
	m_IsChargeAttack = false;
}

void CState_SP_Attack1::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		15 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1) &&
		5 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		m_IsAttackContinue = true;
	}
	if (true == m_pController->Is_HoldorTap(KEY::RBTN) &&
		20 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1) &&
		15 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		m_IsAttackContinue = false;
		m_IsChargeAttack = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_1))
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
	else if (true == m_IsAttackContinue && 15 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_2"));
	}
	else if (true == m_IsChargeAttack && 20 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_4"));
	}
	else if (true == m_pController->Is_Run() && false == m_IsAttackContinue)
	{
		if (20 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
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

void CState_SP_Attack1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Attack1* CState_SP_Attack1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Attack1* pInstance = new CState_SP_Attack1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Attack1::Free()
{
	__super::Free();
}
