#include "..\Public\Skill_GN_Gunkata.h"
#include "Player_Gunslinger.h"

CSkill_GN_Gunkata::CSkill_GN_Gunkata(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
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

	return S_OK;
}

HRESULT CSkill_GN_Gunkata::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_Gunkata::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Gunkata::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Gunkata::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_Gunkata::Check_ColliderState()
{
	__super::Check_ColliderState();
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

