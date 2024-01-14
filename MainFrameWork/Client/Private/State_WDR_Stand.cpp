#include "stdafx.h"
#include "..\Public\State_WDR_Stand.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"

CState_WDR_Stand::CState_WDR_Stand(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Stand::Initialize()
{
	m_iStand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"standup_1", 1.2f);
	if (m_iStand == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Stand::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Stand::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_Stand::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iStand, 0.1f, 0, 0);
}

void CState_WDR_Stand::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Stand::Exit_State()
{
}

void CState_WDR_Stand::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStand))
	{
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Idle"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}
}

void CState_WDR_Stand::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Stand* CState_WDR_Stand::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Stand* pInstance = new CState_WDR_Stand(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Stand::Free()
{
	__super::Free();
}
