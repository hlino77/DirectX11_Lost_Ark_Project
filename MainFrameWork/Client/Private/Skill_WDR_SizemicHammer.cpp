#include "stdafx.h"
#include "..\Public\Skill_WDR_SizemicHammer.h"
#include "Player_Destroyer.h"
#include "Model.h"
#include "Projectile.h"
#include "Item.h"

CSkill_WDR_SizemicHammer::CSkill_WDR_SizemicHammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_SizemicHammer"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_SizemicHammer::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_SizemicHammer");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = m_pOwner;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
		Proj_Desc.fAttackTime = 0.1f;
		Proj_Desc.fRadius = 1.4f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.2f);
		Proj_Desc.iDamage = 230;
		Proj_Desc.iStagger = 30;
		Proj_Desc.fRepulsion = 0.f;
		m_vecSkillProjDesces.push_back(Proj_Desc);
		m_SkillProjDesc = Proj_Desc;
	}
	

	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = m_pOwner;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
		Proj_Desc.fAttackTime = 0.1f;
		Proj_Desc.fRadius = 3.f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.6f);
		Proj_Desc.vChildScale = Vec3(2.f, 0.6f, 2.4f);
		Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.6f);
		Proj_Desc.iDamage = 1500;
		Proj_Desc.iStagger = 100;
		Proj_Desc.fRepulsion = 21.f;
		m_vecSkillProjDesces.push_back(Proj_Desc);
	}

	m_iUseMana = 0;

	return S_OK;
}

void CSkill_WDR_SizemicHammer::Tick(_float fTimeDelta)
{
}

void CSkill_WDR_SizemicHammer::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WDR_SizemicHammer::Render()
{
	return S_OK;
}

HRESULT CSkill_WDR_SizemicHammer::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Seismic_Hammer"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WDR_SizemicHammer::Check_ColliderState()
{
	if (TEXT("Skill_WDR_SizemicHammer") == static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_State())
	{
		_uint iAnimIndex = static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_CurrAnim();
		if (52 > static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
		{
			m_SkillProjDesc = m_vecSkillProjDesces[0];
		}
		else if (52 <= static_cast<CPlayer_Destroyer*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
		{
			m_SkillProjDesc = m_vecSkillProjDesces[1];
		}
	}

	if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
	{
		_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
		if (iItemLevel <= 10)
		{
			m_SkillProjDesc.iDamage * 1.f;
		}
		else
		{
			m_SkillProjDesc.iDamage * 1.5f;
		}
	}
}

CSkill_WDR_SizemicHammer* CSkill_WDR_SizemicHammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_SizemicHammer* pInstance = new CSkill_WDR_SizemicHammer(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_SizemicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_SizemicHammer::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_SizemicHammer::Free()
{
	__super::Free();
}

