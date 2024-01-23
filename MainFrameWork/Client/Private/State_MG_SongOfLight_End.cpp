#include "stdafx.h"
#include "..\Public\State_MG_SongOfLight_End.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Player_Skill.h"
#include "Model.h"

CState_MG_SongOfLight_End::CState_MG_SongOfLight_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_SongOfLight_End::Initialize()
{
	m_iSongOfLight = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_rhapsodyoflight_end", 1.2f);
	if (m_iSongOfLight == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_SongOfLight_End::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_SongOfLight_End::Tick_State_NoneControl;


	return S_OK;
}

void CState_MG_SongOfLight_End::Enter_State()
{

	m_pPlayer->Reserve_Animation(m_iSongOfLight, 0.1f, 0, 0);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

}

void CState_MG_SongOfLight_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_SongOfLight_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_MG_SongOfLight_End::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSongOfLight))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_MG_SongOfLight_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_SongOfLight_End* CState_MG_SongOfLight_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_SongOfLight_End* pInstance = new CState_MG_SongOfLight_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_SongOfLight_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_SongOfLight_End::Free()
{
	__super::Free();
}
