#include "stdafx.h"
#include "..\Public\State_GN_Grenade.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_Grenade::CState_GN_Grenade(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Grenade::Initialize()
{
	m_iGrenade = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_at02grenade", 1.2f);
	if (m_iGrenade == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Grenade::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Grenade::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Grenade::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iGrenade, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_Grenade::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Grenade::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_Grenade::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGrenade))
		m_pPlayer->Set_State(TEXT("Idle"));

	/* 수류탄 던지는 프레임 10 */
}

void CState_GN_Grenade::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Grenade* CState_GN_Grenade::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Grenade* pInstance = new CState_GN_Grenade(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Grenade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Grenade::Free()
{
	__super::Free();
}
