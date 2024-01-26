#include "stdafx.h"
#include "..\Public\Controller_MG.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"
#include "Esther.h"

CController_MG::CController_MG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_MG::CController_MG(const CController_MG& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_MG::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_MG::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Base_Desc;
	Proj_Base_Desc.pAttackOwner = m_pOwner;
	Proj_Base_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Base_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Base_Desc.fAttackTime = 3.f;

	/* 기본공격 */
	Proj_Base_Desc.IsSpawner = true;
	Proj_Base_Desc.fSpawnAttackTime = 0.5f;
	Proj_Base_Desc.iSpawnAmount = 1;
	Proj_Base_Desc.fSpawnTime = 0.f;
	Proj_Base_Desc.fSpawnRadius = 0.7f;
	Proj_Base_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);
	Proj_Base_Desc.IsColliSpawn = true;

	Proj_Base_Desc.fRadius = 0.5f;
	Proj_Base_Desc.vOffset = Vec3(0.0f, 0.2f, 0.8f);
	Proj_Base_Desc.iDamage = 50;
	Proj_Base_Desc.fRepulsion = 0.f;
	Proj_Base_Desc.bUseProjPos = true;
	Proj_Base_Desc.IsMove = true;
	Proj_Base_Desc.fMoveSpeed = 8.f;
	m_Attack_Desces[0] = Proj_Base_Desc;

	//
	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 15;
	Proj_Desc.fRepulsion = 0.f;
	Proj_Desc.bUseProjPos = true;
	m_Attack_Desces[1] = Proj_Desc;

	//
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 20;
	Proj_Desc.fRepulsion = 0.f;
	Proj_Desc.bUseProjPos = true;
	m_Attack_Desces[2] = Proj_Desc;

	//
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 25;
	Proj_Desc.fRepulsion = 0.f;
	Proj_Desc.bUseProjPos = true;
	m_Attack_Desces[3] = Proj_Desc;

	//
	PROJECTILE_DESC Proj_Spawn_Desc;
	Proj_Spawn_Desc.pAttackOwner = m_pOwner;
	Proj_Spawn_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Spawn_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Spawn_Desc.fAttackTime = 12.f;
	Proj_Spawn_Desc.fRadius = 2.5f;
	Proj_Spawn_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);

	Proj_Spawn_Desc.IsSpawner = true;
	Proj_Spawn_Desc.fSpawnAttackTime = 0.05f;
	Proj_Spawn_Desc.iSpawnAmount = 8;
	Proj_Spawn_Desc.fSpawnTime = 1.f;
	Proj_Spawn_Desc.fSpawnRadius = 2.5f;
	Proj_Spawn_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);

	Proj_Spawn_Desc.iDamage = 1;
	Proj_Spawn_Desc.fRepulsion = 0.f;
	Proj_Spawn_Desc.bUseProjPos = true;
	m_Attack_Desces[4] = Proj_Spawn_Desc;

	//
	Proj_Spawn_Desc.pAttackOwner = m_pOwner;
	Proj_Spawn_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Spawn_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Spawn_Desc.fAttackTime = 18.f;
	Proj_Spawn_Desc.fRadius = 2.5f;
	Proj_Spawn_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);

	Proj_Spawn_Desc.IsSpawner = true;
	Proj_Spawn_Desc.fSpawnAttackTime = 0.05f;
	Proj_Spawn_Desc.iSpawnAmount = 12;
	Proj_Spawn_Desc.fSpawnTime = 1.f;
	Proj_Spawn_Desc.fSpawnRadius = 2.5f;
	Proj_Spawn_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);

	Proj_Spawn_Desc.iDamage = 1;
	Proj_Spawn_Desc.fRepulsion = 0.f;
	Proj_Spawn_Desc.bUseProjPos = true;
	m_Attack_Desces[5] = Proj_Spawn_Desc;

	//
	Proj_Spawn_Desc.pAttackOwner = m_pOwner;
	Proj_Spawn_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Spawn_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Spawn_Desc.fAttackTime = 24.f;
	Proj_Spawn_Desc.fRadius = 2.5f;
	Proj_Spawn_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);

	Proj_Spawn_Desc.IsSpawner = true;
	Proj_Spawn_Desc.fSpawnAttackTime = 0.05f;
	Proj_Spawn_Desc.iSpawnAmount = 16;
	Proj_Spawn_Desc.fSpawnTime = 1.f;
	Proj_Spawn_Desc.fSpawnRadius = 2.5f;
	Proj_Spawn_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);

	Proj_Spawn_Desc.iDamage = 1;
	Proj_Spawn_Desc.fRepulsion = 0.f;
	Proj_Spawn_Desc.bUseProjPos = true;
	m_Attack_Desces[6] = Proj_Spawn_Desc;

	return S_OK;
}

void CController_MG::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CController_MG::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CController_MG::DebugRender()
{
}

void CController_MG::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
	__super::Get_HitMessage(iDamge, fForce, vPos);

	// 데미지하락 및 밉라이트?

	if (HIT_TYPE::WEAK != m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperiorArmor())
	{
		if (HIT_TYPE::DMG == m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperArmor())
		{
			m_IsHitState = true;

			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, m_fForced, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit_Common"));
		}
		else if (HIT_TYPE::DMG != m_eHitType)
		{
			m_IsHitState = true;

			_float fCheckHit = 0.0f;
			switch (m_eHitType)
			{
			case Engine::CPlayer_Controller::DOWN:
				fCheckHit = m_fForced;
				break;
			case Engine::CPlayer_Controller::KNOCKDOWN:
				fCheckHit = 10.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::BOUND:
				fCheckHit = 20.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::TWIST:
				fCheckHit = 30.f + m_fForced;
				break;
			}
			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, fCheckHit, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit"));
		}
	}
}

void CController_MG::Get_GrabMessage(CGameObject* pGrabber)
{
	__super::Get_GrabMessage(pGrabber);

	static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Grabbed"));

	static_cast<CPlayer*>(m_pOwner)->Set_SuperArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_Invincible(false);

	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(true);
}

void CController_MG::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();

	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);
}

void CController_MG::Get_DeadMessage()
{
	__super::Get_DeadMessage();

	m_iIdentityGage = 0;
}

_uint CController_MG::Is_MG_Identity()
{
	if (false == m_bKeyActive)
		return 0;

	if (100 > m_iIdentityGage)
		return 0;

	if (KEY_TAP(KEY::Z))
	{
		return 1;
	}
	else if (KEY_TAP(KEY::X))
	{
		return 2;
	}

	return 0;
}

HRESULT CController_MG::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	__super::Bind_Skill(eKey, pSkill);
	pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CController_MG::Get_PlayerSkill_MG(SKILL_KEY eKey)
{
	return nullptr;
}

void CController_MG::Input(const _float& fTimeDelta)
{

}

void CController_MG::Attack(Vec3 vPos)
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();

	if (Vec3() != vPos)
	{
		m_AttackDesc.vAttackPos = vPos;
	}
	else
	{
		m_AttackDesc.AttackMatrix = m_pOwner->Get_TransformCom()->Get_WorldMatrix();
	}

	pAttack->InitProjectile(&m_AttackDesc);
}

void CController_MG::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
	if (nullptr == m_pSkills[eKey])
		return;

	CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
	if (Vec3() != vPos)
	{
		m_pSkills[eKey]->Set_SkillProjPos(vPos);
	}
	else
	{
		m_pSkills[eKey]->Set_SkillProjMat(m_pOwner->Get_TransformCom()->Get_WorldMatrix());
	}
	pSkill->InitProjectile(&m_pSkills[eKey]->Get_Skill_Proj_Desc());
}

void CController_MG::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

void CController_MG::EstherSkill(_uint iIndex)
{
	for (auto& pEsther : m_vecEsther)
	{
		if (static_cast<CEsther*>(pEsther)->Get_EstherType() == iIndex)
		{
			if (false == pEsther->Is_Active())
				static_cast<CEsther*>(pEsther)->Leader_Active_Esther();
		}
	}
}

void CController_MG::Get_MG_IdentityMessage()
{
	if (100 <= m_iIdentityGage)
		m_iGage = 1;
	if (200 <= m_iIdentityGage)
		m_iGage = 2;
	if (300 <= m_iIdentityGage)
		m_iGage = 3;

	m_iIdentityGage = 0;
}

void CController_MG::Increase_IdenGage(_uint iGage)
{
	m_iIdentityGage += iGage;

	if (m_iMaxGage <= m_iIdentityGage)
	{
		m_iIdentityGage = m_iMaxGage;
	}
}

CController_MG* CController_MG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_MG* pInstance = new CController_MG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_MG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_MG::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_MG* pInstance = new CController_MG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_MG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_MG::Free()
{
	__super::Free();
}
