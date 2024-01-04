#include "stdafx.h"
#include "..\Public\State_WDR_EarthEater.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_EarthEater::CState_WDR_EarthEater(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_EarthEater::Initialize()
{
	m_iEarthEater = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_eartheater", 1.2f);
	if (m_iEarthEater == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_EarthEater::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_EarthEater::Tick_State_NoneControl;

	m_SkillFrames.push_back(23);
	m_SkillFrames.push_back(48);
	m_SkillFrames.push_back(58);
	m_SkillFrames.push_back(68);
	m_SkillFrames.push_back(91);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_EarthEater::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iEarthEater, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_UseMarbleMessage();
}

void CState_WDR_EarthEater::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_EarthEater::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_EarthEater::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEarthEater))
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEarthEater))
		m_pPlayer->Set_State(TEXT("Idle"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	if (110 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEarthEater))
	{
	
		if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
		{
			m_pPlayer->Set_State(TEXT("WDR_Identity"));
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
}

void CState_WDR_EarthEater::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_EarthEater* CState_WDR_EarthEater::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_EarthEater* pInstance = new CState_WDR_EarthEater(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_EarthEater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_EarthEater::Free()
{
	__super::Free();
}
