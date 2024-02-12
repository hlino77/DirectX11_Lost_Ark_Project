#include "stdafx.h"
#include "..\Public\State_GN_Hit.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_GN_Hit::CState_GN_Hit(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Hit::Initialize()
{
	m_iHit_Down = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"down", 1.0f);
	if (m_iHit_Down == -1)
		return E_FAIL;

	m_iHit_KnockDown = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"knockdown", 1.0f);
	if (m_iHit_KnockDown == -1)
		return E_FAIL;

	m_iHit_Bound = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"bound", 1.0f);
	if (m_iHit_Bound == -1)
		return E_FAIL;

	m_iHit_Twist = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"twistknockdown", 1.0f);
	if (m_iHit_Twist == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Hit::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Hit::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Hit::Enter_State()
{
	m_pController->Get_StopMessage();

	m_vHitCenter = m_pPlayer->Get_TargetPos();
	m_vHitCenter.y = 0.f;

	Vec3 vPlayerPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = 0;

	if (FLT_EPSILON <= Vec3(vPlayerPos - m_vHitCenter).Length())
		m_pController->Get_LookMessage(m_vHitCenter);

	m_fHitCheck = m_pPlayer->Get_TargetPos().y;
	if (10.f > m_fHitCheck)
	{
		m_eHitType = CPlayer_Controller::HIT_TYPE::DOWN;
		m_fForceDist = m_fHitCheck;
	}
	else if (10.f <= m_fHitCheck && 20.f > m_fHitCheck)
	{
		m_eHitType = CPlayer_Controller::HIT_TYPE::KNOCKDOWN;
		m_fForceDist = m_fHitCheck - 10.f;
	}
	else if (20.f <= m_fHitCheck && 30.f > m_fHitCheck)
	{
		m_eHitType = CPlayer_Controller::HIT_TYPE::BOUND;
		m_fForceDist = m_fHitCheck - 20.f;
	}
	else if (30.f <= m_fHitCheck && 40.f > m_fHitCheck)
	{
		m_eHitType = CPlayer_Controller::HIT_TYPE::TWIST;
		m_fForceDist = m_fHitCheck - 30.f;
	}


	switch (m_eHitType)
	{
	case Engine::CPlayer_Controller::DOWN:
		m_pPlayer->Reserve_Animation(m_iHit_Down, 0.1f, 0, 0, m_fForceDist);
		m_iHit = m_iHit_Down;
		break;
	case Engine::CPlayer_Controller::KNOCKDOWN:
		m_pPlayer->Reserve_Animation(m_iHit_KnockDown, 0.1f, 0, 0, m_fForceDist);
		m_iHit = m_iHit_KnockDown;
		break;
	case Engine::CPlayer_Controller::BOUND:
		m_pPlayer->Reserve_Animation(m_iHit_Bound, 0.1f, 0, 0, m_fForceDist);
		m_iHit = m_iHit_Bound;
		break;
	case Engine::CPlayer_Controller::TWIST:
		m_pPlayer->Reserve_Animation(m_iHit_Twist, 0.1f, 0, 0, m_fForceDist);
		m_iHit = m_iHit_Twist;
		break;
	default:
		break;
	}


	m_pController->Get_SkillEndMessage();

	m_IsFall = false;
	m_iFallFrame = 0;
}

void CState_GN_Hit::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Hit::Exit_State()
{
	m_fTimeAcc = 0.f;
	m_pPlayer->Set_AnimationSpeed(1.f);
	m_IsAnimEnd = false;

}

void CState_GN_Hit::Tick_State_Control(_float fTimeDelta)
{
	if (false == CNavigationMgr::GetInstance()->Is_NeighborActive(m_pPlayer->Get_CurrLevel(), m_pPlayer) &&
		2 <= m_pPlayer->Get_ValtanPhase())
	{
		m_pPlayer->Set_State(TEXT("Fall"));
	}

	if (m_iHit == m_iHit_Down)
	{
		Hit_Down(fTimeDelta);
	}
	else
	{
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHit))
		{
			m_pPlayer->Set_State(TEXT("HitEnd"));
		}
	}
}

void CState_GN_Hit::Tick_State_NoneControl(_float fTimeDelta)
{
	if (m_iHit == m_iHit_Down)
	{
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHit))
		{
			m_pPlayer->Set_AnimationSpeed(0.f);
		}
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_Hit::Hit_Down(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHit))
	{
		m_pController->Get_HitEndMessage();
		m_pPlayer->Set_AnimationSpeed(0.f);
		m_IsAnimEnd = true;
	}

	if (true == m_IsAnimEnd)
	{
		if (true == m_pController->Is_Dash())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("StandDash"));
		}

		m_fTimeAcc += fTimeDelta;
		if (m_fDownTime <= m_fTimeAcc)
		{
			m_pPlayer->Set_State(TEXT("Stand"));
		}
	}
}

CState_GN_Hit* CState_GN_Hit::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Hit* pInstance = new CState_GN_Hit(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Hit::Free()
{
	__super::Free();
}
