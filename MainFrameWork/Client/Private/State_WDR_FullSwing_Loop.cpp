#include "stdafx.h"
#include "..\Public\State_WDR_FullSwing_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "UI_HoldingFrame.h"
#include "GameInstance.h"

CState_WDR_FullSwing_Loop::CState_WDR_FullSwing_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_FullSwing_Loop::Initialize()
{
	m_iFullSwing_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_fullswing_loop", 1.2f);
	if (m_iFullSwing_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_FullSwing_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_FullSwing_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 1.2f;
	m_fSkillSuccessTime_Min = 1.0f;
	m_fSkillSuccessTime_Max = 1.2f;

	CUI_HoldingFrame::HOLDING_SKILL_DESC HoldingDesc;
	HoldingDesc.strSkillName = TEXT("Ç® ½ºÀ®");
	HoldingDesc.fSkillTimeAcc = m_fSkillTimeAcc;
	HoldingDesc.fSkillEndTime = m_fSkillEndTime;
	HoldingDesc.fSkillSuccessTime_Min = 0.f;
	HoldingDesc.fSkillSuccessTime_Max = 0.f;
	m_pHoldingUI = static_cast<CUI_HoldingFrame*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC,
		_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Skill_HoldingGauge"), &HoldingDesc));
	if (nullptr == m_pHoldingUI)
		return E_FAIL;

	return S_OK;
}

void CState_WDR_FullSwing_Loop::Enter_State()
{
	m_fSkillTimeAcc = 0;
	m_pPlayer->Reserve_Animation(m_iFullSwing_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pHoldingUI->Set_SkillOn(true);
}

void CState_WDR_FullSwing_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_FullSwing_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
	m_pHoldingUI->Set_SkillOn(false);
}

void CState_WDR_FullSwing_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;
	m_pHoldingUI->Set_SkillTimeAcc(m_fSkillTimeAcc);
	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
		m_pPlayer->Set_State(TEXT("Skill_WDR_FullSwing_Success"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc)
		{
			m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
			m_pPlayer->Set_State(TEXT("Skill_WDR_FullSwing_Success"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_FullSwing_Fail"));
		}
	}


	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_FullSwing_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_FullSwing_Loop* CState_WDR_FullSwing_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_FullSwing_Loop* pInstance = new CState_WDR_FullSwing_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_FullSwing_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_FullSwing_Loop::Free()
{
	__super::Free();
	m_pHoldingUI->Set_Dead(true);
}
