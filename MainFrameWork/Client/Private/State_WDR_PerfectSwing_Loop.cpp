#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"
#include "GameInstance.h"
#include "UI_HoldingFrame.h"


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

	m_fCamShakeAcc = 0.0f;
	m_fCamShakeDelay = 0.12f;

	if (m_pPlayer->Is_Control())
	{
		CUI_HoldingFrame::HOLDING_SKILL_DESC HoldingDesc;
		HoldingDesc.strSkillName = TEXT("∆€∆Â∆Æ Ω∫¿Æ");
		HoldingDesc.fSkillTimeAcc = m_fSkillTimeAcc;
		HoldingDesc.fSkillEndTime = m_fSkillEndTime * 0.5f;
		HoldingDesc.fSkillSuccessTime_Min = 0.f;
		HoldingDesc.fSkillSuccessTime_Max = 0.f;
		m_pHoldingUI = static_cast<CUI_HoldingFrame*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC,
			_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Skill_HoldingGauge"), &HoldingDesc));
		if (nullptr == m_pHoldingUI)
			return E_FAIL;
	}


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_84.wav"))); //  Skill  
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_88.wav"))); //  Skill  
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WDR_PerfectSwing_Loop::Enter_State()
{
	m_EffectSound = false;



	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);



	m_fSkillTimeAcc = 0;
	m_fUI_AccTime = 0.f;
	m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_1;
	m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;

	m_Effects.clear();

	m_fEffectAcc = m_fEffectDelay;

	m_eCameraState = CameraState::CHARGE1;

	m_fEffectAcc = m_fEffectDelay;
	m_fCamShakeAcc = m_fCamShakeDelay;

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Set_MotionBlur(1.0f, 0.05f);
	}
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(true);
}

void CState_WDR_PerfectSwing_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Loop::Exit_State()
{
	m_pPlayer->Delete_Effect(L"PerfectParticle");

	if (true == m_pPlayer->Is_CancelState())
	{
		m_pPlayer->Delete_Effect(L"PerfectCircle");
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(false);


	
}

void CState_WDR_PerfectSwing_Loop::Tick_State_Control(_float fTimeDelta)
{

	if (true == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt].strName) && m_eCameraState == CameraState::CHARGE2)
	{
		if (false == m_EffectSound)
		{
		
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);

			m_EffectSound = true;
		}
	}


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

	if (m_pPlayer->Get_Camera()->Get_Mode() == CCamera_Player::CameraState::FREE)
		Update_Camera_Charge(fTimeDelta);

	m_fUI_AccTime += fTimeDelta;
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillTimeAcc(m_fUI_AccTime);

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
		m_eCameraState = CameraState::CHARGE2;
		m_pPlayer->Get_Camera()->Set_MotionBlur(1.0f, 0.05f);
		m_fUI_AccTime = 0.f;
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

	if (true == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt].strName) && m_eCameraState == CameraState::CHARGE2)
	{
		if (false == m_EffectSound)
		{

			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);

			m_EffectSound = true;
		}
	}


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

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

	if (m_fSkillSuccessTime_Min <= m_fSkillTimeAcc && m_iPerfectSwing_Loop == m_iPerfectSwing_Loop_1)
	{
		m_iPerfectSwing_Loop = m_iPerfectSwing_Loop_2;
		m_pPlayer->Reserve_Animation(m_iPerfectSwing_Loop, 0.1f, 0, 0);
		m_eCameraState = CameraState::CHARGE2;
	}
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

void CState_WDR_PerfectSwing_Loop::Update_Camera_Charge(_float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();
	
	Vec3 vLook = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);

	Vec3 vTargetOffset;
	_float fCameraLength = 0.f;

	if (m_eCameraState == CameraState::CHARGE1)
	{
		vTargetOffset = vLook + vRight * 0.5f;
		fCameraLength = 1.5f;
	}
	else if (m_eCameraState == CameraState::CHARGE2)
	{
		vTargetOffset = vRight + vLook * -0.1f;
		fCameraLength = 1.0f;
	}
	vTargetOffset.Normalize();
	
	Vec3 vOffset = pCamera->Get_Offset();
	vOffset = Vec3::Lerp(vOffset, vTargetOffset, 5.0f * fTimeDelta);
	pCamera->Set_Offset(vOffset);
	pCamera->ZoomInOut(fCameraLength, 5.0f);

	if (m_fCamShakeAcc >= m_fCamShakeDelay)
	{
		if (m_eCameraState == CameraState::CHARGE1)
		{	
			m_pPlayer->Get_Camera()->Cam_Shake(0.01f, 200.0f, 0.2f, 10.0f);
		}
		else if (m_eCameraState == CameraState::CHARGE2)
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.02f, 200.0f, 0.2f, 10.0f);
		}	
		m_fCamShakeAcc = 0.0f;
	}
	else
		m_fCamShakeAcc += fTimeDelta;
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
	m_pHoldingUI->Set_Dead(true);
}
