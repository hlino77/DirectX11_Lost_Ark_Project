#include "stdafx.h"
#include "..\Public\Skill_GN_TerminatingShot.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"

CSkill_GN_TerminatingShot::CSkill_GN_TerminatingShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_TerminatingShot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_TerminatingShot");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::COMBO;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.6f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(0.8f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 150.f;
	Proj_Desc.fRepulsion = 2.0f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.6f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(0.8f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 200.f;
	Proj_Desc.fRepulsion = 25.0f;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

HRESULT CSkill_GN_TerminatingShot::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;


	return S_OK;
}

void CSkill_GN_TerminatingShot::Check_ColliderState()
{
	if (TEXT("Skill_GN_TerminatingShot_End") == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
	else
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
}

CSkill_GN_TerminatingShot* CSkill_GN_TerminatingShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_TerminatingShot* pInstance = new CSkill_GN_TerminatingShot(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_TerminatingShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_TerminatingShot::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_TerminatingShot::Free()
{
	__super::Free();
}