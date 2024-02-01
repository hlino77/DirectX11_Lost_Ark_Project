#include "stdafx.h"
#include "..\Public\Skill_SP_Flyheaven.h"
#include "Player_Doaga.h"
#include "Projectile.h"

CSkill_SP_Flyheaven::CSkill_SP_Flyheaven(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_Flyheaven"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_Flyheaven::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 두루미나래 */
	m_strSkill_Name = TEXT("Skill_SP_Flyheaven");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.3f;
	Proj_Desc.fRadius = 0.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.6f, 0.3f);
	Proj_Desc.IsMove = true;
	Proj_Desc.fMoveSpeed = 30.f;
	Proj_Desc.iDamage = 300;
	Proj_Desc.iStagger = 70;
	Proj_Desc.fRepulsion = 21.f;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

void CSkill_SP_Flyheaven::Tick(_float fTimeDelta)
{
}

void CSkill_SP_Flyheaven::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_SP_Flyheaven::Render()
{
	return S_OK;
}

HRESULT CSkill_SP_Flyheaven::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sonatina"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SP_Flyheaven::Check_ColliderState()
{
}

CSkill_SP_Flyheaven* CSkill_SP_Flyheaven::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_Flyheaven* pInstance = new CSkill_SP_Flyheaven(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_Flyheaven");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_Flyheaven::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_Flyheaven::Free()
{
	__super::Free();
}

