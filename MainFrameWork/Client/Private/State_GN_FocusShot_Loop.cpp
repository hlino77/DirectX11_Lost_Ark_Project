#include "stdafx.h"
#include "..\Public\State_GN_FocusShot_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"

CState_GN_FocusShot_Loop::CState_GN_FocusShot_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_FocusShot_Loop::Initialize()
{
	m_iFocusShot_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_02", 1.0f);
	if (m_iFocusShot_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_FocusShot_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_FocusShot_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(2);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_FocusShot_Loop::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFocusShot_Loop, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
}

void CState_GN_FocusShot_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_FocusShot_Loop::Exit_State()
{
	m_iShotCount = 0;
}

void CState_GN_FocusShot_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFocusShot_Loop))
	{
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_Loop))
	{
		m_iSkillCnt = 0;
		m_iShotCount++;
	}
	if(m_iShotCount >= 3)
		m_pPlayer->Set_State(TEXT("Skill_GN_FocusShot_End"));

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_GN_FocusShot_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_FocusShot_Loop::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("FocusShotBullet"), &desc)
}

CState_GN_FocusShot_Loop* CState_GN_FocusShot_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_FocusShot_Loop* pInstance = new CState_GN_FocusShot_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_FocusShot_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_FocusShot_Loop::Free()
{
	__super::Free();
}
