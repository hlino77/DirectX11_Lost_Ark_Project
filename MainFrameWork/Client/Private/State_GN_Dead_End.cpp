#include "stdafx.h"
#include "..\Public\State_GN_Dead_End.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_GN_Dead_End::CState_GN_Dead_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Dead_End::Initialize()
{
	m_iDead_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"dead_1_loop", 1.0f);
	if (m_iDead_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Dead_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Dead_End::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Dead_End::Enter_State()
{
	m_pController->Get_HitEndMessage();

	m_pPlayer->Set_Invincible(true);

	if (TEXT("Fall") == m_pPlayer->Get_PreState())
	{
		m_pPlayer->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_pPlayer->Get_TargetPos());
	}

	m_pPlayer->Reserve_Animation(m_iDead_End, 0.1f, 0, 0);
}

void CState_GN_Dead_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Dead_End::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
}

void CState_GN_Dead_End::Tick_State_Control(_float fTimeDelta)
{
}

void CState_GN_Dead_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Dead_End* CState_GN_Dead_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Dead_End* pInstance = new CState_GN_Dead_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Dead_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Dead_End::Free()
{
	__super::Free();
}
