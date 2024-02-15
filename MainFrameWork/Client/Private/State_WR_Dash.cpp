#include "stdafx.h"
#include "..\Public\State_WR_Dash.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Dash::CState_WR_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Dash::Initialize()
{
	m_iDash_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_dash", 1.2f);
	if (m_iDash_Normal == -1)
		return E_FAIL;

	m_iDash_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_dash_identity1", 1.2f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Dash::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_Dash_293.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_Dash::Enter_State()
{
	m_iSoundCnt = 0;

	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_iDash_Identity, 0.2f, 0, 0, 1.2f);
		m_iDash = m_iDash_Identity;
	}
	else
	{
		m_pPlayer->Reserve_Animation(m_iDash_Normal, 0.2f, 0, 0);
		m_iDash = m_iDash_Normal;
	}

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
}

void CState_WR_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Dash::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	m_pController->Get_DashEndMessage(3.f);
}

void CState_WR_Dash::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash);

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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash);

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

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Dash* CState_WR_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Dash* pInstance = new CState_WR_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Dash::Free()
{
	__super::Free();
}
