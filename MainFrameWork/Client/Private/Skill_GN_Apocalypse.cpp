#include "stdafx.h"
#include "..\Public\Skill_GN_Apocalypse.h"
#include "Player_Gunslinger.h"
#include "State_Skill.h"

CSkill_GN_Apocalypse::CSkill_GN_Apocalypse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_Apocalypse::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_Apocalypse");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = false;

	return S_OK;
}

HRESULT CSkill_GN_Apocalypse::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Catastrophe"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_Apocalypse::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Apocalypse::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Apocalypse::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Apocalypse::Check_ColliderState()
{
}

CSkill_GN_Apocalypse* CSkill_GN_Apocalypse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_Apocalypse* pInstance = new CSkill_GN_Apocalypse(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_Apocalypse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_Apocalypse::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_Apocalypse::Free()
{
	__super::Free();
}

