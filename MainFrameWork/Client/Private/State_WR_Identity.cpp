#include "stdafx.h"
#include "..\Public\State_WR_Identity.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Identity::CState_WR_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Identity::Initialize()
{
	m_iIdentity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity1", 1.0f);
	if (m_iIdentity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Identity::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Identity::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iIdentity, 0.2f, 0, 0);

	m_pController->Get_StopMessage();
	
	static_cast<CController_WR*>(m_pController)->Get_WR_IdentityMessage();
}

void CState_WR_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Identity::Exit_State()
{
}

void CState_WR_Identity::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Identity* CState_WR_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Identity* pInstance = new CState_WR_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Identity::Free()
{
	__super::Free();
}
