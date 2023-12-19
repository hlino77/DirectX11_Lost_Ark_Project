#include "stdafx.h"
#include "..\Public\State_GN_FocusShot_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_FocusShot_Start::CState_GN_FocusShot_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_FocusShot_Start::Initialize()
{
	m_iFocusShot_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_01", 1.0f);
	if (m_iFocusShot_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_FocusShot_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_FocusShot_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_FocusShot_Start::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iFocusShot_Start, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::LONG, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_FocusShot_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_FocusShot_Start::Exit_State()
{
}

void CState_GN_FocusShot_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_Start))
		m_pPlayer->Set_State(TEXT("Skill_GN_FocusShot_Loop"));

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_GN_FocusShot_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_FocusShot_Start* CState_GN_FocusShot_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_FocusShot_Start* pInstance = new CState_GN_FocusShot_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_FocusShot_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_FocusShot_Start::Free()
{
	__super::Free();
}
