#include "stdafx.h"
#include "..\Public\State_GN_Run.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Run::CState_GN_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Run::Initialize()
{
	m_Run = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity3", 1.0f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Run);
	if (m_Run == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Run::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Run, 0.1f, 0, 0);
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Run::Exit_State()
{
}

void CState_GN_Run::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Run())
	{
		Vec3 vPos;
		if (m_pPlayer->Get_CellPickingPos(vPos))
		{
			m_pPlayer->Set_TargetPos(vPos);
			m_pController->Get_MoveMessage(vPos);
		}
		
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("GN_Idle"));
	}
}

void CState_GN_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Run* CState_GN_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Run* pInstance = new CState_GN_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Run::Free()
{
	__super::Free();
}
