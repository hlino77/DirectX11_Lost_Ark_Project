#include "stdafx.h"
#include "..\Public\Skill_GN_Gunkata.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"
#include "Model.h"
#include "Item.h"

CSkill_GN_Gunkata::CSkill_GN_Gunkata(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN_Gunkata"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_Gunkata::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_Gunkata");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::COUNTER;;
	m_eCtrlType = SKILL_CTRLTYPE::COMBO;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.8f);
	Proj_Desc.vChildScale = Vec3(0.6f, 0.6f, 0.6f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 0.8f);
	Proj_Desc.iDamage = 300;
	Proj_Desc.iStagger = 0;
	Proj_Desc.fRepulsion = 1.2f;
	Proj_Desc.IsCounter = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;
	Proj_Desc.IsCounter = false;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.4f);
	Proj_Desc.vChildScale = Vec3(0.35f, 0.6f, 1.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.4f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.fRepulsion = 1.5f;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.8f);
	Proj_Desc.vChildScale = Vec3(0.6f, 0.6f, 0.6f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 0.8f);
	Proj_Desc.iDamage = 200;
	Proj_Desc.fRepulsion = 21.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	m_iUseMana = 59;

	return S_OK;
}

void CSkill_GN_Gunkata::Tick(_float fTimeDelta)
{
}

void CSkill_GN_Gunkata::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_GN_Gunkata::Render()
{
	return S_OK;
}

HRESULT CSkill_GN_Gunkata::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Peacekeeper"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_Gunkata::Check_ColliderState()
{
	if (TEXT("Skill_GN_Gunkata_3") == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_State())
	{
		_uint iAnimIndex = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_CurrAnim();
		if (8 >= static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
		{
			m_SkillProjDesc = m_vecSkillProjDesces[2];

			if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
			{
				_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
				if (iItemLevel <= 10)
				{
					_uint iDamage = m_vecSkillProjDesces[2].iDamage;
					m_SkillProjDesc.iDamage = iDamage * m_iDefaultItem;
				}
				else
				{
					_uint iDamage = m_vecSkillProjDesces[2].iDamage;
					m_SkillProjDesc.iDamage = iDamage * m_iUpgradedItem;
				}
			}
		}
		else if(8 < static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(iAnimIndex))
		{
			m_SkillProjDesc = m_vecSkillProjDesces[1];

			if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
			{
				_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
				if (iItemLevel <= 10)
				{
					_uint iDamage = m_vecSkillProjDesces[1].iDamage;
					m_SkillProjDesc.iDamage = iDamage * m_iDefaultItem;
				}
				else
				{
					_uint iDamage = m_vecSkillProjDesces[1].iDamage;
					m_SkillProjDesc.iDamage = iDamage * m_iUpgradedItem;
				}
			}
		}
	}
	else if (TEXT("Skill_GN_Gunkata_2") == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[1];

		if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
		{
			_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
			if (iItemLevel <= 10)
			{
				_uint iDamage = m_vecSkillProjDesces[1].iDamage;
				m_SkillProjDesc.iDamage = iDamage * m_iDefaultItem;
			}
			else
			{
				_uint iDamage = m_vecSkillProjDesces[1].iDamage;
				m_SkillProjDesc.iDamage = iDamage * m_iUpgradedItem;
			}
		}
	}
	else if (TEXT("Skill_GN_Gunkata_1") == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_State())
	{
		m_SkillProjDesc = m_vecSkillProjDesces[0];

		if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
		{
			_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
			if (iItemLevel <= 10)
			{
				_uint iDamage = m_vecSkillProjDesces[0].iDamage;
				m_SkillProjDesc.iDamage = iDamage * m_iDefaultItem;
			}
			else
			{
				_uint iDamage = m_vecSkillProjDesces[0].iDamage;
				m_SkillProjDesc.iDamage = iDamage * m_iUpgradedItem;
			}
		}
	}
}

CSkill_GN_Gunkata* CSkill_GN_Gunkata::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_Gunkata* pInstance = new CSkill_GN_Gunkata(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_Gunkata");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_Gunkata::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_Gunkata::Free()
{
	__super::Free();
}

