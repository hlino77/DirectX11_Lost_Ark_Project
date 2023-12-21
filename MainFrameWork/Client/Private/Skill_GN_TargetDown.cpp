#include "..\Public\Skill_GN_TargetDown.h"
#include "Player_Gunslinger.h"

CSkill_GN_TargetDown::CSkill_GN_TargetDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_TargetDown::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_TargetDown");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::TARGET;
	m_IsSuperArmor = false;

	return S_OK;
}

HRESULT CSkill_GN_TargetDown::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_TargetDown::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_TargetDown::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_TargetDown::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_TargetDown::Check_ColliderState()
{
	__super::Check_ColliderState();
}

CSkill_GN_TargetDown* CSkill_GN_TargetDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_TargetDown* pInstance = new CSkill_GN_TargetDown(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_TargetDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_TargetDown::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_TargetDown::Free()
{
	__super::Free();
}