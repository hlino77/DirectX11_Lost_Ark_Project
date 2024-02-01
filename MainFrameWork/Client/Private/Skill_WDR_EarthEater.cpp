#include "stdafx.h"
#include "..\Public\Skill_WDR_EarthEater.h"
#include "Player_Destroyer.h"
#include "Model.h"
#include "Projectile.h"

CSkill_WDR_EarthEater::CSkill_WDR_EarthEater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_EarthEater"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_EarthEater::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_EarthEater");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.2f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, -1.5f);
	Proj_Desc.iDamage = 100;
	Proj_Desc.iStagger = 20;
	Proj_Desc.fRepulsion = 8.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.4f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 50;
	Proj_Desc.iStagger = 15;
	Proj_Desc.fRepulsion = 2.f;
	Proj_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.iStagger = 50;
	Proj_Desc.fRepulsion = 10.f;
	Proj_Desc.bUseProjPos = false;
	Proj_Desc.bUseFactor = false;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

void CSkill_WDR_EarthEater::Tick(_float fTimeDelta)
{
}

void CSkill_WDR_EarthEater::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WDR_EarthEater::Render()
{
	return S_OK;
}

HRESULT CSkill_WDR_EarthEater::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Earth_Eater"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WDR_EarthEater::Check_ColliderState()
{
	_uint iAnimIndex = static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_CurrAnim();
	if (25 >= static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
	if (25 < static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
	if (80 < static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
	{
		m_SkillProjDesc = m_vecSkillProjDesces[2];
	}
}

CSkill_WDR_EarthEater* CSkill_WDR_EarthEater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_EarthEater* pInstance = new CSkill_WDR_EarthEater(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_EarthEater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_EarthEater::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_EarthEater::Free()
{
	__super::Free();
}

