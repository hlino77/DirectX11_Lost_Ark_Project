#include "stdafx.h"
#include "..\Public\State_GN_PerfectShot_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Effect.h"

CState_GN_PerfectShot_Loop::CState_GN_PerfectShot_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_PerfectShot_Loop::Initialize()
{
	m_iPerfectShot_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectshot_loop", 1.0f);
	if (m_iPerfectShot_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_PerfectShot_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_PerfectShot_Loop::Tick_State_NoneControl;

	m_fSkillEndTime = 4.f;

	m_fSkillSuccessTime_Min = 3.f;
	m_fSkillSuccessTime_Max = 3.5f;

	m_fEffectChargeTime = 0.23f;

	m_fEffectCharge2Time = 0.4f;

	return S_OK;
}

void CState_GN_PerfectShot_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iPerfectShot_Loop, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);

	m_bEffect = false;

	m_fEffectChargeAcc = 0.4f;
	m_fEffectCharge2Acc = 0.3f;
}

void CState_GN_PerfectShot_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_PerfectShot_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;

	Effect_Glow(false);


	for (auto& Effect : m_ChargeEffects)
	{
		Effect->EffectEnd();
	}
	m_ChargeEffects.clear();
}

void CState_GN_PerfectShot_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;

	Vec3 vClickPos;
	if (m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pController->Get_LerpDirLookMessage(vClickPos, 10.f);
	}

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_PerfectShot_End"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_PerfectShot_End"));

		if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_fSkillSuccessTime_Max >= m_fSkillTimeAcc)
			m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Set_SkillSuccess(true);
	}

	if (true == m_pController->Is_Dash())
	{
		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (m_pPlayer->Get_ModelCom()->Get_CurrAnim() == m_iPerfectShot_Loop)
	{
		if (m_bEffect == false)
		{
			Effect_Glow(true);
			m_bEffect = true;
		}

		Update_Effect(fTimeDelta);
	}
}

void CState_GN_PerfectShot_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_pPlayer->Get_ModelCom()->Get_CurrAnim() == m_iPerfectShot_Loop)
	{
		if (m_bEffect == false)
		{
			Effect_Glow(true);
			m_bEffect = true;
		}

		Update_Effect(fTimeDelta);
	}
}



void CState_GN_PerfectShot_Loop::Effect_Glow(_bool bOnOff)
{
	if (bOnOff)
	{
		vector<CEffect*> EffectList;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = const_cast<Matrix*>(&static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix());
		EFFECT_START_OUTLIST(TEXT("LongLazer"), &desc, EffectList);
		EFFECT_START_OUTLIST(TEXT("PerpectShotGlow"), &desc, EffectList);

		m_pEffectGlow = EffectList[0];
		m_pEffectLazer = EffectList[1];
	}
	else
	{
		if (m_pEffectGlow)
		{
			m_pEffectGlow->EffectEnd();
			m_pEffectGlow = nullptr;
		}

		if (m_pEffectLazer)
		{
			m_pEffectLazer->EffectEnd();
			m_pEffectLazer = nullptr;
		}
	}

	

}

void CState_GN_PerfectShot_Loop::Update_Effect(_float fTimeDelta)
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix();

	if (m_pEffectGlow)
		m_pEffectGlow->Update_Pivot(matWorld);

	if (m_pEffectLazer)
		m_pEffectLazer->Update_Pivot(matWorld);


	for (auto& iter = m_ChargeEffects.begin(); iter != m_ChargeEffects.end();)
	{
		if ((*iter)->Is_Active())
		{
			(*iter)->Update_Pivot(matWorld);
			++iter;
		}
		else
		{
			iter = m_ChargeEffects.erase(iter);
		}
	}

	m_fEffectChargeAcc += fTimeDelta;

	if (m_fEffectChargeAcc >= m_fEffectChargeTime)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START_OUTLIST(TEXT("PerpectShotCharge"), &desc, m_ChargeEffects);
		m_fEffectChargeAcc = 0.0f;
	}

	m_fEffectCharge2Acc += fTimeDelta;

	if (m_fEffectCharge2Acc >= m_fEffectCharge2Time)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START_OUTLIST(TEXT("PerpectShotCharge2"), &desc, m_ChargeEffects);
		m_fEffectCharge2Acc = 0.0f;
	}
}

CState_GN_PerfectShot_Loop* CState_GN_PerfectShot_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_PerfectShot_Loop* pInstance = new CState_GN_PerfectShot_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_PerfectShot_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_PerfectShot_Loop::Free()
{
	__super::Free();
}
