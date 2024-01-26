#include "stdafx.h"
#include "..\Public\State_MG_Run.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"
#include "Esther.h"

CState_MG_Run::CState_MG_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Run::Initialize()
{
	m_iRun = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_battle_1", 1.0f);
	if (m_iRun == -1)
		return E_FAIL;


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Run::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_Run::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iRun, 0.1f, 0, 0);
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();
}

void CState_MG_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Run::Exit_State()
{
}

void CState_MG_Run::Tick_State_Control(_float fTimeDelta)
{
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
			break;
		case (_uint)CEsther::ESTHERTYPE::WY:
			m_pPlayer->Set_State(TEXT("Esther_Way"));
			break;
		case (_uint)CEsther::ESTHERTYPE::BT:
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

		m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
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
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}

}

void CState_MG_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Run* CState_MG_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Run* pInstance = new CState_MG_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Run::Free()
{
	__super::Free();
}
