#include "stdafx.h"
#include "..\Public\Skill_SP_Moondrawing.h"
#include "Player_Doaga.h"
#include "Projectile.h"

CSkill_SP_Moondrawing::CSkill_SP_Moondrawing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_Moondrawing"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_Moondrawing::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 올려치기 */
	m_strSkill_Name = TEXT("Skill_SP_Moondrawing");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::COUNTER;
	m_eCtrlType = SKILL_CTRLTYPE::COMBO;
	m_IsSuperArmor = true;

	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = m_pOwner;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
		Proj_Desc.fAttackTime = 0.05f;
		Proj_Desc.fRadius = 2.f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.8f);
		Proj_Desc.vChildScale = Vec3(1.2f, 0.6f, 1.6f);
		Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.8f);
		Proj_Desc.iDamage = 300;
		Proj_Desc.iStagger = 25;
		Proj_Desc.fRepulsion = 22.f;
		Proj_Desc.bUseFactor = false;
		Proj_Desc.IsCounter = true;

		m_vecSkillProjDesces.push_back(Proj_Desc);
		m_SkillProjDesc = Proj_Desc;
	}
	
	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = m_pOwner;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
		Proj_Desc.fAttackTime = 0.05f;
		Proj_Desc.fRadius = 2.f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
		Proj_Desc.vChildScale = Vec3(1.2f, 0.6f, 1.2f);
		Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
		Proj_Desc.iDamage = 200;
		Proj_Desc.iStagger = 25;
		Proj_Desc.fRepulsion = 21.f;
		Proj_Desc.bUseFactor = false;
		Proj_Desc.IsCounter = true;

		m_vecSkillProjDesces.push_back(Proj_Desc);
	}

	return S_OK;
}

void CSkill_SP_Moondrawing::Tick(_float fTimeDelta)
{
}

void CSkill_SP_Moondrawing::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_SP_Moondrawing::Render()
{
	return S_OK;
}

HRESULT CSkill_SP_Moondrawing::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yinyanshi_Upward_Stroke"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SP_Moondrawing::Check_ColliderState()
{
	if (TEXT("Skill_SP_Moondrawing_Start") == static_cast<CPlayer_Doaga*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
	else
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
}

CSkill_SP_Moondrawing* CSkill_SP_Moondrawing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_Moondrawing* pInstance = new CSkill_SP_Moondrawing(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_Moondrawing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_Moondrawing::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_Moondrawing::Free()
{
	__super::Free();
}

