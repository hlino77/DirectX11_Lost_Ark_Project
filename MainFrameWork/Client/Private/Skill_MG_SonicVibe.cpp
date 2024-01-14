#include "stdafx.h"
#include "..\Public\Skill_MG_SonicVibe.h"
#include "Player_Bard.h"
#include "Projectile.h"
#include "Model.h"

CSkill_MG_SonicVibe::CSkill_MG_SonicVibe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_MG_SonicVibe"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_MG_SonicVibe::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_MG_SonicVibe");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 1.2f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.fRepulsion = 20.f;
	Proj_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	PROJECTILE_DESC Proj_Spawn_Desc;
	Proj_Spawn_Desc.pAttackOwner = m_pOwner;
	Proj_Spawn_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Spawn_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Spawn_Desc.fAttackTime = 10.f;
	Proj_Spawn_Desc.fRadius = 1.2f;
	Proj_Spawn_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	
	Proj_Spawn_Desc.IsSpawner = true;
	Proj_Spawn_Desc.fSpawnAttackTime = 0.05f;
	Proj_Spawn_Desc.iSpawnAmount = 3;
	Proj_Spawn_Desc.fSpawnTime = 1.f;
	Proj_Spawn_Desc.fSpawnRadius = 1.2f;
	Proj_Spawn_Desc.vSpawnOffset = Vec3(0.0f, 0.0f, 0.0f);
	
	Proj_Spawn_Desc.iDamage = 50;
	Proj_Spawn_Desc.fRepulsion = 0.f;
	Proj_Spawn_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Spawn_Desc);

	return S_OK;
}

HRESULT CSkill_MG_SonicVibe::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sonic_Vibration"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_MG_SonicVibe::Check_ColliderState()
{
	_uint iAnimIndex = static_cast<CPlayer_Bard*>(m_pOwner)->Get_ModelCom()->Get_CurrAnim();
	if (60 >= static_cast<CPlayer_Bard*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
	else if (60 < static_cast<CPlayer_Bard*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
}

CSkill_MG_SonicVibe* CSkill_MG_SonicVibe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer, void* pArg)
{
	CSkill_MG_SonicVibe* pInstance = new CSkill_MG_SonicVibe(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_MG_SonicVibe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_MG_SonicVibe::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_MG_SonicVibe::Free()
{
	__super::Free();
}

