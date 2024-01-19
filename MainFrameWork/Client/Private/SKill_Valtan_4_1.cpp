#include "stdafx.h"
#include "SKill_Valtan_4_1.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSKill_Valtan_4_1::CSKill_Valtan_4_1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_Valtan_4_1::CSKill_Valtan_4_1(const CSKill_Valtan_4_1& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_Valtan_4_1::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_Valtan_4_1::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 0.2f;
	m_iAtk = 20;
	m_fForce = 42.f;

    return S_OK;
}

void CSKill_Valtan_4_1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSKill_Valtan_4_1::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_Valtan_4_1::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSKill_Valtan_4_1::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_4_1::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_4_1::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_Valtan_4_1::Ready_Coliders()
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

HRESULT CSKill_Valtan_4_1::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_Valtan_4_1* CSKill_Valtan_4_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_Valtan_4_1* pInstance = new CSKill_Valtan_4_1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_Valtan_4_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_Valtan_4_1::Clone(void* pArg)
{
    CSKill_Valtan_4_1* pInstance = new CSKill_Valtan_4_1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_Valtan_4_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_Valtan_4_1::Free()
{
	__super::Free();
}

