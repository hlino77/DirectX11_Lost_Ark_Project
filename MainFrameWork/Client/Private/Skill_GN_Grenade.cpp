#include "stdafx.h"
#include "..\Public\Skill_GN_Grenade.h"
#include "Player_Gunslinger.h"

CSkill_GN_Grenade::CSkill_GN_Grenade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_Grenade::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_strSkill_Name = TEXT("Skill_GN_Grenade");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::TARGET;
	m_IsSuperArmor = false;

	return S_OK;
}

HRESULT CSkill_GN_Grenade::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_AT02_Grenade"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_Grenade::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Grenade::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Grenade::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Grenade::Check_ColliderState()
{
}

CSkill_GN_Grenade* CSkill_GN_Grenade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_Grenade* pInstance = new CSkill_GN_Grenade(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_Grenade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_Grenade::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_Grenade::Free()
{
	__super::Free();
}

