#include "stdafx.h"
#include "..\Public\Skill_WR_FuriousClaw.h"
#include "Player_Slayer.h"
#include "Projectile.h"

CSkill_WR_FuriousClaw::CSkill_WR_FuriousClaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_WR_FuriousClaw"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_FuriousClaw::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_FuriousClaw");

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
	Proj_Desc.iStagger = 5;
	Proj_Desc.fRepulsion = 5.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 120;
	Proj_Desc.iStagger = 10;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = false;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

HRESULT CSkill_WR_FuriousClaw::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Furious_Claw"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_FuriousClaw::Check_ColliderState()
{
	if (TEXT("Skill_WR_FuriousClaw_End") == static_cast<CPlayer_Slayer*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
	else
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
}

CSkill_WR_FuriousClaw* CSkill_WR_FuriousClaw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_FuriousClaw* pInstance = new CSkill_WR_FuriousClaw(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_FuriousClaw");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_FuriousClaw::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_FuriousClaw::Free()
{
	__super::Free();
}

