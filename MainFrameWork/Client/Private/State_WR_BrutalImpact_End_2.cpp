#include "stdafx.h"
#include "..\Public\State_WR_BrutalImpact_End_2.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_BrutalImpact_End_2::CState_WR_BrutalImpact_End_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_BrutalImpact_End_2::Initialize()
{
	m_iBrutalImpact_End_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_brutalimpact_04", 1.f);
	if (m_iBrutalImpact_End_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_BrutalImpact_End_2::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_BrutalImpact_End_2::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_BrutalImpact_End_2::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iBrutalImpact_End_2, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_End_2, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_End_2, 1.f);
}

void CState_WR_BrutalImpact_End_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_BrutalImpact_End_2::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_BrutalImpact_End_2::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iBrutalImpact_End_2))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_End_2))
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
				m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
			else
				m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
		{
			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			{
				if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
					m_pPlayer->Set_TargetPos(vClickPos);

				m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
			}
			else
			{
				m_pPlayer->Set_State(TEXT("WR_Identity"));
			}
		}
		else if (true == m_pController->Is_Run())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_End_2, 1.f);
}

void CState_WR_BrutalImpact_End_2::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_End_2, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_BrutalImpact_End_2* CState_WR_BrutalImpact_End_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_BrutalImpact_End_2* pInstance = new CState_WR_BrutalImpact_End_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_BrutalImpact_End_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_BrutalImpact_End_2::Free()
{
	__super::Free();
}
