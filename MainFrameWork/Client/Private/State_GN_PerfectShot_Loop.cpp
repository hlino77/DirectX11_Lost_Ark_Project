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
#include "UI_HoldingFrame.h"

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

	m_fSkillEndTime = 2.f;

	m_fSkillSuccessTime_Min = 1.5f;
	m_fSkillSuccessTime_Max = 1.9f;

	m_fEffectChargeTime = 0.23f;

	m_fEffectCharge2Time = 0.4f;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("GN_Perfect_189.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	if (m_pPlayer->Is_Control())
	{
		CUI_HoldingFrame::HOLDING_SKILL_DESC HoldingDesc;
		HoldingDesc.strSkillName = TEXT("∆€∆Â∆Æ º¶");
		HoldingDesc.fSkillTimeAcc = m_fSkillTimeAcc;
		HoldingDesc.fSkillEndTime = m_fSkillEndTime;
		HoldingDesc.fSkillSuccessTime_Min = m_fSkillSuccessTime_Min;
		HoldingDesc.fSkillSuccessTime_Max = m_fSkillSuccessTime_Max;
		m_pHoldingUI = static_cast<CUI_HoldingFrame*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC,
			_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Skill_HoldingGauge"), &HoldingDesc));
		if (nullptr == m_pHoldingUI)
			return E_FAIL;
	}

	return S_OK;
}

void CState_GN_PerfectShot_Loop::Enter_State()
{
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPerfectShot_Loop, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;

	m_fEffectChargeAcc = 0.4f;
	m_fEffectCharge2Acc = 0.3f;
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(true);
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

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(false);

	StopStateSound();
}

void CState_GN_PerfectShot_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPerfectShot_Loop);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	m_fSkillTimeAcc += fTimeDelta;
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillTimeAcc(m_fSkillTimeAcc);
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

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPerfectShot_Loop);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
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
	m_pHoldingUI->Set_Dead(true);
}
