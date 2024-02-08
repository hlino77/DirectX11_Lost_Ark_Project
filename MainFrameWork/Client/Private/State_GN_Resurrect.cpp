#include "stdafx.h"
#include "..\Public\State_GN_Resurrect.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "Renderer.h"

CState_GN_Resurrect::CState_GN_Resurrect(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Resurrect::Initialize()
{
	m_iResurrect = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"resurrect_1", 1.0f);
	if (m_iResurrect == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Resurrect::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Resurrect::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Resurrect::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iResurrect, 0.2f, 0, 0);

	m_pPlayer->Set_Invincible(true);

	m_pPlayer->Get_RendererCom()->Set_DeadScene(false);

	CGameObject::STATDESC tPcStat = m_pPlayer->Get_PlayerStat_Desc();
	tPcStat.iCurHp = tPcStat.iMaxHp;
	tPcStat.iCurMp = tPcStat.iMaxMp;
	m_pPlayer->Set_PlayerStat_Desc(tPcStat);
}

void CState_GN_Resurrect::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Resurrect::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
}

void CState_GN_Resurrect::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iResurrect))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_Resurrect::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Resurrect* CState_GN_Resurrect::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Resurrect* pInstance = new CState_GN_Resurrect(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Resurrect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Resurrect::Free()
{
	__super::Free();
}
