#include "stdafx.h"
#include "..\Public\State_SP_Idle.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "Esther.h"

CState_SP_Idle::CState_SP_Idle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Idle::Initialize()
{
	m_iIdle = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1", 1.2f);
	if (m_iIdle == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Idle::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Idle::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Idle::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
}

void CState_SP_Idle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Idle::Exit_State()
{
}

void CState_SP_Idle::Tick_State_Control(_float fTimeDelta)
{
	_uint iIdentity = static_cast<CController_SP*>(m_pController)->Is_SP_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_EstherSkill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		switch (m_pController->Get_EstherType())
		{
		case (_uint)CEsther::ESTHERTYPE::SA:
			m_pPlayer->Set_State(TEXT("Esther_Silian"));
			break;
		case (_uint)CEsther::ESTHERTYPE::WY:
			m_pPlayer->Set_State(TEXT("Esther_Way"));
			break;
		case (_uint)CEsther::ESTHERTYPE::BT:
			m_pPlayer->Set_State(TEXT("Esther_Bahuntur"));
			break;
		}
	}
	else if (0 != iIdentity)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_SP_Moon"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_SP_Sun"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_SP_Idle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Idle* CState_SP_Idle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Idle* pInstance = new CState_SP_Idle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Idle::Free()
{
	__super::Free();
}
