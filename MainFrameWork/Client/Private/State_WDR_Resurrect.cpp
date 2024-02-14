#include "stdafx.h"
#include "..\Public\State_WDR_Resurrect.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "Renderer.h"

CState_WDR_Resurrect::CState_WDR_Resurrect(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Resurrect::Initialize()
{
	m_iResurrect = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"resurrect_1", 1.0f);
	if (m_iResurrect == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Resurrect::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Resurrect::Tick_State_NoneControl;


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_247.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WDR_Resurrect::Enter_State()
{
	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

	}


	m_pPlayer->Reserve_Animation(m_iResurrect, 0.2f, 0, 0);

	m_pPlayer->Set_Invincible(true);

	if (true == m_pPlayer->Is_Control())
		m_pPlayer->Get_RendererCom()->Set_DeadScene(false);

	CGameObject::STATDESC tPcStat = m_pPlayer->Get_PlayerStat_Desc();
	tPcStat.iCurHp = tPcStat.iMaxHp;
	tPcStat.iCurMp = tPcStat.iMaxMp;
	m_pPlayer->Set_PlayerStat_Desc(tPcStat);
}

void CState_WDR_Resurrect::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Resurrect::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
	m_pController->Get_DeadEndMessage();
}

void CState_WDR_Resurrect::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iResurrect))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WDR_Resurrect::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Resurrect* CState_WDR_Resurrect::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Resurrect* pInstance = new CState_WDR_Resurrect(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Resurrect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Resurrect::Free()
{
	__super::Free();
}
