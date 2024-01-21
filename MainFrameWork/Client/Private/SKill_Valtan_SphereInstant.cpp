#include "stdafx.h"
#include "SKill_Valtan_SphereInstant.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSKill_Valtan_SphereInstant::CSKill_Valtan_SphereInstant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_Valtan_SphereInstant::CSKill_Valtan_SphereInstant(const CSKill_Valtan_SphereInstant& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_Valtan_SphereInstant::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_Valtan_SphereInstant::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 0.2f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 42.f;

    return S_OK;
}

void CSKill_Valtan_SphereInstant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSKill_Valtan_SphereInstant::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_Valtan_SphereInstant::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSKill_Valtan_SphereInstant::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_SphereInstant::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_SphereInstant::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_Valtan_SphereInstant::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(4.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

HRESULT CSKill_Valtan_SphereInstant::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_Valtan_SphereInstant* CSKill_Valtan_SphereInstant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_Valtan_SphereInstant* pInstance = new CSKill_Valtan_SphereInstant(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_Valtan_SphereInstant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_Valtan_SphereInstant::Clone(void* pArg)
{
    CSKill_Valtan_SphereInstant* pInstance = new CSKill_Valtan_SphereInstant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_Valtan_SphereInstant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_Valtan_SphereInstant::Free()
{
	__super::Free();
}

