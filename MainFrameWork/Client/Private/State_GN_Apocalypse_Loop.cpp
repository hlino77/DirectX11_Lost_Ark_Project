#include "stdafx.h"
#include "..\Public\State_GN_Apocalypse_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_Apocalypse_Loop::CState_GN_Apocalypse_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Apocalypse_Loop::Initialize()
{
	m_iApocalypse_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_apocalyspe_loop", 1.2f);
	if (m_iApocalypse_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Apocalypse_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Apocalypse_Loop::Tick_State_NoneControl;

	m_fSkillEndTime = 2.f;

	return S_OK;
}

void CState_GN_Apocalypse_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iApocalypse_Loop, 0.1f, 0, 0);
}

void CState_GN_Apocalypse_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Apocalypse_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;
}

void CState_GN_Apocalypse_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_Apocalypse_Success"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_Apocalypse_Success"));
	}
}

void CState_GN_Apocalypse_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Apocalypse_Loop* CState_GN_Apocalypse_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Apocalypse_Loop* pInstance = new CState_GN_Apocalypse_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Apocalypse_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Apocalypse_Loop::Free()
{
	__super::Free();
}
