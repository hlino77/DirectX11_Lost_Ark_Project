#include "stdafx.h"
#include "..\Public\Skill_WDR_FullSwing.h"
#include "Player_Destroyer.h"
#include "Projectile.h"

CSkill_WDR_FullSwing::CSkill_WDR_FullSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_FullSwing"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_FullSwing::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_FullSwing");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 1.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.3f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.iStagger = 80;
	Proj_Desc.fRepulsion = 8.f;
	Proj_Desc.bUseFactor = false;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

HRESULT CSkill_WDR_FullSwing::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Full_Swing"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WDR_FullSwing::Check_ColliderState()
{
	if (true == m_IsSkillSuccess)
	{
		m_SkillProjDesc.iDamage = 750;
	}
	else
	{
		m_SkillProjDesc.iDamage = 350;
	}
}

CSkill_WDR_FullSwing* CSkill_WDR_FullSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_FullSwing* pInstance = new CSkill_WDR_FullSwing(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_FullSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_FullSwing::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_FullSwing::Free()
{
	__super::Free();
}
