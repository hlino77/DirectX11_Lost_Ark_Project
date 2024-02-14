#include "stdafx.h"
#include "..\Public\State_WDR_Iden_Attack_3.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_Iden_Attack_3::CState_WDR_Iden_Attack_3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Iden_Attack_3::Initialize()
{
	m_Attack_3 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_gravity_1_03", 1.4f);
	if (m_Attack_3 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Iden_Attack_3::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Iden_Attack_3::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(12);
	m_AttackFrames.push_back(-1);


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_3.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WDR_Iden_Attack_3::Enter_State()
{

	m_EffectSound = false;

	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_3, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_WDR*>(m_pController)->Set_Attack_Desc(3);

}

void CState_WDR_Iden_Attack_3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Iden_Attack_3::Exit_State()
{
	m_IsAttackContinue = false;
	m_IsSkillContinue = false;
}

void CState_WDR_Iden_Attack_3::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimIndex = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_3);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= iAnimIndex)
	{
		m_iAttackCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_AttackMessage();

		if (m_EffectSound == false)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
			m_EffectSound = true;
		}

		Effect_Shot();
	}


	if (true == m_pController->Is_Attack() &&
		29 > iAnimIndex &&
		19 <= iAnimIndex)
	{
		m_IsAttackContinue = true;
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity() &&
		29 > iAnimIndex &&
		19 <= iAnimIndex)
	{
		m_IsAttackContinue = false;
		m_IsSkillContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_3))
	{
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Idle"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Iden_Skill"));
	}
	else if (true == m_IsSkillContinue && 29 <= iAnimIndex)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Skill"));
		}
	}
	else if (true == m_IsAttackContinue && 29 <= iAnimIndex)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Attack_4"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (29 < iAnimIndex)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
}

void CState_WDR_Iden_Attack_3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_3))
	{
		m_iAttackCnt++;
		Effect_Shot();
	}
}

void CState_WDR_Iden_Attack_3::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(L"AttGravity3", &desc);

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 100.0f, 0.5f, 10.0f);
}



CState_WDR_Iden_Attack_3* CState_WDR_Iden_Attack_3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Iden_Attack_3* pInstance = new CState_WDR_Iden_Attack_3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Iden_Attack_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Iden_Attack_3::Free()
{
	__super::Free();
}
