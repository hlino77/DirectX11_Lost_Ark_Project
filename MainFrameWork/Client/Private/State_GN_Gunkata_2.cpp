#include "stdafx.h"
#include "..\Public\State_GN_Gunkata_2.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_Gunkata_2::CState_GN_Gunkata_2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Gunkata_2::Initialize()
{
	m_iGunkata_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_gunkata_02", 1.f);
	if (m_iGunkata_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Gunkata_2::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Gunkata_2::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Gunkata_2::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iGunkata_2, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Gunkata_2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Gunkata_2::Exit_State()
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGunkata_2))
	{
		m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

		if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
			m_pPlayer->Set_SuperArmorState(false);
	}
	m_bComboContinue = false;
}

void CState_GN_Gunkata_2::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGunkata_2))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pPlayer->Get_GN_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGunkata_2) &&
		15 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGunkata_2))
	{
		m_bComboContinue = true;
	}
	else if (true == m_bComboContinue && 15 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGunkata_2))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Skill_GN_Gunkata_3"));
	}
}

void CState_GN_Gunkata_2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Gunkata_2* CState_GN_Gunkata_2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Gunkata_2* pInstance = new CState_GN_Gunkata_2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Gunkata_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Gunkata_2::Free()
{
	__super::Free();
}
