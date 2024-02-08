#include "stdafx.h"
#include "..\Public\Skill_WDR_PerfectSwing.h"
#include "Player_Destroyer.h"
#include "Model.h"
#include "Projectile.h"

CSkill_WDR_PerfectSwing::CSkill_WDR_PerfectSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_PerfectSwing"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_PerfectSwing::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_PerfectSwing");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.5f);
	Proj_Desc.iDamage = 420;
	Proj_Desc.iStagger = 100;
	Proj_Desc.fRepulsion = 10.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 114;

	return S_OK;
}

void CSkill_WDR_PerfectSwing::Tick(_float fTimeDelta)
{
}

void CSkill_WDR_PerfectSwing::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WDR_PerfectSwing::Render()
{
	return S_OK;
}

HRESULT CSkill_WDR_PerfectSwing::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Perfect_Swing"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WDR_PerfectSwing::Check_ColliderState()
{
	if (TEXT("Skill_WDR_PerfectSwing_Success") == static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc.iDamage = 3000;
	}
	else
	{
		m_SkillProjDesc.iDamage = 2000;
	}
}

CSkill_WDR_PerfectSwing* CSkill_WDR_PerfectSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_PerfectSwing* pInstance = new CSkill_WDR_PerfectSwing(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_PerfectSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_PerfectSwing::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_PerfectSwing::Free()
{
	__super::Free();
}

