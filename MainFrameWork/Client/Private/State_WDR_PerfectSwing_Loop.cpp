#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"

CState_WDR_PerfectSwing_Loop::CState_WDR_PerfectSwing_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PerfectSwing_Loop::Initialize()
{
	m_iPerfectSwing_Loop_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_loop_1", 1.2f);
	if (m_iPerfectSwing_Loop_1 == -1)
		return E_FAIL;

	m_iPerfectSwing_Loop_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_loop_2", 1.2f);
	if (m_iPerfectSwing_Loop_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PerfectSwing_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PerfectSwing_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 1.6f;
	m_fSkillSuccessTime_Min = 0.8f;
	m_fSkillSuccessTime_Max = 1.6f;

	m_fEffectAcc = 0.1f;
	m_fEffectDelay = 0.1f;

	return S_OK;
}

void CState_WDR_PerfectSwing_Loop::Enter_State()
{
	m_fSkillTimeAcc = 0;

	m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_1;
	m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);

	m_bEffect = false;

	m_Effects.clear();

	m_fEffectAcc = m_fEffectDelay;
}

void CState_WDR_PerfectSwing_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Loop::Exit_State()
{
	m_pPlayer->Delete_Effect(L"PerfectParticle");
}

void CState_WDR_PerfectSwing_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_bEffect == false)
	{
		Effect_Start();
		m_bEffect = true;
	}
	else
	{
		Update_Effect(fTimeDelta);
	}

	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc > m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Success"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_1)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Fail"));
		
		}
		else if (m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_2)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Success"));
		}
	}

	if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_1)
	{
		m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_2;
		m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);
	}


	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_PerfectSwing_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WDR_PerfectSwing_Loop::Effect_Start()
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START_OUTLIST(L"PerfectSwingCircle", &tDesc, m_Effects);
	EFFECT_START_OUTLIST(L"PerfectSwingParticle", &tDesc, m_Effects);

	m_pPlayer->Add_Effect(L"PerfectCircle", m_Effects[0]);
	m_pPlayer->Add_Effect(L"PerfectParticle", m_Effects[1]);
}

void CState_WDR_PerfectSwing_Loop::Update_Effect(_float fTimeDelta)
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

	for (auto& Effectiter = m_Effects.begin(); Effectiter != m_Effects.end();)
	{
		if ((*Effectiter)->Is_Active())
		{
			(*Effectiter)->Update_Pivot(matWorld);
			++Effectiter;
		}
		else
		{
			Effectiter = m_Effects.erase(Effectiter);
		}
	}

	m_fEffectAcc += fTimeDelta;
	if (m_fEffectAcc >= m_fEffectDelay)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START_OUTLIST(L"PerfectSwingCharge", &tDesc, m_Effects);
		m_fEffectAcc = 0.0f;
	}
}

CState_WDR_PerfectSwing_Loop* CState_WDR_PerfectSwing_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PerfectSwing_Loop* pInstance = new CState_WDR_PerfectSwing_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PerfectSwing_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PerfectSwing_Loop::Free()
{
	__super::Free();
}
