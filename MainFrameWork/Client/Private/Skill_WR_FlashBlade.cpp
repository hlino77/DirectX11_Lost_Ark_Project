#include "stdafx.h"
#include "..\Public\Skill_WR_FlashBlade.h"
#include "Player_Slayer.h"
#include "Projectile.h"

CSkill_WR_FlashBlade::CSkill_WR_FlashBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WR_FlashBlade"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_FlashBlade::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_FlashBlade");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.iStagger = 0;
	Proj_Desc.fRepulsion = 8.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

void CSkill_WR_FlashBlade::Tick(_float fTimeDelta)
{
}

void CSkill_WR_FlashBlade::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WR_FlashBlade::Render()
{
	return S_OK;
}

HRESULT CSkill_WR_FlashBlade::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Cross_Blade"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_FlashBlade::Check_ColliderState()
{
}

CSkill_WR_FlashBlade* CSkill_WR_FlashBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_FlashBlade* pInstance = new CSkill_WR_FlashBlade(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_FlashBlade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_FlashBlade::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_FlashBlade::Free()
{
	__super::Free();
}

