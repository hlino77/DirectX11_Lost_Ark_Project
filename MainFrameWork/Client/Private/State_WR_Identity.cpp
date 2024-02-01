#include "stdafx.h"
#include "..\Public\State_WR_Identity.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"

CState_WR_Identity::CState_WR_Identity(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Identity::Initialize()
{
	m_iIdentity = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"mode_identity1", 1.0f);
	if (m_iIdentity == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Identity::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Identity::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Identity::Enter_State()
{
	m_bEffect = false;

	m_pPlayer->Reserve_Animation(m_iIdentity, 0.2f, 0, 0);

	m_pController->Get_StopMessage();
	
	static_cast<CController_WR*>(m_pController)->Get_WR_IdentityMessage();

	m_Effects.clear();
}

void CState_WR_Identity::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Identity::Exit_State()
{
	
}

void CState_WR_Identity::Tick_State_Control(_float fTimeDelta)
{
	if (!m_bEffect && 9 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity))
	{
		m_bEffect = true;

		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

		m_pPlayer->Get_Camera()->Cam_Shake(0.3f, 100.f, 0.2f, 10.0f);
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matPivot;

		EFFECT_START(TEXT("Slayer_Rage"), &desc)
		m_pPlayer->Get_Camera()->Set_Chromatic(0.3f, matPivot.Translation(), 0.1f, 0.1f);

		desc.pPivotMatrix = &matPivot;
		EFFECT_START_OUTLIST(L"Slayer_Rage_Aura", &desc, m_Effects);

		auto func = bind(&CTransform::Load_WorldMatrix, m_pPlayer->Get_TransformCom(), placeholders::_1);
		m_pPlayer->Get_WR_Controller()->CB_UpdateIdentityAuraPivot += func;

		m_pPlayer->Add_Effect(L"Slayer_Rage_Aura", m_Effects[0]);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Identity::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Identity* CState_WR_Identity::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Identity* pInstance = new CState_WR_Identity(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Identity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Identity::Free()
{
	__super::Free();
}
