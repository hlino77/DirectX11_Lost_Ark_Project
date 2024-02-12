#include "stdafx.h"
#include "..\Public\State_GN_Esther_Way.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"
#include "Esther.h"

CState_GN_Esther_Way::CState_GN_Esther_Way(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Esther_Way::Initialize()
{
	m_iEsther = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"act_estherskill_1", 1.0f);
	if (m_iEsther == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Esther_Way::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Esther_Way::Tick_State_NoneControl;

	return S_OK;
}
 
void CState_GN_Esther_Way::Enter_State()
{
	m_bEstherActive = false;

	m_pPlayer->Reserve_Animation(m_iEsther, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_EshterGageUseMessage();

	m_pPlayer->Set_SuperiorArmorState(true);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_1, false);
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_2, false);
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_3, false);
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_4, false);
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);
}

void CState_GN_Esther_Way::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Esther_Way::Exit_State()
{
	m_pPlayer->Set_SuperiorArmorState(false);

	CPlayer_Controller_GN::GN_IDENTITY eIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Get_GN_Identity();
	m_pPlayer->Set_Weapon_RenderState(eIdentity);
}

void CState_GN_Esther_Way::Tick_State_Control(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_GN_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::WY);
		m_bEstherActive = true;
	}
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEsther))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Esther_Way::Tick_State_NoneControl(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_GN_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::WY);
		m_bEstherActive = true;
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Esther_Way* CState_GN_Esther_Way::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Esther_Way* pInstance = new CState_GN_Esther_Way(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Esther_Way");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Esther_Way::Free()
{
	__super::Free();
}
