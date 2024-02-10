#include "stdafx.h"
#include "..\Public\Skill_GN_PerfectShot.h"
#include "Player_Gunslinger.h"
#include "Projectile.h"
#include "Item.h"

CSkill_GN_PerfectShot::CSkill_GN_PerfectShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN_PerfectShot"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_PerfectShot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_PerfectShot");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.3f;
	Proj_Desc.fRadius = 0.8f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.6f, 0.5f);
	Proj_Desc.IsMove = true;
	Proj_Desc.fMoveSpeed = 30.f;
	Proj_Desc.iDamage = 700;
	Proj_Desc.iStagger = 20;
	Proj_Desc.fRepulsion = 21.f;	
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 100;

	return S_OK;
}

void CSkill_GN_PerfectShot::Tick(_float fTimeDelta)
{
}

void CSkill_GN_PerfectShot::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_GN_PerfectShot::Render()
{
	return S_OK;
}

HRESULT CSkill_GN_PerfectShot::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Perfect_Shot"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_PerfectShot::Check_ColliderState()
{
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

CSkill_GN_PerfectShot* CSkill_GN_PerfectShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_PerfectShot* pInstance = new CSkill_GN_PerfectShot(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_PerfectShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_PerfectShot::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_PerfectShot::Free()
{
	__super::Free();
}

