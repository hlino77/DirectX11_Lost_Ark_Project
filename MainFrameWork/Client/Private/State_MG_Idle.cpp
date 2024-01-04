#include "stdafx.h"
#include "..\Public\State_MG_Idle.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"

CState_MG_Idle::CState_MG_Idle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Idle::Initialize()
{
	m_iIdle_N = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1", 1.2f);
	if (m_iIdle_N == -1)
		return E_FAIL;

	m_iIdle_SP = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1_1", 1.2f);
	if (m_iIdle_SP == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Idle::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Idle::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_Idle::Enter_State()
{
	m_fSPTimeAcc = 0.f;
	m_iIdle = m_iIdle_N;
	m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
}

void CState_MG_Idle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Idle::Exit_State()
{
}

void CState_MG_Idle::Tick_State_Control(_float fTimeDelta)
{
	Change_Idle_Anim(fTimeDelta);

	_uint iIdentity = static_cast<CController_MG*>(m_pController)->Is_MG_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_Hit())
	{
		m_pPlayer->Set_State(TEXT("Hit"));
	}
	else if (0 != iIdentity)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_MG"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_MG_Back"));
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

void CState_MG_Idle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_MG_Idle::Change_Idle_Anim(_float fTimeDelta)
{
	if (m_iIdle == m_iIdle_N)
	{
		m_fSPTimeAcc += fTimeDelta;
		if (m_fSPTime <= m_fSPTimeAcc)
		{
			m_iIdle = m_iIdle_SP;
			m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);
			m_fSPTimeAcc = 0.f;
		}
	}
	else if (m_iIdle == m_iIdle_SP)
	{
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdle_SP))
		{
			m_iIdle = m_iIdle_N;
			m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);
		}
	}
}

CState_MG_Idle* CState_MG_Idle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Idle* pInstance = new CState_MG_Idle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Idle::Free()
{
	__super::Free();
}
