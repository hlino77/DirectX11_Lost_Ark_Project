#include "stdafx.h"
#include "..\Public\State_GN_Attack_Long2.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Attack_Long2::CState_GN_Attack_Long2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Long2::Initialize()
{
	m_Attack_Long2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity2_1_02", 1.f);
	if (m_Attack_Long2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Long2::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Long2::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Attack_Long2::Enter_State()
{
	m_iShotFire++;

	m_pPlayer->Reserve_Animation(m_Attack_Long2, 0.2f, 0, 0);
	m_pController->Get_LerpLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Attack_Long2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Long2::Exit_State()
{
	if (3 < m_iShotFire)
		m_iShotFire = 0;
}

void CState_GN_Attack_Long2::Tick_State_Control(_float fTimeDelta)
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
		if (10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long2) && 3 >= m_iShotFire)
		{
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		}
		else if(true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long2) && 3 < m_iShotFire)
		{
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);


		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		switch (eKey)
		{
		case Engine::CPlayer_Controller::Q:
			m_pPlayer->Set_State(TEXT("Skill_Q"));
			break;
		case Engine::CPlayer_Controller::W:
			m_pPlayer->Set_State(TEXT("Skill_W"));
			break;
		case Engine::CPlayer_Controller::E:
			m_pPlayer->Set_State(TEXT("Skill_E"));
			break;
		case Engine::CPlayer_Controller::R:
			m_pPlayer->Set_State(TEXT("Skill_R"));
			break;
		case Engine::CPlayer_Controller::A:
			m_pPlayer->Set_State(TEXT("Skill_Q"));
			break;
		case Engine::CPlayer_Controller::S:
			m_pPlayer->Set_State(TEXT("Skill_S"));
			break;
		case Engine::CPlayer_Controller::D:
			m_pPlayer->Set_State(TEXT("Skill_D"));
			break;
		case Engine::CPlayer_Controller::F:
			m_pPlayer->Set_State(TEXT("Skill_F"));
			break;
		}
	}
	else if (true == m_pController->Is_Attack())
	{
		if (10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long2) && 3 >= m_iShotFire)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_Long_2"));
		}
		else if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long2) && 3 < m_iShotFire)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_Long_1"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long2) && 3 >= m_iShotFire)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
		else if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long2) && 3 < m_iShotFire)
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
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long2))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Attack_Long2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Attack_Long2* CState_GN_Attack_Long2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Long2* pInstance = new CState_GN_Attack_Long2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Long2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Long2::Free()
{
	__super::Free();
}
