#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Custom_CrossHair.h"
#include "GameInstance.h"
#include "Camera_Player.h"


CState_GN_TargetDown_Loop::CState_GN_TargetDown_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Loop::Initialize()
{
	m_iTargetDown_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_02", 1.0f);
	if (m_iTargetDown_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Loop::Tick_State_NoneControl;

	m_fSkillEndTime = 2.5f;

	return S_OK;
}

void CState_GN_TargetDown_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iTargetDown_Loop, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);
}

void CState_GN_TargetDown_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;

	if (3 < m_iShotCount)
		m_iShotCount = 0;

	if (true == m_pController->Is_HitState())
	{
		//Effect_End();
	}
}

void CState_GN_TargetDown_Loop::Tick_State_Control(_float fTimeDelta)
{
	Vec3 vClickPos;
	if (m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pController->Get_LerpDirLookMessage(vClickPos, 10.f);
	}

	m_fSkillTimeAcc += fTimeDelta;

	if (2 < m_iShotCount)
	{
		m_iShotCount++;
		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_End"));
	}
	else if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_End"));
	}
	else if (true == m_pController->Is_Tap(KEY::LBTN))
	{
		m_iShotCount++;

		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Shot"));
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
		Effect_End();
	}
}

void CState_GN_TargetDown_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos(), 10.f);
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_Loop::Effect_End()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect_Custom_CrossHair* pEffect = dynamic_cast<CEffect_Custom_CrossHair*>(pGameInstance->Find_GameObejct(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"GN_CrossHair"));

	pEffect->EffectEnd();

	m_pPlayer->Get_Camera()->DefaultLength(7.0f);


	CEffect* pDecal = dynamic_cast<CEffect*>(pGameInstance->Find_GameObejct(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, L"Effect_TargetDownDecal"));
	pDecal->Set_ObjectTag(L"Effect_TargetDownDecalReady");
	pDecal->EffectEnd();

	Safe_Release(pGameInstance);
}

CState_GN_TargetDown_Loop* CState_GN_TargetDown_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Loop* pInstance = new CState_GN_TargetDown_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Loop::Free()
{
	__super::Free();
}
