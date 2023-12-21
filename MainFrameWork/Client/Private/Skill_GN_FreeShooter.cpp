#include "..\Public\Skill_GN_FreeShooter.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CSkill_GN_FreeShooter::CSkill_GN_FreeShooter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("SKill_GN"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_GN_FreeShooter::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_GN_FreeShooter");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	return S_OK;
}

HRESULT CSkill_GN_FreeShooter::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	//CCollider::ColliderInfo tColliderInfo;
	//tColliderInfo.m_bActive = true;
	//tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	//CSphereCollider* pCollider = nullptr;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
	//	return E_FAIL;

	//if (pCollider)
	//{
	//	{
	//		CCollider::ColliderInfo tChildColliderInfo;
	//		tChildColliderInfo.m_bActive = true;
	//		tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
	//		COBBCollider* pChildCollider = nullptr;

	//		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderAttackChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
	//			return E_FAIL;

	//		pCollider->Set_Child(pChildCollider);
	//	}

	//	m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER, pCollider);
	//}

	return S_OK;
}

void CSkill_GN_FreeShooter::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FreeShooter::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FreeShooter::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_GN_FreeShooter::Check_ColliderState()
{
	__super::Check_ColliderState();
}

CSkill_GN_FreeShooter* CSkill_GN_FreeShooter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Gunslinger* pPlayer, void* pArg)
{
	CSkill_GN_FreeShooter* pInstance = new CSkill_GN_FreeShooter(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_GN_FreeShooter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_GN_FreeShooter::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_GN_FreeShooter::Free()
{
	__super::Free();
}

