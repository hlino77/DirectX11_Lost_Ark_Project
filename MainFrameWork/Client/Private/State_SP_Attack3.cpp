#include "stdafx.h"
#include "..\Public\State_SP_Attack3.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Trail.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_SP_Attack3::CState_SP_Attack3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Attack3::Initialize()
{
	m_Attack_3 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_01", 1.0f);
	if (m_Attack_3 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Attack3::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Attack3::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(18);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Attack3::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_3, 0.2f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_SP*>(m_pController)->Set_Attack_Desc(0);

	m_bEffect = false;
}

void CState_SP_Attack3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Attack3::Exit_State()
{
	m_IsAttackContinue = false;

	TrailEnd();
}

void CState_SP_Attack3::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_3);

	if (m_bEffect == false && iAnimFrame > 16)
	{
		Effect_Shot();
		m_bEffect = true;
	}

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= (_uint)iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		38 > iAnimFrame &&
		30 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_3))
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
	else if (true == m_IsAttackContinue && 38 <= iAnimFrame)
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
		if (38 < iAnimFrame)
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

void CState_SP_Attack3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_3);

	if (m_bEffect == false && iAnimFrame > 16)
	{
		Effect_Shot();
		m_bEffect = true;
	}
}

void CState_SP_Attack3::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"InkAttackTrail", 4.0f);
}

void CState_SP_Attack3::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"SP_Attack3", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.2f, 10.0f);
	}
}

CState_SP_Attack3* CState_SP_Attack3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Attack3* pInstance = new CState_SP_Attack3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Attack3::Free()
{
	__super::Free();
}
