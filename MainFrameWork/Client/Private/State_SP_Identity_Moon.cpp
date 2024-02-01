#include "stdafx.h"
#include "..\Public\State_SP_Identity_Moon.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "ServerSessionManager.h"

CState_SP_Identity_Moon::CState_SP_Identity_Moon(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Identity_Moon::Initialize()
{
	m_iIdentity_Moon_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_harmonyofyin_01", 1.0f);
	if (m_iIdentity_Moon_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Identity_Moon::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Identity_Moon::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Identity_Moon::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iIdentity_Moon_Start, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SP_IdentityMessage();
	m_pPlayer->Set_SuperArmorState(true);

	static_cast<CController_SP*>(m_pController)->Set_Attack_Desc(2);
}

void CState_SP_Identity_Moon::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Identity_Moon::Exit_State()
{
	m_pPlayer->Set_SuperArmorState(false);
}

void CState_SP_Identity_Moon::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity_Moon_Start))
	{
		m_pPlayer->Set_State(TEXT("Identity_Moon_End"));
	}
}

void CState_SP_Identity_Moon::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Identity_Moon* CState_SP_Identity_Moon::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Identity_Moon* pInstance = new CState_SP_Identity_Moon(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Identity_Moon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Identity_Moon::Free()
{
	__super::Free();
}
