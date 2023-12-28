#include "stdafx.h"
#include "..\Public\State_WDR_Attack_2.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_WDR_Attack_2::CState_WDR_Attack_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Attack_2::Initialize()
{
	m_Attack_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_02", 1.2f);
	if (m_Attack_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Attack_2::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Attack_2::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(29);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_Attack_2::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_2, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_WDR*>(m_pController)->Set_Attack_Desc(1);
}

void CState_WDR_Attack_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Attack_2::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_WDR_Attack_2::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		m_iAttackCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		40 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2) &&
		30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
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

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		m_pPlayer->Set_State(TEXT("WDR_Identity"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);


		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 40 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);


		m_pPlayer->Set_State(TEXT("Attack_3"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (40 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_2))
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

void CState_WDR_Attack_2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Attack_2* CState_WDR_Attack_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Attack_2* pInstance = new CState_WDR_Attack_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Attack_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Attack_2::Free()
{
	__super::Free();
}
