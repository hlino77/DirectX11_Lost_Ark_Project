#include "stdafx.h"
#include "..\Public\State_GN_Attack_Shot2.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CState_GN_Attack_Shot2::CState_GN_Attack_Shot2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Shot2::Initialize()
{
	m_Attack_Shot2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity1_1_02", 1.4f);
	if (m_Attack_Shot2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Shot2::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Shot2::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(15);
	m_AttackFrames.push_back(24);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_GN_Attack_Shot2::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_Shot2, 0.1f, 0, 0);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_IsAttackContinue = false;
}

void CState_GN_Attack_Shot2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Shot2::Exit_State()
{
}

void CState_GN_Attack_Shot2::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Shot2);

	if (m_AttackFrames[m_iAttackCnt] <= (_int)iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_AttackMessage();

		Effect_Shot();
	}

	if (true == m_pController->Is_Attack() &&
		40 > iAnimFrame &&
		30 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Shot2))
		m_pPlayer->Set_State(TEXT("Idle"));

	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != iIdentity)
	{
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 40 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Attack_Shot_1"));
	}
	else if (true == m_pController->Is_Run() && 40 < iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_GN_Attack_Shot2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_AttackFrames[m_iAttackCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Shot2))
	{
		m_iAttackCnt++;

		Effect_Shot();
	}
}

void CState_GN_Attack_Shot2::Effect_Shot()
{
	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 500.f, 0.1f, 14.f);

	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_4))->Get_Part_WorldMatrix().Translation();
	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();
	vPos += vOriginLook * 0.3f;
	Vec3 vOriginUp = matWorld.Up();
	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginUp.Normalize();
	vOriginRight.Normalize();

	{
		matWorld.Translation(vPos);
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;

		EFFECT_START(TEXT("ShotMuzzleFlash"), &desc)
	}

	for (_uint i = 0; i < 10; ++i)
	{
		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);

		
		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(TEXT("ShotBullet"), &desc)
	}
}

CState_GN_Attack_Shot2* CState_GN_Attack_Shot2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Shot2* pInstance = new CState_GN_Attack_Shot2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Shot2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Shot2::Free()
{
	__super::Free();
}
