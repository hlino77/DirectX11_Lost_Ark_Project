#include "..\Public\Skill_GN_PerfectShot.h"
#include "Player_Gunslinger.h"

CSkill_GN_PerfectShot::CSkill_GN_PerfectShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
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

	return S_OK;
}

HRESULT CSkill_GN_PerfectShot::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	return S_OK;
}

void CSkill_GN_PerfectShot::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_PerfectShot::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_PerfectShot::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_PerfectShot::Check_ColliderState()
{
	__super::Check_ColliderState();
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

