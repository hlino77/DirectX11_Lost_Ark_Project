#include "stdafx.h"
#include "..\Public\Skill_WR_VolcanoEruption.h"
#include "Player_Slayer.h"
#include "Projectile.h"
#include "Model.h"

CSkill_WR_VolcanoEruption::CSkill_WR_VolcanoEruption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WR_VolcanoEruption"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_VolcanoEruption::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_VolcanoEruption");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.f);
	Proj_Desc.iDamage = 110;
	Proj_Desc.iStagger = 5;
	Proj_Desc.fRepulsion = 0.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.f);
	Proj_Desc.iDamage = 950;
	Proj_Desc.iStagger = 40;
	Proj_Desc.fRepulsion = 31.f;
	Proj_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

void CSkill_WR_VolcanoEruption::Tick(_float fTimeDelta)
{
}

void CSkill_WR_VolcanoEruption::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WR_VolcanoEruption::Render()
{
	return S_OK;
}

HRESULT CSkill_WR_VolcanoEruption::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Volcanic_Eruption"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_VolcanoEruption::Check_ColliderState()
{
	if (TEXT("Skill_WR_VolcanoEruption_Success") == static_cast<CPlayer_Slayer*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];
	}
	else
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];
	}
}

CSkill_WR_VolcanoEruption* CSkill_WR_VolcanoEruption::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_VolcanoEruption* pInstance = new CSkill_WR_VolcanoEruption(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_VolcanoEruption");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_VolcanoEruption::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_VolcanoEruption::Free()
{
	__super::Free();
}

