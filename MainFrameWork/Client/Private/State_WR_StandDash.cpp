#include "stdafx.h"
#include "..\Public\State_WR_StandDash.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_StandDash::CState_WR_StandDash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_StandDash::Initialize()
{
	m_iStandDahs_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_standup_1", 1.f);
	if (m_iStandDahs_Normal == -1)
		return E_FAIL;

	m_iStandDahs_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_standup_identity1", 1.f);
	if (m_iStandDahs_Identity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_StandDash::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_StandDash::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_StandDash_295.wav")));
	m_SoundFrames.push_back(SOUNDDESC(5, TEXT("Effect"), TEXT("WR_Tat_303.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_StandDash::Enter_State()
{
	m_iSoundCnt = 0;

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();


	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_iStandDahs_Identity, 0.2f, 0, 0, 1.2f);
		m_iStandDash = m_iStandDahs_Identity;
	}
	else if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_iStandDahs_Normal, 0.2f, 0, 0, 1.2f);
		m_iStandDash = m_iStandDahs_Normal;
	}

}

void CState_WR_StandDash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_StandDash::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);
}

void CState_WR_StandDash::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iStandDash);

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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iStandDash))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_StandDash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_StandDash* CState_WR_StandDash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_StandDash* pInstance = new CState_WR_StandDash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_StandDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_StandDash::Free()
{
	__super::Free();
}
