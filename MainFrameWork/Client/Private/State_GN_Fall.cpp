#include "stdafx.h"
#include "..\Public\State_GN_Fall.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_GN_Fall::CState_GN_Fall(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Fall::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Fall::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Fall::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Fall::Enter_State()
{
	m_pController->Get_HitEndMessage();

	m_pPlayer->Set_AnimationSpeed(0.0f);
	m_pPlayer->Set_SuperiorArmorState(true);
	m_pPlayer->Set_Navi(false);
	m_pPlayer->Get_RigidBody()->Set_Gravity(true);

	m_fTimeAcc = 0.0f;
}

void CState_GN_Fall::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Fall::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.0f);
	m_pPlayer->Set_SuperiorArmorState(false);
	m_pPlayer->Set_Navi(true);
	m_pPlayer->Get_RigidBody()->Set_Gravity(false);

	m_pPlayer->Set_TargetPos(CNavigationMgr::GetInstance()->Find_CloseCell_Middle(m_pPlayer->Get_CurrLevel(), m_pPlayer));
}

void CState_GN_Fall::Tick_State_Control(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fFallTime < m_fTimeAcc)
	{
		m_pPlayer->Set_State(TEXT("Dead_End"));
	}
}

void CState_GN_Fall::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Fall* CState_GN_Fall::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Fall* pInstance = new CState_GN_Fall(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Fall::Free()
{
	__super::Free();
}