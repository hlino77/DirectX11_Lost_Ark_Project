#include "stdafx.h"
#include "..\Public\State_MG_Identity.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"

CState_MG_Identity::CState_MG_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Identity::Initialize()
{
	m_iIdentity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_heavenserenade", 1.2f);
	if (m_iIdentity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Identity::Tick_State_NoneControl;

	/* 일반공격 프레임 */
	m_AttackFrames.push_back(20);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_MG_Identity::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_iIdentity, 0.1f, 0, 0, 1.2f);

	m_pController->Get_StopMessage();
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	static_cast<CController_MG*>(m_pController)->Get_MG_IdentityMessage();

	switch (static_cast<CController_MG*>(m_pController)->Get_MG_Gage())
	{
	case 1:
		static_cast<CController_MG*>(m_pController)->Set_Attack_Desc(1);
		break;
	case 2:
		static_cast<CController_MG*>(m_pController)->Set_Attack_Desc(2);
		break;
	case 3:
		static_cast<CController_MG*>(m_pController)->Set_Attack_Desc(3);
		break;
	}
}

void CState_MG_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Identity::Exit_State()
{
	static_cast<CController_MG*>(m_pController)->Get_MG_IdenSkillMessage();
}

void CState_MG_Identity::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity))
	{
		m_iAttackCnt++;
		static_cast<CController_MG*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_MG_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Identity* CState_MG_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Identity* pInstance = new CState_MG_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Identity::Free()
{
	__super::Free();
}
