#include "stdafx.h"
#include "..\Public\Skill_SP_Onestroke.h"
#include "Player_Doaga.h"
#include "Projectile.h"

CSkill_SP_Onestroke::CSkill_SP_Onestroke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_Onestroke"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_Onestroke::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* ÇÑÈ¹±ß±â */
	m_strSkill_Name = TEXT("Skill_SP_Onestroke");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::COUNTER;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.6f);
	Proj_Desc.vChildScale = Vec3(1.5f, 0.6f, 2.4f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.6f);
	Proj_Desc.iDamage = 1800;
	Proj_Desc.iStagger = 80;
	Proj_Desc.fRepulsion = 21.f;
	Proj_Desc.IsCounter = true;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 103;

	return S_OK;
}

void CSkill_SP_Onestroke::Tick(_float fTimeDelta)
{
}

void CSkill_SP_Onestroke::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_SP_Onestroke::Render()
{
	return S_OK;
}

HRESULT CSkill_SP_Onestroke::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yinyanshi_One_Stroke"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SP_Onestroke::Check_ColliderState()
{
}

CSkill_SP_Onestroke* CSkill_SP_Onestroke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_Onestroke* pInstance = new CSkill_SP_Onestroke(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_Onestroke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_Onestroke::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_Onestroke::Free()
{
	__super::Free();
}

