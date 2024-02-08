#include "stdafx.h"
#include "..\Public\State_SP_Resurrect.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "Renderer.h"

CState_SP_Resurrect::CState_SP_Resurrect(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Resurrect::Initialize()
{
	m_iResurrect = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"resurrect_1", 1.0f);
	if (m_iResurrect == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Resurrect::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Resurrect::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Resurrect::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iResurrect, 0.2f, 0, 0);

	m_pPlayer->Set_Invincible(true);

	m_pPlayer->Get_RendererCom()->Set_DeadScene(false);

	CGameObject::STATDESC tPcStat = m_pPlayer->Get_PlayerStat_Desc();
	tPcStat.iCurHp = tPcStat.iMaxHp;
	tPcStat.iCurMp = tPcStat.iMaxMp;
	m_pPlayer->Set_PlayerStat_Desc(tPcStat);
}

void CState_SP_Resurrect::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Resurrect::Exit_State()
{
	m_pPlayer->Set_Invincible(false);
}

void CState_SP_Resurrect::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iResurrect))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_SP_Resurrect::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Resurrect* CState_SP_Resurrect::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Resurrect* pInstance = new CState_SP_Resurrect(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Resurrect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Resurrect::Free()
{
	__super::Free();
}
