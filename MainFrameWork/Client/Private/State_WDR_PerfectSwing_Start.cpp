#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Start.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Camera_Player.h"
#include "Renderer.h"

CState_WDR_PerfectSwing_Start::CState_WDR_PerfectSwing_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PerfectSwing_Start::Initialize()
{
	m_iPerfectSwing_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_start", 1.2f);
	if (m_iPerfectSwing_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PerfectSwing_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PerfectSwing_Start::Tick_State_NoneControl;


	return S_OK;
}

void CState_WDR_PerfectSwing_Start::Enter_State()
{

	m_pPlayer->Reserve_Animation(m_iPerfectSwing_Start, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_UseMarbleMessage();

	if (m_pPlayer->Is_Control())
	{
		Init_Camera();
	}
}

void CState_WDR_PerfectSwing_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Start::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WDR_PerfectSwing_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPerfectSwing_Start))
		m_pPlayer->Set_State(TEXT("Skill_WDR_PerfectSwing_Loop"));
}

void CState_WDR_PerfectSwing_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WDR_PerfectSwing_Start::Init_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::FREE);

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	Vec3 vLook = m_pPlayer->Get_TargetPos() - vPos;
	vLook.Normalize();
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();

	Vec3 vTargetPos = vPos;
	vTargetPos.y += 0.7f;

	vTargetPos += vRight * -0.05f;

	m_vCameraOffset = vLook * 1.0f + vUp * 0.5f;
	m_vCameraOffset.Normalize();

	m_pPlayer->Get_Camera()->Set_TargetPos(vTargetPos);
	m_pPlayer->Get_Camera()->Set_Offset(m_vCameraOffset);
	m_pPlayer->Get_Camera()->Set_CameraLength(2.8f);
	m_pPlayer->Get_Camera()->ZoomInOut(2.0f, 1.0f);

	CRenderer::Set_DOF_Range(150.f);
}

void CState_WDR_PerfectSwing_Start::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(5.0f);

	CRenderer::Set_DOF_Range(50.f);
}

CState_WDR_PerfectSwing_Start* CState_WDR_PerfectSwing_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PerfectSwing_Start* pInstance = new CState_WDR_PerfectSwing_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PerfectSwing_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PerfectSwing_Start::Free()
{
	__super::Free();
}
