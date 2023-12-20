#include "stdafx.h"
#include "..\Public\State_GN_Dash.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Dash::CState_GN_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Dash::Initialize()
{
	m_iDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moving_normal_1", 1.5f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Dash::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Dash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash, 0.1f, 0, 0, 2.f);
	m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iDash, 1.5f);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos(), 7.f);
}

void CState_GN_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Dash::Exit_State()
{
}

void CState_GN_Dash::Tick_State_Control(_float fTimeDelta)
{
	if (15 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash))
	{
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iDash, 1.f);
		m_pPlayer->Get_ModelCom()->Set_RootDist(1.5f);
	}

	if (false == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
		return;

	if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
			
		m_pPlayer->Set_State(TEXT("Run"));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		CPlayer_Controller_GN::GN_IDENTITY eIden = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();

		switch (eIden)
		{
		case Client::CPlayer_Controller_GN::HAND:
			m_pPlayer->Set_State(TEXT("Attack_Hand_1"));
			break;
		case Client::CPlayer_Controller_GN::SHOT:
			m_pPlayer->Set_State(TEXT("Attack_Shot_1"));
			break;
		case Client::CPlayer_Controller_GN::LONG:
			m_pPlayer->Set_State(TEXT("Attack_Long_1"));
			break;
		}
	}
	else
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	if (15 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash))
	{
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iDash, 1.f);
		m_pPlayer->Get_ModelCom()->Set_RootDist(1.5f);
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Dash* CState_GN_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Dash* pInstance = new CState_GN_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Dash::Free()
{
	__super::Free();
}
