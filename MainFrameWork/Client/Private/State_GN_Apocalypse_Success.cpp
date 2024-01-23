#include "stdafx.h"
#include "..\Public\State_GN_Apocalypse_Success.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_Apocalypse_Success::CState_GN_Apocalypse_Success(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Apocalypse_Success::Initialize()
{
	m_iApocalypse_Success = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_apocalyspe_success", 1.2f);
	if (m_iApocalypse_Success == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Apocalypse_Success::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Apocalypse_Success::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Apocalypse_Success::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iApocalypse_Success, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_Apocalypse_Success::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Apocalypse_Success::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_Apocalypse_Success::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iApocalypse_Success))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Apocalypse_Success::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Apocalypse_Success* CState_GN_Apocalypse_Success::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Apocalypse_Success* pInstance = new CState_GN_Apocalypse_Success(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Apocalypse_Success");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Apocalypse_Success::Free()
{
	__super::Free();
}
