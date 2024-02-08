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

	return S_OK;
}

void CState_WR_Guillotine_Start::Enter_State()
{
	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iGuillotine_Start, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);

	m_pPlayer->Get_WR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_WR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_Guillotine_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Guillotine_Start::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_Guillotine_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGuillotine_Start))
		m_pPlayer->Set_State(TEXT("Skill_WR_Guillotine_Loop"));

	if (false == m_bEffectStart && 4 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Start))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_Guillotine_Charge"), &desc)

		m_pPlayer->Get_Camera()->Set_RadialBlur(0.05f, matPivot.Translation(), 1.f, 0.05f);

		m_bEffectStart = true;
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);
}

void CState_WR_Guillotine_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Start, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
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
