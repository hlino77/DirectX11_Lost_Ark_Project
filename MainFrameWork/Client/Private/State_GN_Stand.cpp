#include "stdafx.h"
#include "..\Public\State_GN_Stand.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Stand::CState_GN_Stand(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Stand::Initialize()
{
	m_iStand = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"standup_normal_1", 1.2f);
	if (m_iStand == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Stand::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Stand::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Stand::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iStand, 0.1f, 0, 0);
}

void CState_GN_Stand::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Stand::Exit_State()
{
}

void CState_GN_Stand::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStand))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Stand::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Stand* CState_GN_Stand::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Stand* pInstance = new CState_GN_Stand(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Stand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Stand::Free()
{
	__super::Free();
}
