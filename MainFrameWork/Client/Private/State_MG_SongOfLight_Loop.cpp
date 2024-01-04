#include "stdafx.h"
#include "..\Public\State_MG_SongOfLight_Loop.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Player_Skill.h"
#include "Model.h"

CState_MG_SongOfLight_Loop::CState_MG_SongOfLight_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_SongOfLight_Loop::Initialize()
{
	m_iSongOfLight = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_rhapsodyoflight_loop", 1.2f);
	if (m_iSongOfLight == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_SongOfLight_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_SongOfLight_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(60);
	m_SkillFrames.push_back(107);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_MG_SongOfLight_Loop::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iSongOfLight, 0.1f, 0, 0);

	m_pPlayer->Get_MG_Controller()->Get_StopMessage();
	m_pPlayer->Get_MG_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_MG_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());



	Vec3 vDir = m_pPlayer->Get_TargetPos() - m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	if (5.f <= vDir.Length())
	{
		m_vColliPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vDir.Normalize();
		m_vColliPos += vDir * 5.f;
	}
	else
	{
		m_vColliPos = m_pPlayer->Get_TargetPos();
	}

}

void CState_MG_SongOfLight_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_SongOfLight_Loop::Exit_State()
{
}

void CState_MG_SongOfLight_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSongOfLight))
	{
		m_iSkillCnt++;
		static_cast<CController_MG*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey, m_vColliPos);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSongOfLight))
		m_pPlayer->Set_State(TEXT("Skill_MG_SongOfLight_End"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_MG_SongOfLight_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_SongOfLight_Loop* CState_MG_SongOfLight_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_SongOfLight_Loop* pInstance = new CState_MG_SongOfLight_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_SongOfLight_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_SongOfLight_Loop::Free()
{
	__super::Free();
}
