#include "stdafx.h"
#include "..\Public\Skill_GN_SpiralChaser.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"

CSkill_GN_SpiralChaser::CSkill_GN_SpiralChaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN_SpiralChaser"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_SpiralChaser::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_SprialChaser");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::TARGET;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Spawn_Desc;
	Proj_Spawn_Desc.pAttackOwner = m_pOwner;
	Proj_Spawn_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Spawn_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Spawn_Desc.fAttackTime = 10.f;
	Proj_Spawn_Desc.fRadius = 1.7f;
	Proj_Spawn_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);

	Proj_Spawn_Desc.IsSpawner = true;
	Proj_Spawn_Desc.fSpawnAttackTime = 0.05f;
	Proj_Spawn_Desc.iSpawnAmount = 20;
	Proj_Spawn_Desc.fSpawnTime = 0.1f;
	Proj_Spawn_Desc.fSpawnRadius = 1.2f;
	Proj_Spawn_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);

	Proj_Spawn_Desc.iDamage = 90;
	Proj_Spawn_Desc.iStagger = 0;
	Proj_Spawn_Desc.fRepulsion = 0.f;
	Proj_Spawn_Desc.bUseProjPos = true;
	m_SkillProjDesc = Proj_Spawn_Desc;
	m_vecSkillProjDesces.push_back(Proj_Spawn_Desc);


	return S_OK;
}

HRESULT CSkill_GN_SpiralChaser::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Spiral_Tracker"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_SpiralChaser::Check_ColliderState()
{
}

CSkill_GN_SpiralChaser* CSkill_GN_SpiralChaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_SpiralChaser* pInstance = new CSkill_GN_SpiralChaser(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_SpiralChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_SpiralChaser::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_SpiralChaser::Free()
{
	__super::Free();
}