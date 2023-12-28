#include "stdafx.h"
#include "..\Public\State_WDR_PowerStrike_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_PowerStrike_Loop::CState_WDR_PowerStrike_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PowerStrike_Loop::Initialize()
{
	m_iPowerStrike_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_powerstrike_02", 1.2f);
	if (m_iPowerStrike_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PowerStrike_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PowerStrike_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(26);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_PowerStrike_Loop::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPowerStrike_Loop, 0.1f, 0, 0, 1.f);
}

void CState_WDR_PowerStrike_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PowerStrike_Loop::Exit_State()
{
}

void CState_WDR_PowerStrike_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerStrike_Loop))
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (27 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerStrike_Loop))
		m_pPlayer->Set_State(TEXT("Skill_WDR_PowerStrike_End"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_PowerStrike_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_PowerStrike_Loop* CState_WDR_PowerStrike_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PowerStrike_Loop* pInstance = new CState_WDR_PowerStrike_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PowerStrike_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PowerStrike_Loop::Free()
{
	__super::Free();
}
