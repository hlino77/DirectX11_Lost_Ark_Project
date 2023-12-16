#include "stdafx.h"
#include "..\Public\State_GN_Attack_Long1.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Attack_Long1::CState_GN_Attack_Long1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Long1::Initialize()
{
	m_Attack_Long1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity2_1_01", 1.f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_Attack_Long1);
	if (m_Attack_Long1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Long1::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Long1::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Attack_Long1::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Attack_Long1, 0.1f, 0, 0);
	m_pController->Get_LookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Attack_Long1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Long1::Exit_State()
{
}

void CState_GN_Attack_Long1::Tick_State_Control(_float fTimeDelta)
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
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
		{
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
	}
	else if (true == m_pController->Is_Attack())
	{
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_Long_2"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
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
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long1))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Attack_Long1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Attack_Long1* CState_GN_Attack_Long1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Long1* pInstance = new CState_GN_Attack_Long1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Long1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Long1::Free()
{
	__super::Free();
}
