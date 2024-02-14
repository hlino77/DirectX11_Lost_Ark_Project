#include "stdafx.h"
#include "..\Public\State_WR_VolcanoEruption_Success.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Model.h"

CState_WR_VolcanoEruption_Success::CState_WR_VolcanoEruption_Success(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_VolcanoEruption_Success::Initialize()
{
	m_iVolcano_Success = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hell_blades_success", 1.2f);
	if (m_iVolcano_Success == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_VolcanoEruption_Success::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_VolcanoEruption_Success::Tick_State_NoneControl;

	m_SkillFrames.push_back(20);
	m_SkillFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_Vol_168.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_VolcanoEruption_Success::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iVolcano_Success, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Success, 1.4f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Success, 1.2f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_VolcanoEruption_Success::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_VolcanoEruption_Success::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_VolcanoEruption_Success::Tick_State_Control(_float fTimeDelta)
{
	_int iSkillFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iVolcano_Success);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iSkillFrame)
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

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] == iSkillFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (false == m_bEffectStart && 18 <= iSkillFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_VolcanoEruption_Explode"), &desc)

		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 80.0f, 1.0f, 5.0f);
		m_pPlayer->Get_Camera()->Set_Chromatic(0.5f, matPivot.Translation(), 0.1f, 0.1f);

		m_bEffectStart = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iVolcano_Success))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (45 <= iSkillFrame)
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
				m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
			else
				m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
		{
			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			{
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
		else if (true == m_pController->Is_Run())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Success, 1.2f);
}

void CState_WR_VolcanoEruption_Success::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Success, 1.2f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_VolcanoEruption_Success* CState_WR_VolcanoEruption_Success::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_VolcanoEruption_Success* pInstance = new CState_WR_VolcanoEruption_Success(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_VolcanoEruption_Success");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_VolcanoEruption_Success::Free()
{
	__super::Free();
}
