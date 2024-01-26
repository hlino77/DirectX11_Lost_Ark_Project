#include "stdafx.h"
#include "..\Public\State_WR_Esther.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_WR_Esther::CState_WR_Esther(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Esther::Initialize()
{
	m_iEsther = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"act_estherskill_1", 1.0f);
	if (m_iEsther == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Esther::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Esther::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Esther::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iEsther, 0.1f, 0, 0);


	m_pPlayer->Get_WR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperiorArmorState(true);
}

void CState_WR_Esther::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Esther::Exit_State()
{
	m_pPlayer->Set_SuperiorArmorState(false);
}

void CState_WR_Esther::Tick_State_Control(_float fTimeDelta)
{
	if (20 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther))
	{
		m_pPlayer->Get_WR_Controller()->Get_EstherMessage();
	}
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEsther))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_WR_Esther::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Esther* CState_WR_Esther::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Esther* pInstance = new CState_WR_Esther(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Esther");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Esther::Free()
{
	__super::Free();
}
