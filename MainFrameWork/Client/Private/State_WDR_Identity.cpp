#include "stdafx.h"
#include "..\Public\State_WDR_Identity.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"

CState_WDR_Identity::CState_WDR_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Identity::Initialize()
{
	m_Idle_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_gravity_1", 1.0f);
	if (m_Idle_Identity == -1)
		return E_FAIL;


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Identity::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_Identity::Enter_State()
{
	m_fTimeAcc = 0.f;

	m_pPlayer->Reserve_Animation(m_Idle_Identity, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
	static_cast<CController_WDR*>(m_pController)->Get_WDR_IdentityMessage();
}

void CState_WDR_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Identity::Exit_State()
{
}

void CState_WDR_Identity::Tick_State_Control(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (0.5f <= m_fTimeAcc)
	{
		m_pPlayer->Set_State(TEXT("Iden_Idle"));
	}
}

void CState_WDR_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Identity* CState_WDR_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Identity* pInstance = new CState_WDR_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Identity::Free()
{
	__super::Free();
}
