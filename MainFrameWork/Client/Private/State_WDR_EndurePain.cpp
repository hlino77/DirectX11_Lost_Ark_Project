#include "stdafx.h"
#include "..\Public\State_WDR_EndurePain.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_EndurePain::CState_WDR_EndurePain(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_EndurePain::Initialize()
{
	m_iEndurePain = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_endurepain", 1.2f);
	if (m_iEndurePain == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_EndurePain::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_EndurePain::Tick_State_NoneControl;

	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_EndurePain::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iEndurePain, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillChangeStatMessage(m_eSkillSelectKey);
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_AddMarbleMessage(3.f);
}

void CState_WDR_EndurePain::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_EndurePain::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_EndurePain::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEndurePain))
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEndurePain))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WDR_EndurePain::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_EndurePain* CState_WDR_EndurePain::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_EndurePain* pInstance = new CState_WDR_EndurePain(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_EndurePain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_EndurePain::Free()
{
	__super::Free();
}
