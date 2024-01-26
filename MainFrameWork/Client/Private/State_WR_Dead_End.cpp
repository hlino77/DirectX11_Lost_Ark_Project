#include "stdafx.h"
#include "..\Public\State_WR_Dead_End.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_WR_Dead_End::CState_WR_Dead_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Dead_End::Initialize()
{
	m_iDead_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"dead_1_loop", 1.0f);
	if (m_iDead_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Dead_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Dead_End::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Dead_End::Enter_State()
{
	m_pController->Get_HitEndMessage();

	m_pPlayer->Set_Invincible(true);

	if (TEXT("Fall") == m_pPlayer->Get_PreState())
	{
		m_pPlayer->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_pPlayer->Get_TargetPos());
		m_pController->Get_DeadMessage();
	}

	m_pPlayer->Reserve_Animation(m_iDead_End, 0.1f, 0, 0);
}

void CState_WR_Dead_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Dead_End::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
	m_pController->Get_DeadEndMessage();
}

void CState_WR_Dead_End::Tick_State_Control(_float fTimeDelta)
{
}

void CState_WR_Dead_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Dead_End* CState_WR_Dead_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Dead_End* pInstance = new CState_WR_Dead_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Dead_End::Free()
{
	__super::Free();
}
