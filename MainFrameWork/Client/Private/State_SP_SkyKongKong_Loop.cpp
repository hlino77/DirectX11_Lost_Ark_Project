#include "stdafx.h"
#include "..\Public\State_SP_SkyKongKong_Loop.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_SP_SkyKongKong_Loop::CState_SP_SkyKongKong_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_SkyKongKong_Loop::Initialize()
{
	m_iSkyKongKong_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_skykongkong_02", 1.0f);
	if (m_iSkyKongKong_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_SkyKongKong_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_SkyKongKong_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(13);
	m_SkillFrames.push_back(-1);

	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_282.wav"))); // Player
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_183.wav"))); // Skill

	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_SP_SkyKongKong_Loop::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt+1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);

	}


	m_iSkillCnt = 0;
	m_bComboContinue = false;

	m_pPlayer->Reserve_Animation(m_iSkyKongKong_Loop, 0.1f, 0, 0, 1.f);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());


	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_SP_SkyKongKong_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_SkyKongKong_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState() || m_pPlayer->Get_ServerState() != TEXT("Skill_SP_SkyKongKong_Loop"))
	{
		TrailEnd();
		m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
		StopStateSound();
	}

}

void CState_SP_SkyKongKong_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Loop);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_SP_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		5 <= iAnimFrame &&
		15 > iAnimFrame &&
		1 > m_iContinueCnt &&
		false == m_bComboContinue)
	{
		m_iContinueCnt++;
		m_bComboContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSkyKongKong_Loop) && true == m_bComboContinue)
	{
		m_bComboContinue = false;

		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_SP_SkyKongKong_Loop"));
	}
	else if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSkyKongKong_Loop) && false == m_bComboContinue)
	{
		m_iContinueCnt = 0;

		m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);

		m_pPlayer->Set_State(TEXT("Skill_SP_SkyKongKong_End"));
	}
		
}

void CState_SP_SkyKongKong_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Loop);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
	}
}

void CState_SP_SkyKongKong_Loop::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"SkyKongKongShot", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.2f, 10.0f);
	}
}

void CState_SP_SkyKongKong_Loop::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"SkyKongKongTrail", 5.0f);
}



CState_SP_SkyKongKong_Loop* CState_SP_SkyKongKong_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_SkyKongKong_Loop* pInstance = new CState_SP_SkyKongKong_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_SkyKongKong_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_SkyKongKong_Loop::Free()
{
	__super::Free();
}
