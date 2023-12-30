#include "stdafx.h"
#include "..\Public\Skill_WDR_PowerShoulder.h"
#include "Player_Destroyer.h"
#include "Model.h"
#include "Projectile.h"

CSkill_WDR_PowerShoulder::CSkill_WDR_PowerShoulder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_PowerShoulder"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_PowerShoulder::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_PowerShoulder");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::COMBO;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.6f);
	Proj_Desc.vChildScale = Vec3(0.8f, 0.6f, 0.6f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 0.6f);
	Proj_Desc.iDamage = 100.f;
	Proj_Desc.fRepulsion = 5.f;
	Proj_Desc.bUseFactor = false;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.2f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.fRepulsion = 25.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);


	return S_OK;
}

HRESULT CSkill_WDR_PowerShoulder::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bullet_Rain"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;*/

	return S_OK;
}

void CSkill_WDR_PowerShoulder::Check_ColliderState()
{
	if (TEXT("Skill_WDR_PowerShoulder_End") == static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
	else
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
}

CSkill_WDR_PowerShoulder* CSkill_WDR_PowerShoulder::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_PowerShoulder* pInstance = new CSkill_WDR_PowerShoulder(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_PowerShoulder");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_PowerShoulder::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_PowerShoulder::Free()
{
	__super::Free();
}

