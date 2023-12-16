#include "stdafx.h"
#include "..\Public\State_GN_Attack_Hand3.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Attack_Hand3::CState_GN_Attack_Hand3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Hand3::Initialize()
{
	m_Attack_Hand3 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_03", 1.4f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Attack_Hand3);
	if (m_Attack_Hand3 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Hand3::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Hand3::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Attack_Hand3::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Attack_Hand3, 0.1f, 0, 0);
	m_pController->Get_LookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Attack_Hand3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Hand3::Exit_State()
{
}

void CState_GN_Attack_Hand3::Tick_State_Control(_float fTimeDelta)
{
	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != iIdentity)
	{
		if (50 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3))
		{
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
	}
	else if (true == m_pController->Is_Attack())
	{
		if (50 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_Hand_1"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (50 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (50 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Attack_Hand3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Attack_Hand3* CState_GN_Attack_Hand3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Hand3* pInstance = new CState_GN_Attack_Hand3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Hand3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Hand3::Free()
{
	__super::Free();
}
