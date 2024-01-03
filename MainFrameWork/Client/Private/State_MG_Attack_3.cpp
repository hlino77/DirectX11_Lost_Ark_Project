#include "stdafx.h"
#include "..\Public\State_MG_Attack_3.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_MG_Attack_3::CState_MG_Attack_3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Attack_3::Initialize()
{
	m_Attack_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_03", 1.2f);
	if (m_Attack_1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Attack_3::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Attack_3::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(10);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_MG_Attack_3::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_1, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_MG*>(m_pController)->Set_Attack_Desc(0);
}

void CState_MG_Attack_3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Attack_3::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_MG_Attack_3::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		m_iAttackCnt++;
		static_cast<CController_MG*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		35 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1) &&
		25 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_1))
		m_pPlayer->Set_State(TEXT("Idle"));

	_uint iIdentity = static_cast<CController_MG*>(m_pController)->Is_MG_Identity();

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
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_MG"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_MG_Back"));
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
	else if (true == m_IsAttackContinue && 35 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (35 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1))
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

void CState_MG_Attack_3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Attack_3* CState_MG_Attack_3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Attack_3* pInstance = new CState_MG_Attack_3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Attack_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Attack_3::Free()
{
	__super::Free();
}
