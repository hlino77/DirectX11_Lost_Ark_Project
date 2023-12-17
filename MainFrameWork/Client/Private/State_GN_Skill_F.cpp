#include "stdafx.h"
#include "..\Public\State_GN_Skill_F.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_Skill_F::CState_GN_Skill_F(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Skill_F::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Skill_F::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Skill_F::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Skill_F::Enter_State()
{
	m_pController->Get_StopMessage();
	m_pController->Get_PlayerSkill(m_iSkillKey)->Enter();
}

void CState_GN_Skill_F::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Skill_F::Exit_State()
{
	m_pController->Get_PlayerSkill(m_iSkillKey)->Exit();
}

void CState_GN_Skill_F::Tick_State_Control(_float fTimeDelta)
{
	m_pController->Get_PlayerSkill(m_iSkillKey)->Tick(fTimeDelta);

	if (true == m_pController->Get_PlayerSkill(m_iSkillKey)->Is_SkillCancel())
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
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
		else if (true == static_cast<CPlayer_Controller_GN*>(m_pController)->Is_Idle())
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}
	else if (true == m_pController->Get_PlayerSkill(m_iSkillKey)->Is_SkillEnd())
	{
		if (true == m_pController->Is_Dash())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Dash"));
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
		else if (true == static_cast<CPlayer_Controller_GN*>(m_pController)->Is_Idle())
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}
}

void CState_GN_Skill_F::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_PlayerSkill(m_iSkillKey)->UnCtrl_Tick(fTimeDelta);
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Skill_F* CState_GN_Skill_F::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Skill_F* pInstance = new CState_GN_Skill_F(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Skill_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Skill_F::Free()
{
	__super::Free();
}
