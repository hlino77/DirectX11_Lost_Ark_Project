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
#include "Effect_Trail.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"


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
	m_AttackFrames.push_back(9);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Attack1::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_1, 0.2f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_SP*>(m_pController)->Set_Attack_Desc(0);

	m_bEffect = false;
	m_bTrail = false;
}

void CState_SP_Attack1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Attack1::Exit_State()
{
	m_IsAttackContinue = false;
	m_IsChargeAttack = false;

	if (m_pPlayer->Get_ServerState() != TEXT("Attack_2") && m_pPlayer->Get_ServerState() != TEXT("Attack_4"))
	{
		TrailEnd();
	}
}

void CState_SP_Attack1::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (m_bEffect == false && iAnimFrame > 7)
	{
		Effect_Shot();
		m_bEffect = true;
	}

	if (m_AttackFrames[m_iAttackCnt] == iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		15 > iAnimFrame &&
		5 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}
	if (true == m_pController->Is_HoldorTap(KEY::RBTN) &&
		20 > iAnimFrame &&
		15 <= iAnimFrame)
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
	else if (true == m_IsAttackContinue && 15 == iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_2"));
	}
	else if (true == m_IsChargeAttack && 20 == iAnimFrame)
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
		if (20 < iAnimFrame)
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

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (m_bEffect == false && iAnimFrame > 7)
	{
		Effect_Shot();
		m_bEffect = true;
	}
}

void CState_SP_Attack1::Effect_Trail()
{
	vector<CEffect*> Effects;
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("InkPaddleTrail"), func, Effects);

	m_pPlayer->Add_Effect(L"InkAttackTrail", Effects.front());
}

void CState_SP_Attack1::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"InkAttackTrail", 4.0f);
}

void CState_SP_Attack1::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"SP_Attack1", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 100.0f, 0.2f, 10.0f);
	}
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
