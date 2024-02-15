#include "stdafx.h"
#include "..\Public\State_WDR_FullSwing_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"
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


	m_fEffectAcc = 0.1f;
	m_fEffectDelay = 0.1f;

	m_fCamShakeAcc = 0.0f;
	m_fCamShakeDelay = 0.3f;

	if (m_pPlayer->Is_Control())
	{
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
	}

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_67.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WDR_FullSwing_Loop::Enter_State()
{

	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);


	m_fSkillTimeAcc = 0;
	m_pPlayer->Reserve_Animation(m_iFullSwing_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_Effects.clear();

	m_fEffectAcc = m_fEffectDelay;
	m_fCamShakeAcc = m_fCamShakeDelay;

	if(nullptr != m_pHoldingUI)
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
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	CSound_Manager::GetInstance()->Stop_Channel_Sound(L"WDR_67.wav");

}

void CState_WDR_FullSwing_Loop::Tick_State_Control(_float fTimeDelta)
{
	Update_Effect(fTimeDelta);

	if (m_fCamShakeAcc >= m_fCamShakeDelay)
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.02f, 80.0f, 1.0f, 5.0f);
		m_fCamShakeAcc = 0.0f;
	}
	else
		m_fCamShakeAcc += fTimeDelta;

	m_fSkillTimeAcc += fTimeDelta;
	if (nullptr != m_pHoldingUI)
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

	Update_Effect(fTimeDelta);
}


void CState_WDR_FullSwing_Loop::Update_Effect(_float fTimeDelta)
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
