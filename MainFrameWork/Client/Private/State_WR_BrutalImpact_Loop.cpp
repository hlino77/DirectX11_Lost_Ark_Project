#include "stdafx.h"
#include "..\Public\State_WR_BrutalImpact_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "UI_HoldingFrame.h"

CState_WR_BrutalImpact_Loop::CState_WR_BrutalImpact_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_BrutalImpact_Loop::Initialize()
{
	m_iBrutalImpact_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_brutalimpact_02", 1.f);
	if (m_iBrutalImpact_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_BrutalImpact_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_BrutalImpact_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 1.5f;

	m_fSkillSuccessTime_Max = 1.5f;
	m_fSkillSuccessTime_Min = 1.0f;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_loudU_394.wav")));
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_Brutal_30.wav")));
	m_SoundFrames.push_back(SOUNDDESC());


	if (m_pPlayer->Is_Control())
	{
		CUI_HoldingFrame::HOLDING_SKILL_DESC HoldingDesc;
		HoldingDesc.strSkillName = TEXT("∫Í∑Á≈ª ¿”∆—∆Æ");
		HoldingDesc.fSkillTimeAcc = m_fSkillTimeAcc;
		HoldingDesc.fSkillEndTime = m_fSkillEndTime;
		HoldingDesc.fSkillSuccessTime_Min = 0.f;
		HoldingDesc.fSkillSuccessTime_Max = 0.f;
		m_pHoldingUI = static_cast<CUI_HoldingFrame*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC,
			_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Skill_HoldingGauge"), &HoldingDesc));
		if (nullptr == m_pHoldingUI)
			return E_FAIL;
	}


	return S_OK;
}

void CState_WR_BrutalImpact_Loop::Enter_State()
{
	m_iSoundCnt = 0;

	for(_int i = 0; i < 3; ++i)
		m_bEffectStart[i] = false;

	m_pPlayer->Reserve_Animation(m_iBrutalImpact_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_fSkillTimeAcc = 0.f;
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(true);
}

void CState_WR_BrutalImpact_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_BrutalImpact_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(false);

	StopStateSound();
}

void CState_WR_BrutalImpact_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Loop);

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
	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
		m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_fSkillSuccessTime_Max >= m_fSkillTimeAcc)
		{
			m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
			m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_End"));
		}
	}

	Effect_BrutalImpact_Loop_Aura_Impact2();
	Effect_BrutalImpact_Loop_Impact3_Control();

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);
}

void CState_WR_BrutalImpact_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Loop, 1.f);

	Effect_BrutalImpact_Loop_Aura_Impact2();
	Effect_BrutalImpact_Loop_Impact3_NonControl();

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_BrutalImpact_Loop::Effect_BrutalImpact_Loop_Aura_Impact2()
{
	_int iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Loop);

	if (false == m_bEffectStart[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Aura"), &desc)
			m_bEffectStart[0] = true;
	}

	if (false == m_bEffectStart[1] && 4 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Impact2"), &desc)

			m_bEffectStart[1] = true;
	}
}

void CState_WR_BrutalImpact_Loop::Effect_BrutalImpact_Loop_Impact3_NonControl()
{
	if (false == m_bEffectStart[2] && 23 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Loop))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Impact3"), &desc)
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Lightning"), &desc)

		m_bEffectStart[2] = true;
	}
}

void CState_WR_BrutalImpact_Loop::Effect_BrutalImpact_Loop_Impact3_Control()
{
	if (false == m_bEffectStart[2] && 23 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Loop))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Impact3"), &desc)
		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Lightning"), &desc)

		m_pPlayer->Get_Camera()->Set_RadialBlur(0.05f, matPivot.Translation(), 0.1f, 0.08f);

		m_bEffectStart[2] = true;
	}
}

CState_WR_BrutalImpact_Loop* CState_WR_BrutalImpact_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_BrutalImpact_Loop* pInstance = new CState_WR_BrutalImpact_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_BrutalImpact_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_BrutalImpact_Loop::Free()
{
	__super::Free();
	m_pHoldingUI->Set_Dead(true);
}
