#include "stdafx.h"
#include "..\Public\Skill_GN_FocusShot.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"

CSkill_GN_FocusShot::CSkill_GN_FocusShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_FocusShot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_FocusShot");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_SKILL;
	Proj_Desc.fAttackTime = 0.6f;
	Proj_Desc.fRadius = 0.6f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.6f, 0.5f);
	Proj_Desc.IsMove = true;
	Proj_Desc.fMoveSpeed = 10.f;
	m_SkillProjDesc = Proj_Desc;


	return S_OK;
}

HRESULT CSkill_GN_FocusShot::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_FocusShot::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FocusShot::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FocusShot::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FocusShot::Check_ColliderState()
{
	__super::Check_ColliderState();
}

CSkill_GN_FocusShot* CSkill_GN_FocusShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_FocusShot* pInstance = new CSkill_GN_FocusShot(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_FocusShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_FocusShot::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_FocusShot::Free()
{
	__super::Free();
}

