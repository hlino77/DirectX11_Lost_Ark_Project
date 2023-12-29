#include "stdafx.h"
#include "..\Public\State_WDR_FullSwing_Start.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_FullSwing_Start::CState_WDR_FullSwing_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_FullSwing_Start::Initialize()
{
	m_iFullSwing_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_fullswing_start", 1.2f);
	if (m_iFullSwing_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_FullSwing_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_FullSwing_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_FullSwing_Start::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iFullSwing_Start, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_UseMarbleMessage();
}

void CState_WDR_FullSwing_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_FullSwing_Start::Exit_State()
{
}

void CState_WDR_FullSwing_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFullSwing_Start))
		m_pPlayer->Set_State(TEXT("Skill_WDR_FullSwing_Loop"));
}

void CState_WDR_FullSwing_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_FullSwing_Start* CState_WDR_FullSwing_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_FullSwing_Start* pInstance = new CState_WDR_FullSwing_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_FullSwing_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_FullSwing_Start::Free()
{
	__super::Free();
}
