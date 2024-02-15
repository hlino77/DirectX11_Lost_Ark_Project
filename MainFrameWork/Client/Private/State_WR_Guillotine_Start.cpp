#include "stdafx.h"
#include "..\Public\State_WR_Guillotine_Start.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Model.h"

CState_WR_Guillotine_Start::CState_WR_Guillotine_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Guillotine_Start::Initialize()
{
	m_iGuillotine_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_guillotine_01", 1.f);
	if (m_iGuillotine_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Guillotine_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Guillotine_Start::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(2, TEXT("Effect"), TEXT("WR_Huut_347.wav")));
	m_SoundFrames.push_back(SOUNDDESC(2, TEXT("Effect"), TEXT("WR_Gill_146.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_Guillotine_Start::Enter_State()
{
	m_bEffectStart = false;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iGuillotine_Start, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);

	m_pPlayer->Get_WR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_WR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	if (m_pPlayer->Is_Control())
		Init_Camera();
}

void CState_WR_Guillotine_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Guillotine_Start::Exit_State()
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

void CState_WR_Guillotine_Start::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Start);

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
	
	Effect_Guillotine_Charge();

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGuillotine_Start))
		m_pPlayer->Set_State(TEXT("Skill_WR_Guillotine_Loop"));


	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);

	Update_Camera(iAnimFrame, fTimeDelta);
}

void CState_WR_Guillotine_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);

	Effect_Guillotine_Charge();

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_Guillotine_Start::Effect_Guillotine_Charge()
{
	if (false == m_bEffectStart && 4 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Start))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_Guillotine_Charge"), &desc);

		if(m_pPlayer->Is_Control())
			m_pPlayer->Get_Camera()->Set_RadialBlur(0.05f, matPivot.Translation(), 1.f, 0.05f);

		m_bEffectStart = true;
	}
}

void CState_WR_Guillotine_Start::Init_Camera()
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_pPlayer->Get_TargetPos() - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vRight = Vec3::Up.Cross(vLook);
	vRight.Normalize();

	m_vCameraTargetPos = vPos + vLook * 1.f;

	Vec3 vTargetPos = vPos;
	vTargetPos.y += 0.7f;

	Vec3 vOffset = vLook * -3.f + vRight * 2.1f; // 초기 위치 인듯

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
	pCamera->Set_TargetPos(vTargetPos);		// 시작시점에 보는 위치 인듯
	pCamera->Set_Offset(vOffset);
	pCamera->Set_CameraLength(1.25f);

	//pCamera->ZoomInOut(1.5f, 2.0f);
}

void CState_WR_Guillotine_Start::Update_Camera(_uint iAnimFrame, _float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_vCameraTargetPos - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vRight = Vec3::Up.Cross(vLook);
	vRight.Normalize();

	Vec3 vTargetOffset = vLook * 5.0f + Vec3::Up * 0.87f; // 카메라가 도착할 최종 목표 위치 인듯
	vTargetOffset.Normalize();
		
	Vec3 vOffset = pCamera->Get_Offset();
	vOffset = Vec3::Lerp(vOffset, vTargetOffset, 6.f * fTimeDelta);
	pCamera->Set_Offset(vOffset);

	Vec3 vTargetPos = vPos + vLook * 1.5f + Vec3::Up * 0.87f;
	Vec3 vCameraTargetPos = pCamera->Get_TargetPos();
	vCameraTargetPos = Vec3::Lerp(vCameraTargetPos, vTargetPos, 3.0f * fTimeDelta);
	pCamera->Set_TargetPos(vCameraTargetPos); // 여기서 심어주면 다음 프레임 시작때 볼 위치가 되는 듯
}

void CState_WR_Guillotine_Start::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

CState_WR_Guillotine_Start* CState_WR_Guillotine_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Guillotine_Start* pInstance = new CState_WR_Guillotine_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Guillotine_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Guillotine_Start::Free()
{
	__super::Free();
}
