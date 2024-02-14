#include "stdafx.h"
#include "..\Public\State_WDR_Identity.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect_Custom_WDIdenSpace.h"
#include "GameInstance.h"

CState_WDR_Identity::CState_WDR_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Identity::Initialize()
{
	m_Idle_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_gravity_1", 1.0f);
	if (m_Idle_Identity == -1)
		return E_FAIL;


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Identity::Tick_State_NoneControl;


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_279.wav"))); // Player
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_23.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WDR_Identity::Enter_State()
{
	m_EffectSound = false;


	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);
	}



	m_fTimeAcc = 0.f;

	m_pPlayer->Reserve_Animation(m_Idle_Identity, 0.1f, 0, 0);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
	static_cast<CController_WDR*>(m_pController)->Get_WDR_IdentityMessage();

	m_bEffect = false;
}

void CState_WDR_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Identity::Exit_State()
{
}

void CState_WDR_Identity::Tick_State_Control(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (0.5f <= m_fTimeAcc)
	{
		m_pPlayer->Set_State(TEXT("Iden_Idle"));
	}

	if (m_bEffect == false)
	{
		Effect_Start();
		m_bEffect = true;
	}
}

void CState_WDR_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WDR_Identity::Effect_Start()
{
	//Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	//CEffect_Manager::EFFECTPIVOTDESC tDesc;
	//tDesc.pPivotMatrix = &matWorld;

	//vector<CEffect*> Effects;
	//EFFECT_START_OUTLIST(L"WDIdentitySpace", &tDesc, Effects);
	//m_pPlayer->Add_Effect(L"WDIdentitySpace", Effects.front());

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CEffect_Custom_WDIdenSpace::CustomEffectDesc tDesc;
	tDesc.pOwner = m_pPlayer;
	CEffect_Custom_WDIdenSpace* pEffect = dynamic_cast<CEffect_Custom_WDIdenSpace*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER, L"Prototype_GameObject_Effect_Custom_WDIdenSpace", &tDesc));
	
	Safe_Release(pGameInstance);
}

CState_WDR_Identity* CState_WDR_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Identity* pInstance = new CState_WDR_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Identity::Free()
{
	__super::Free();
}
