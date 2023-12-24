#include "stdafx.h"
#include "..\Public\State_GN_PerfectShot_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_PerfectShot_Loop::CState_GN_PerfectShot_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_PerfectShot_Loop::Initialize()
{
	m_iPerfectShot_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectshot_loop", 1.0f);
	if (m_iPerfectShot_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_PerfectShot_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_PerfectShot_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 4.f;

	return S_OK;
}

void CState_GN_PerfectShot_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iPerfectShot_Loop, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_LerpLookMessage(m_pPlayer->Get_TargetPos(), 8.f);
}

void CState_GN_PerfectShot_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_PerfectShot_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;
}

void CState_GN_PerfectShot_Loop::Tick_State_Control(_float fTimeDelta)
{
	m_fSkillTimeAcc += fTimeDelta;

	Vec3 vClickPos;
	if (m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pController->Get_LerpLookMessage(vClickPos, 5.f);
	}

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pPlayer->Set_State(TEXT("Skill_GN_PerfectShot_End"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pPlayer->Set_State(TEXT("Skill_GN_PerfectShot_End"));

		if (m_fSuccess_MinTime <= m_fSkillTimeAcc && m_fSuccess_MaxTime >= m_fSkillTimeAcc)
			m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Set_SkillSuccess(true);
	}
}

void CState_GN_PerfectShot_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_LerpLookMessage(m_pPlayer->Get_TargetPos(), 8.f);
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_PerfectShot_Loop* CState_GN_PerfectShot_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_PerfectShot_Loop* pInstance = new CState_GN_PerfectShot_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_PerfectShot_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_PerfectShot_Loop::Free()
{
	__super::Free();
}
