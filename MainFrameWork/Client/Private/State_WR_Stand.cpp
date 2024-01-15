#include "stdafx.h"
#include "..\Public\State_WR_Stand.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Stand::CState_WR_Stand(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Stand::Initialize()
{
	m_iStand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"standup_1", 1.2f);
	if (m_iStand == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Stand::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Stand::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Stand::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iStand, 0.1f, 0, 0);
}

void CState_WR_Stand::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Stand::Exit_State()
{
}

void CState_WR_Stand::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStand))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Stand::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Stand* CState_WR_Stand::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Stand* pInstance = new CState_WR_Stand(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Stand::Free()
{
	__super::Free();
}
