#include "stdafx.h"
#include "..\Public\Skill_SP_DimensionalShift.h"
#include "Player_Doaga.h"
#include "Projectile.h"

CSkill_SP_DimensionalShift::CSkill_SP_DimensionalShift(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_DimensionalShift"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_DimensionalShift::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 환영의 문 */
	m_strSkill_Name = TEXT("Skill_SP_DimensionalShift");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::TARGET;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 0;
	Proj_Desc.iStagger = 0;
	Proj_Desc.fRepulsion = 0.f;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 135;

	return S_OK;
}

void CSkill_SP_DimensionalShift::Tick(_float fTimeDelta)
{
}

void CSkill_SP_DimensionalShift::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_SP_DimensionalShift::Render()
{
	return S_OK;
}

HRESULT CSkill_SP_DimensionalShift::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yinyanshi_Door_of_Illusion"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SP_DimensionalShift::Check_ColliderState()
{
}

CSkill_SP_DimensionalShift* CSkill_SP_DimensionalShift::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_DimensionalShift* pInstance = new CSkill_SP_DimensionalShift(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_DimensionalShift");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_DimensionalShift::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_DimensionalShift::Free()
{
	__super::Free();
}

