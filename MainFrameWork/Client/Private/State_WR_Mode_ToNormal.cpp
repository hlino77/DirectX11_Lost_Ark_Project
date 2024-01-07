#include "stdafx.h"
#include "..\Public\State_WR_Mode_ToNormal.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Mode_ToNormal::CState_WR_Mode_ToNormal(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Mode_ToNormal::Initialize()
{
	m_iMode_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_normal", 1.2f);
	if (m_iMode_Normal == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Mode_ToNormal::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Mode_ToNormal::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Mode_ToNormal::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iMode_Normal, 0.2f, 0, 0);

	m_pController->Get_StopMessage();
}

void CState_WR_Mode_ToNormal::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Mode_ToNormal::Exit_State()
{
	m_pPlayer->Store_Part();
}

void CState_WR_Mode_ToNormal::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iMode_Normal))
		m_pPlayer->Set_State(TEXT("Idle_Normal"));
}

void CState_WR_Mode_ToNormal::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Mode_ToNormal* CState_WR_Mode_ToNormal::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Mode_ToNormal* pInstance = new CState_WR_Mode_ToNormal(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Mode_ToNormal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Mode_ToNormal::Free()
{
	__super::Free();
}
