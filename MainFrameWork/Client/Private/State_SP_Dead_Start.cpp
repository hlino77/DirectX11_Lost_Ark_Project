#include "stdafx.h"
#include "..\Public\State_SP_Dead_Start.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_SP_Dead_Start::CState_SP_Dead_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Dead_Start::Initialize()
{
	m_iDead_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"dead_1", 1.0f);
	if (m_iDead_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Dead_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Dead_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Dead_Start::Enter_State()
{
	m_pController->Get_HitEndMessage();
	m_pController->Get_DeadMessage();

	m_pPlayer->Set_Invincible(true);

	m_pPlayer->Reserve_Animation(m_iDead_Start, 0.1f, 0, 0);
}

void CState_SP_Dead_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Dead_Start::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
}

void CState_SP_Dead_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDead_Start))
	{
		m_pPlayer->Set_State(TEXT("Dead_End"));
	}
}

void CState_SP_Dead_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Dead_Start* CState_SP_Dead_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Dead_Start* pInstance = new CState_SP_Dead_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Dead_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Dead_Start::Free()
{
	__super::Free();
}
