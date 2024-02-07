#include "stdafx.h"
#include "..\Public\State_WR_Fall.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_WR_Fall::CState_WR_Fall(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Fall::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Fall::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Fall::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Fall::Enter_State()
{
	m_pController->Get_HitEndMessage();

	m_pPlayer->Set_AnimationSpeed(0.0f);
	m_pPlayer->Set_SuperiorArmorState(true);
	m_pPlayer->Set_Navi(false);
	//m_pPlayer->Get_RigidBody()->Set_Gravity(true);

	if (TEXT("Hit_Common") == m_pPlayer->Get_PreState())
	{
		m_vFallDir = m_pPlayer->Get_TargetPos();
		m_vFallDir.Normalize();
	}
	else
	{
		m_vFallDir = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		m_vFallDir.Normalize();
	}


	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::FORCE);
	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::VELOCITY_CHANGE);

	m_fTimeAcc = 0.0f;
	m_fStartAcc = 0.0f;

	m_bStart = false;

	m_pPlayer->Get_RendererCom()->Set_DeadScene(true);
}

void CState_WR_Fall::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Fall::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.0f);
	m_pPlayer->Set_SuperiorArmorState(false);
	m_pPlayer->Set_Navi(true);
	m_pPlayer->Get_RigidBody()->Set_Gravity(false);

	Vec3 vCellPos = CNavigationMgr::GetInstance()->Find_CloseCell_Middle(m_pPlayer->Get_CurrLevel(), m_pPlayer);
	vCellPos.y = 0;
	Vec3 vCenter = Vec3(100.f, 0.f, 100.f);
	Vec3 vDir = vCenter - vCellPos;
	vDir.Normalize();

	vCellPos += vDir * 0.4f;

	m_pPlayer->Set_TargetPos(vCellPos);
}

void CState_WR_Fall::Tick_State_Control(_float fTimeDelta)
{
	if (false == m_bStart)
	{
		m_fStartAcc += fTimeDelta;
		if (m_fStartTime < m_fStartAcc)
		{
			m_pPlayer->Get_RigidBody()->AddForce(m_vFallDir * -3.f, ForceMode::FORCE);
			m_pPlayer->Get_RigidBody()->Set_Gravity(true);

			m_bStart = true;
		}
	}
	if (true == m_bStart)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fFallTime <= m_fTimeAcc)
		{
			m_pPlayer->Set_State(TEXT("Dead_End"));
		}
	}
}

void CState_WR_Fall::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Fall* CState_WR_Fall::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Fall* pInstance = new CState_WR_Fall(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Fall::Free()
{
	__super::Free();
}
