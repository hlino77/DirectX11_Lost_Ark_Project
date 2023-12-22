#include "stdafx.h"
#include "..\Public\Skill_GN_RapidFire.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"

CSkill_GN_RapidFire::CSkill_GN_RapidFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_RapidFire::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_RapidFire");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::COMBO;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.6f, 0.0f);
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

HRESULT CSkill_GN_RapidFire::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_RapidFire::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_RapidFire::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_RapidFire::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_RapidFire::Check_ColliderState()
{
}

CSkill_GN_RapidFire* CSkill_GN_RapidFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_RapidFire* pInstance = new CSkill_GN_RapidFire(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_RapidFire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_RapidFire::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_RapidFire::Free()
{
	__super::Free();
}