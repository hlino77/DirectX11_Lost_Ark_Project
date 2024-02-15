#include "stdafx.h"
#include "..\Public\State_WR_Run.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "Esther.h"

CState_WR_Run::CState_WR_Run(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Run::Initialize()
{
	m_Run_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_battle_1", 1.0f);
	if (m_Run_Normal == -1)
		return E_FAIL;

	m_Run_Identity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"run_identity1", 1.0f);
	if (m_Run_Identity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Run::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Run::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("PC_Step_2.wav"), 0.3f, false));
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("PC_Step_253.wav"), 0.3f, false));
	m_SoundFrames.push_back(SOUNDDESC(11, TEXT("Effect"), TEXT("PC_Step_2.wav"), 0.3f, false));
	m_SoundFrames.push_back(SOUNDDESC(11, TEXT("Effect"), TEXT("PC_Step_253.wav"), 0.3f, false));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_Run::Enter_State()
{
	m_iSoundCnt = 0;

	m_pController->Get_MoveSpeedMessage(3.f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_Run_Identity, 0.2f, 0, 0, 1.8f);
		m_iRun = m_Run_Identity;
		m_pController->Get_MoveSpeedMessage(4.f);
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}
	else
	{
		m_pPlayer->Reserve_Animation(m_Run_Normal, 0.2f, 0, 0);
		m_iRun = m_Run_Normal;
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}

	m_pController->Get_SkillEndMessage();
}

void CState_WR_Run::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Run::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WR_Run::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iRun);

	if (0 >= iAnimFrame)
	{
		m_iSoundCnt = 0;
	}

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

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_EstherSkill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		switch (m_pController->Get_EstherType())
		{
		case (_uint)CEsther::ESTHERTYPE::SA:
			m_pPlayer->Set_State(TEXT("Esther_Silian"));
			break;
		case (_uint)CEsther::ESTHERTYPE::WY:
			m_pPlayer->Set_State(TEXT("Esther_Way"));
			break;
		case (_uint)CEsther::ESTHERTYPE::BT:
			m_pPlayer->Set_State(TEXT("Esther_Bahuntur"));
			break;
		}
	}
	else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
	{
		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("WR_Identity"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
	}
	else if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
		else
			m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			if (m_iRun == m_Run_Normal)
			{
				m_pController->Get_MoveSpeedMessage(3.f);
				m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
			}
			else if (m_iRun == m_Run_Identity)
			{
				m_pController->Get_MoveSpeedMessage(4.f);
				m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
			}
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pController->Is_Stop())
			m_pPlayer->Set_State(TEXT("Idle"));
	}

	if (m_iRun == m_Run_Identity && false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_Run_Normal, 0.2f, 0, 0);
		m_iRun = m_Run_Normal;
	}
}

void CState_WR_Run::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_MoveSpeedMessage(3.f);
	if (m_iRun == m_Run_Normal)
	{
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}
	else if (m_iRun == m_Run_Identity)
	{
		m_pController->Get_MoveSpeedMessage(4.f);
		m_pController->Get_MoveMessage(m_pPlayer->Get_TargetPos());
	}

	if (m_iRun == m_Run_Identity && false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
	{
		m_pPlayer->Reserve_Animation(m_Run_Normal, 0.2f, 0, 0);
		m_iRun = m_Run_Normal;
	}
	
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Run* CState_WR_Run::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Run* pInstance = new CState_WR_Run(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Run::Free()
{
	__super::Free();
}
