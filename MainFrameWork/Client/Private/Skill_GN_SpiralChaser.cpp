#include "stdafx.h"
#include "..\Public\Skill_GN_SpiralChaser.h"
#include "Player_Gunslinger.h"

CSkill_GN_SpiralChaser::CSkill_GN_SpiralChaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_SpiralChaser::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_SprialChaser");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::TARGET;
	m_IsSuperArmor = false;

	return S_OK;
}

HRESULT CSkill_GN_SpiralChaser::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_SpiralChaser::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_SpiralChaser::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_SpiralChaser::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_SpiralChaser::Check_ColliderState()
{
	__super::Check_ColliderState();
}

CSkill_GN_SpiralChaser* CSkill_GN_SpiralChaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_SpiralChaser* pInstance = new CSkill_GN_SpiralChaser(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_SpiralChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_SpiralChaser::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_SpiralChaser::Free()
{
	__super::Free();
}