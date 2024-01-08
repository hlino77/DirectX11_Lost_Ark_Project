#include "stdafx.h"
#include "..\Public\State_WR_VolcanoEruption_Start_1.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_VolcanoEruption_Start_1::CState_WR_VolcanoEruption_Start_1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_VolcanoEruption_Start_1::Initialize()
{
	m_iVolcano_Start_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hell_blades_start_custom_2", 1.2f);
	if (m_iVolcano_Start_1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_VolcanoEruption_Start_1::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_VolcanoEruption_Start_1::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_VolcanoEruption_Start_1::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iVolcano_Start_1, 0.1f, 0, 0, 2.f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_1, 1.4f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_1, 1.2f);

}

void CState_WR_VolcanoEruption_Start_1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_VolcanoEruption_Start_1::Exit_State()
{
}

void CState_WR_VolcanoEruption_Start_1::Tick_State_Control(_float fTimeDelta)
{
	m_pController->Get_RootMessage();

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iVolcano_Start_1))
		m_pPlayer->Set_State(TEXT("Skill_WR_VolcanoEruption_Start_2"));

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_1, 1.2f);
}

void CState_WR_VolcanoEruption_Start_1::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iVolcano_Start_1, 1.2f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_VolcanoEruption_Start_1* CState_WR_VolcanoEruption_Start_1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_VolcanoEruption_Start_1* pInstance = new CState_WR_VolcanoEruption_Start_1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_VolcanoEruption_Start_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_VolcanoEruption_Start_1::Free()
{
	__super::Free();
}
