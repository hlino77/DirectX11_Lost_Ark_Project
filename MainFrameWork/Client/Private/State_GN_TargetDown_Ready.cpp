#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Ready.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Custom_CrossHair.h"
#include "GameInstance.h"
#include "Camera_Player.h"


CState_GN_TargetDown_Ready::CState_GN_TargetDown_Ready(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Ready::Initialize()
{
	m_iTargetDown_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_02", 1.0f);
	if (m_iTargetDown_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Ready::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Ready::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_TargetDown_Ready::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iTargetDown_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	//m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);

	m_fStartDelay = 0.3f;
	m_bZoomOut = false;
}

void CState_GN_TargetDown_Ready::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Ready::Exit_State()
{
	if (true == m_pController->Is_HitState())
	{
		Effect_End();
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_TargetDown_Ready::Tick_State_Control(_float fTimeDelta)
{
	if (m_fStartDelay > 0.0f)
	{
		m_fStartDelay -= fTimeDelta;
		return;
	}

	if (m_bZoomOut == false)
	{
		m_bZoomOut = true;
		Effect_ZoomOut();
	}

	if (m_pPlayer->Get_Camera()->Get_CameraLength() > 6.0f)
	{
		Effect_Start();
		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Loop"));
	}
}

void CState_GN_TargetDown_Ready::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_Ready::Effect_Start()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->ZoomInOut(12.0f, 3.0f);

	CEffect* pCrossHair = dynamic_cast<CEffect*>(CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_CrossHair"));

	m_pPlayer->Add_Effect(L"TargetDownCrossHair", pCrossHair);
}

void CState_GN_TargetDown_Ready::Effect_ZoomOut()
{
	m_pPlayer->Get_Camera()->ZoomInOut(12.0f, 3.0f);

	vector<CEffect*> EffectList;

	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix& matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	desc.pPivotMatrix = &matWorld;
	EFFECT_START_OUTLIST(TEXT("TargetDownDecal"), &desc, EffectList);

	CEffect* pDecal = EffectList.front();

	m_pPlayer->Add_Effect(L"TargetDownDecal", pDecal);
}

void CState_GN_TargetDown_Ready::Effect_End()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);

	m_pPlayer->Delete_Effect(L"TargetDownCrossHair");
	m_pPlayer->Delete_Effect(L"TargetDownDecal");
}

CState_GN_TargetDown_Ready* CState_GN_TargetDown_Ready::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Ready* pInstance = new CState_GN_TargetDown_Ready(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Ready::Free()
{
	__super::Free();
}
