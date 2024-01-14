#include "stdafx.h"
#include "..\Public\State_GN_HitEnd.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_HitEnd::CState_GN_HitEnd(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_HitEnd::Initialize()
{
	m_iHitEnd_KnockDown = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"knockdown_land", 1.0f);
	if (m_iHitEnd_KnockDown == -1)
		return E_FAIL;

	m_iHitEnd_Bound = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"bound_hit_land", 1.0f);
	if (m_iHitEnd_Bound == -1)
		return E_FAIL;

	m_iHitEnd_Twist = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"twistknockdown_land", 1.0f);
	if (m_iHitEnd_Twist == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_HitEnd::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_HitEnd::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_HitEnd::Enter_State()
{
	m_eHitType = (CPlayer_Controller::HIT_TYPE)m_pPlayer->Get_TargetPos().x;
	m_fForceDist = m_pPlayer->Get_TargetPos().y;

	switch (m_eHitType)
	{
	case Engine::CPlayer_Controller::KNOCKDOWN:
		m_pPlayer->Reserve_Animation(m_iHitEnd_KnockDown, 0.1f, 0, 0);
		m_iHitEnd = m_iHitEnd_KnockDown;
		break;
	case Engine::CPlayer_Controller::BOUND:
		m_pPlayer->Reserve_Animation(m_iHitEnd_Bound, 0.1f, 0, 0);
		m_iHitEnd = m_iHitEnd_Bound;
		break;
	case Engine::CPlayer_Controller::TWIST:
		m_pPlayer->Reserve_Animation(m_iHitEnd_Twist, 0.1f, 0, 0);
		m_iHitEnd = m_iHitEnd_Twist;
		break;
	default:
		break;
	}
}

void CState_GN_HitEnd::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_HitEnd::Exit_State()
{
	m_fTimeAcc = 0.f;
	m_pPlayer->Set_AnimationSpeed(1.f);
	m_IsAnimEnd = false;
	m_pController->Get_HitEndMessage();
}

void CState_GN_HitEnd::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHitEnd))
	{
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

void CState_GN_HitEnd::Tick_State_NoneControl(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHitEnd))
	{
		m_pPlayer->Set_AnimationSpeed(0.f);
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_HitEnd* CState_GN_HitEnd::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_HitEnd* pInstance = new CState_GN_HitEnd(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_HitEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_HitEnd::Free()
{
	__super::Free();
}
