#include "stdafx.h"
#include "..\Public\State_WR_Guillotine_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "Model.h"

CState_WR_Guillotine_Loop::CState_WR_Guillotine_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Guillotine_Loop::Initialize()
{
	m_iGuillotine_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_guillotine_02", 1.f);
	if (m_iGuillotine_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Guillotine_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Guillotine_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(6);
	m_SkillFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Hot_346.wav")));
	m_SoundFrames.push_back(SOUNDDESC(3, TEXT("Effect"), TEXT("WR_Gill_150.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_Guillotine_Loop::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;
	m_bEffectStart = false;

	for(_int i = 0; i < 2; ++i)
		m_bCameraStart[i] = false;

	m_pPlayer->Reserve_Animation(m_iGuillotine_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	if (m_pPlayer->Is_Control())
		Init_Camera();
}

void CState_WR_Guillotine_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Guillotine_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (m_pPlayer->Is_Control())
		Reset_Camera();

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WR_Guillotine_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Loop);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGuillotine_Loop))
		m_pPlayer->Set_State(TEXT("Skill_WR_Guillotine_End"));

	if (false == m_bEffectStart && 4 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_Guillotine_Slash"), &desc)

		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 60.0f, 0.8f, 5.0f);

		m_bEffectStart = true;
	}

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	Update_Camera(iAnimFrame, fTimeDelta);
}

void CState_WR_Guillotine_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Loop);

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

void CState_WR_Guillotine_Loop::Init_Camera()
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_pPlayer->Get_TargetPos() - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vRight = Vec3::Up.Cross(vLook);
	vRight.Normalize();

	m_vCameraTargetPos = vPos + vLook * 1.f;

	Vec3 vTargetPos = vPos + vLook * 1.5f + Vec3::Up * 0.87f;

	Vec3 vOffset = vLook * 5.0f + Vec3::Up * 0.87f;
	vOffset.Normalize();

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
	pCamera->Set_TargetPos(vTargetPos);
	pCamera->Set_Offset(vOffset);
	pCamera->Set_CameraLength(1.25f);
}

void CState_WR_Guillotine_Loop::Update_Camera(_uint iAnimFrame, _float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	if (false == m_bCameraStart[0] && 2 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Loop))
	{
		pCamera->ZoomInOut(6.7f, 9.0f);

		m_bCameraStart[0] = true;
	}

	if (false == m_bCameraStart[1] && 3 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Loop))
	{
		pCamera->Cam_Shake(0.05f, 100.f, 0.1f, 5.5f);
		pCamera->Set_RadialBlur(0.055f, m_vCameraTargetPos, 0.15f, 0.1f);
		pCamera->Set_Chromatic(0.08f, m_vCameraTargetPos, 0.15f, 0.12f);

		m_bCameraStart[1] = true;
	}

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_vCameraTargetPos - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();

	pCamera->Set_Offset(vLook * 5.0f + Vec3::Up * 1.f);
	pCamera->Set_TargetPos(vPos + vLook * 1.5f + Vec3::Up * 0.85f); // 여기서 심어주면 다음 프레임 시작때 볼 위치가 되는 듯
}

void CState_WR_Guillotine_Loop::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

CState_WR_Guillotine_Loop* CState_WR_Guillotine_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Guillotine_Loop* pInstance = new CState_WR_Guillotine_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Guillotine_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Guillotine_Loop::Free()
{
	__super::Free();
}
