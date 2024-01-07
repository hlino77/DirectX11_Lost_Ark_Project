#include "stdafx.h"
#include "..\Public\State_WR_Mode_ToBattle.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Mode_ToBattle::CState_WR_Mode_ToBattle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Mode_ToBattle::Initialize()
{
	m_iMode_Battle = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_battle", 1.2f);
	if (m_iMode_Battle == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Mode_ToBattle::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Mode_ToBattle::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Mode_ToBattle::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iMode_Battle, 0.2f, 0, 0);
	m_pController->Get_StopMessage();
}

void CState_WR_Mode_ToBattle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Mode_ToBattle::Exit_State()
{
}

void CState_WR_Mode_ToBattle::Tick_State_Control(_float fTimeDelta)
{
	if (5 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iMode_Battle))
	{
		m_pPlayer->UnStore_Part();
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iMode_Battle))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Mode_ToBattle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Mode_ToBattle* CState_WR_Mode_ToBattle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Mode_ToBattle* pInstance = new CState_WR_Mode_ToBattle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Mode_ToBattle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Mode_ToBattle::Free()
{
	__super::Free();
}
