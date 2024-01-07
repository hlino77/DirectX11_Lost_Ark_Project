#include "stdafx.h"
#include "..\Public\State_GN_Attack_Long1.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Effect_Manager.h"
#include "GameInstance.h"

CState_GN_Attack_Long1::CState_GN_Attack_Long1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Long1::Initialize()
{
	m_Attack_Long1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity2_1_01", 1.f);
	if (m_Attack_Long1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Long1::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Long1::Tick_State_NoneControl;

	/* 일반공격 프레임 */
	m_AttackFrames.push_back(10);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_GN_Attack_Long1::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_Long1, 0.1f, 0, 0);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Attack_Long1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Long1::Exit_State()
{
}

void CState_GN_Attack_Long1::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
	{
		Effect_Shot();

		m_iAttackCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_AttackMessage();
	}


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
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
		{
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
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
	else if (true == m_pController->Is_Attack())
	{
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Attack_Long_2"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Long1))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Long1))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Attack_Long1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_Attack_Long1::Effect_Shot()
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
	EFFECT_START(TEXT("LongBullet"), &desc)
	EFFECT_START(TEXT("LongMuzzleFlash"), &desc)

		_uint iCount = rand() % 2 + 1;
	for (_uint i = 0; i < iCount; ++i)
	{
		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(TEXT("LongBulletParticle"), &desc)
	}
}

CState_GN_Attack_Long1* CState_GN_Attack_Long1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Long1* pInstance = new CState_GN_Attack_Long1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Long1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Long1::Free()
{
	__super::Free();
}
