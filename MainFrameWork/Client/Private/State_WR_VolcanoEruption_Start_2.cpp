#include "stdafx.h"
#include "..\Public\State_WR_VolcanoEruption_Start_2.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_VolcanoEruption_Start_2::CState_WR_VolcanoEruption_Start_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_VolcanoEruption_Start_2::Initialize()
{
	m_iVolcano_Start_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hell_blades_start_custom_3", 1.f);
	if (m_iVolcano_Start_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_VolcanoEruption_Start_2::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_VolcanoEruption_Start_2::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_VolcanoEruption_Start_2::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iVolcano_Start_2, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_2, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_2, 1.f);

}

void CState_WR_VolcanoEruption_Start_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_VolcanoEruption_Start_2::Exit_State()
{
}

void CState_WR_VolcanoEruption_Start_2::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iVolcano_Start_2))
		m_pPlayer->Set_State(TEXT("Skill_WR_VolcanoEruption_Loop"));

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_2, 1.f);
}

void CState_WR_VolcanoEruption_Start_2::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_2, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_VolcanoEruption_Start_2* CState_WR_VolcanoEruption_Start_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_VolcanoEruption_Start_2* pInstance = new CState_WR_VolcanoEruption_Start_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_VolcanoEruption_Start_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_VolcanoEruption_Start_2::Free()
{
	__super::Free();
}
