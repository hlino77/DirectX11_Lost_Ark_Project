#include "stdafx.h"
#include "..\Public\State_WR_Resurrect.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "Renderer.h"

CState_WR_Resurrect::CState_WR_Resurrect(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Resurrect::Initialize()
{
	m_iResurrect = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"resurrect_1", 1.0f);
	if (m_iResurrect == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Resurrect::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Resurrect::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_Resurrect_401.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_Resurrect::Enter_State()
{
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iResurrect, 0.2f, 0, 0);

	m_pPlayer->Set_Invincible(true);

	if (true == m_pPlayer->Is_Control())
		m_pPlayer->Get_RendererCom()->Set_DeadScene(false);

	CGameObject::STATDESC tPcStat = m_pPlayer->Get_PlayerStat_Desc();
	tPcStat.iCurHp = tPcStat.iMaxHp;
	tPcStat.iCurMp = tPcStat.iMaxMp;
	m_pPlayer->Set_PlayerStat_Desc(tPcStat);
}

void CState_WR_Resurrect::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Resurrect::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	m_pPlayer->Set_Invincible(false);
	m_pController->Get_DeadEndMessage();
}

void CState_WR_Resurrect::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iResurrect);

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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iResurrect))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Resurrect::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Resurrect* CState_WR_Resurrect::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Resurrect* pInstance = new CState_WR_Resurrect(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Resurrect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Resurrect::Free()
{
	__super::Free();
}
