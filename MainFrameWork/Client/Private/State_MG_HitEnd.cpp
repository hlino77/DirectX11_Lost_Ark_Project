#include "stdafx.h"
#include "..\Public\State_MG_HitEnd.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_MG_HitEnd::CState_MG_HitEnd(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_HitEnd::Initialize()
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
		m_TickFunc = &CState_MG_HitEnd::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_HitEnd::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_HitEnd::Enter_State()
{
	m_fHitCheck = m_pPlayer->Get_TargetPos().y;
	if (10.f <= m_fHitCheck && 20.f > m_fHitCheck)
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

	m_pController->Get_HitEndMessage();

	m_IsFall = false;
	m_iFallFrame = 0;
}

void CState_MG_HitEnd::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_HitEnd::Exit_State()
{
	m_fTimeAcc = 0.f;
	m_pPlayer->Set_AnimationSpeed(1.f);
	m_IsAnimEnd = false;
}

void CState_MG_HitEnd::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_IsFall && m_iFallFrame == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iHitEnd))
	{
		m_pPlayer->Set_State(TEXT("Fall"));
	}
	if (false == m_IsFall && false == CNavigationMgr::GetInstance()->Is_NeighborActive(m_pPlayer->Get_CurrLevel(), m_pPlayer) &&
		2 == m_pPlayer->Get_ValtanPhase())
	{
		m_pPlayer->Set_Invincible(true);
		m_pPlayer->Set_Navi(false);
		m_IsFall = true;

		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHitEnd))
		{
			m_pPlayer->Set_State(TEXT("Fall"));
		}
		else
		{
			m_iFallFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iHitEnd) + 2;

			if (m_iFallFrame >= m_pPlayer->Get_ModelCom()->Get_Anim_MaxFrame(m_iHitEnd))
			{
				m_iFallFrame = m_pPlayer->Get_ModelCom()->Get_Anim_MaxFrame(m_iHitEnd);
			}
		}
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHitEnd))
	{
		m_pPlayer->Set_AnimationSpeed(0.f);
		m_IsAnimEnd = true;
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("StandDash"));
	}

	if (true == m_IsAnimEnd)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fDownTime <= m_fTimeAcc)
		{
			m_pPlayer->Set_State(TEXT("Stand"));
		}
	}
}

void CState_MG_HitEnd::Tick_State_NoneControl(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHitEnd))
	{
		m_pPlayer->Set_AnimationSpeed(0.f);
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_HitEnd* CState_MG_HitEnd::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_HitEnd* pInstance = new CState_MG_HitEnd(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_HitEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_HitEnd::Free()
{
	__super::Free();
}
