#include "stdafx.h"
#include "..\Public\State_SP_Dash.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"

CState_SP_Dash::CState_SP_Dash(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Dash::Initialize()
{
	m_iDash = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_spacing", 1.2f);
	if (m_iDash == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Dash::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Dash::Tick_State_NoneControl;

	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_142.wav"))); // Skill Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_224.wav"), 0.3f)); // Player Sound 
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_SP_Dash::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iDash, 0.1f, 0, 0, 1.2f);

	m_pController->Get_StopMessage();
	m_pController->Get_DashMessage(m_pPlayer->Get_TargetPos());
	m_pController->Get_SkillEndMessage();



	// Skill Sound 
	CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

	// Player Sound
	CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);


}

void CState_SP_Dash::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Dash::Exit_State()
{
	m_pController->Get_DashEndMessage(3.f);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_SP_Dash::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDash))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}

	if (true == m_pController->Is_Run() && 30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDash))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_SP_Dash::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Dash* CState_SP_Dash::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Dash* pInstance = new CState_SP_Dash(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Dash::Free()
{
	__super::Free();
}
