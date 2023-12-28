#include "stdafx.h"
#include "..\Public\Skill_WR_SpiningSword.h"
#include "Player_Slayer.h"
#include "Projectile.h"

CSkill_WR_SpiningSword::CSkill_WR_SpiningSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_WR_SpiningSword"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_SpiningSword::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_SpiningSword");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.8f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 10.f;
	Proj_Desc.bUseFactor = false;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

HRESULT CSkill_WR_SpiningSword::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Spinning_Sword"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_SpiningSword::Check_ColliderState()
{
}

CSkill_WR_SpiningSword* CSkill_WR_SpiningSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_SpiningSword* pInstance = new CSkill_WR_SpiningSword(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_SpiningSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_SpiningSword::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_SpiningSword::Free()
{
	__super::Free();
}

