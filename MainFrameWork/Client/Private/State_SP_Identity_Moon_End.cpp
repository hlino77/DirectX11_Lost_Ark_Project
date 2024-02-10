#include "stdafx.h"
#include "..\Public\State_SP_Identity_Moon_End.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "ServerSessionManager.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"

CState_SP_Identity_Moon_End::CState_SP_Identity_Moon_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Identity_Moon_End::Initialize()
{
	m_iIdentity_Moon_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_harmonyofyin_02", 1.0f);
	if (m_iIdentity_Moon_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Identity_Moon_End::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Identity_Moon_End::Tick_State_NoneControl;

	m_AttackFrames.push_back(1);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Identity_Moon_End::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_iIdentity_Moon_End, 0.1f, 0, 0);

	m_pPlayer->Set_SuperArmorState(true);
}

void CState_SP_Identity_Moon_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Identity_Moon_End::Exit_State()
{
	m_pPlayer->Set_SuperArmorState(false);

	TrailEnd();
}

void CState_SP_Identity_Moon_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity_Moon_End);

	if (m_AttackFrames[m_iAttackCnt] != -1 && m_AttackFrames[m_iAttackCnt] <= iAnimFrame)
	{
		Effect_Shot();

		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity_Moon_End))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (25 <= iAnimFrame)
	{
		_uint iIdentity = static_cast<CController_SP*>(m_pController)->Is_SP_Identity();

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

			m_pPlayer->Set_State(TEXT("Attack_1"));
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

void CState_SP_Identity_Moon_End::Tick_State_NoneControl(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity_Moon_End);

	if (m_AttackFrames[m_iAttackCnt] != -1 && m_AttackFrames[m_iAttackCnt] <= iAnimFrame)
	{
		Effect_Shot();

		m_iAttackCnt++;
		static_cast<CController_SP*>(m_pController)->Get_AttackMessage();
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_SP_Identity_Moon_End::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"SP_IdenMoon", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.2f, 10.0f);
	}
}

void CState_SP_Identity_Moon_End::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"IdenMoonTrail1", 3.0f);
	m_pPlayer->Delete_Effect_Trail(L"IdenMoonTrail2", 3.0f);
}

CState_SP_Identity_Moon_End* CState_SP_Identity_Moon_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Identity_Moon_End* pInstance = new CState_SP_Identity_Moon_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Identity_Moon_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Identity_Moon_End::Free()
{
	__super::Free();
}
