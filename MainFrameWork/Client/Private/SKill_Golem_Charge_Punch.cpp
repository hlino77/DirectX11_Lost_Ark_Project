#include "stdafx.h"
#include "Skill_Golem_Charge_Punch.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSkill_Golem_Charge_Punch::CSkill_Golem_Charge_Punch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Golem_Charge_Punch::CSkill_Golem_Charge_Punch(const CSkill_Golem_Charge_Punch& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Golem_Charge_Punch::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Golem_Charge_Punch::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fMoveSpeed = 3.5f;
	m_fLastTime = 5.f;
	m_SkillDesc.iAtk = 22;
	m_SkillDesc.fForce = 30.5f;
    return S_OK;
}

void CSkill_Golem_Charge_Punch::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	m_pTransformCom->Go_Straight(m_fMoveSpeed, fTimeDelta);
	m_fMoveSpeed += 2.f*fTimeDelta;
}

void CSkill_Golem_Charge_Punch::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Golem_Charge_Punch::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_Golem_Charge_Punch::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Charge_Punch::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Charge_Punch::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Golem_Charge_Punch::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)		
			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(1.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.0f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
    return S_OK;
}

HRESULT CSkill_Golem_Charge_Punch::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Golem_Charge_Punch* CSkill_Golem_Charge_Punch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Golem_Charge_Punch* pInstance = new CSkill_Golem_Charge_Punch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Golem_Charge_Punch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Golem_Charge_Punch::Clone(void* pArg)
{
    CSkill_Golem_Charge_Punch* pInstance = new CSkill_Golem_Charge_Punch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Golem_Charge_Punch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Golem_Charge_Punch::Free()
{
	__super::Free();
}

