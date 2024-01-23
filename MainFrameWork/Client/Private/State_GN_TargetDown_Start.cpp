#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Effect_Custom_CrossHair.h"
#include "Camera_Player.h"


CState_GN_TargetDown_Start::CState_GN_TargetDown_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Start::Initialize()
{
	m_iTargetDown_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_01", 0.5f);
	if (m_iTargetDown_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_TargetDown_Start::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iTargetDown_Start, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::LONG, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	if (m_pPlayer->Is_Control())
	{
		Init_CameraOffset();
	}
}

void CState_GN_TargetDown_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Start::Exit_State()
{
	if (true == m_pController->Is_HitState())
	{
		Reset_Camera();
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_TargetDown_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iTargetDown_Start))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Ready"));
	}

	Update_Camera(fTimeDelta);
}

void CState_GN_TargetDown_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_Start::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

void CState_GN_TargetDown_Start::Update_Camera(_float fTimeDelta)
{
	m_vCameraOffset = Vec3::Lerp(m_vCameraOffset, m_vTargetOffset, 2.0f * fTimeDelta);
	m_pPlayer->Get_Camera()->Set_Offset(m_vCameraOffset);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iTargetDown_Start);

	if (iAnimFrame > 25)
	{
		m_pPlayer->Get_Camera()->ZoomInOut(1.0f, 15.0f);
	}
}

void CState_GN_TargetDown_Start::Init_CameraOffset()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::FREE);
	
	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	Vec3 vLook = m_pPlayer->Get_TargetPos() - vPos;
	vLook.Normalize();
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();

	Vec3 vTargetPos = vPos;
	vTargetPos.y += 0.5f;

	vTargetPos += vRight * -0.05f;

	m_vCameraOffset = vLook * 1.0f + vRight * 1.0f + vUp * 0.5f;
	m_vCameraOffset.Normalize();

	m_vTargetOffset = vLook * 1.0f;
	m_vTargetOffset.Normalize();

	m_pPlayer->Get_Camera()->Set_TargetPos(vTargetPos);
	m_pPlayer->Get_Camera()->Set_Offset(m_vCameraOffset);
	m_pPlayer->Get_Camera()->Set_CameraLength(1.8f);
	m_pPlayer->Get_Camera()->ZoomInOut(0.001f, 1.0f);
}

CState_GN_TargetDown_Start* CState_GN_TargetDown_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Start* pInstance = new CState_GN_TargetDown_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Start::Free()
{
	__super::Free();
}
