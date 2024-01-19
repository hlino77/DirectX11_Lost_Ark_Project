#include "stdafx.h"
#include "SKill_Valtan_Doughnut.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

cSKill_Valtan_Doughnut::cSKill_Valtan_Doughnut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

cSKill_Valtan_Doughnut::cSKill_Valtan_Doughnut(const cSKill_Valtan_Doughnut& rhs)
          : CSkill(rhs)
{
}

HRESULT cSKill_Valtan_Doughnut::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT cSKill_Valtan_Doughnut::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 10.f;
	m_iAtk = 5;
	m_fForce = 10.f;
    return S_OK;
}

void cSKill_Valtan_Doughnut::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f)
	{
		m_fBlinkTime =  1.f;
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());
	}
}

void cSKill_Valtan_Doughnut::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT cSKill_Valtan_Doughnut::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void cSKill_Valtan_Doughnut::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void cSKill_Valtan_Doughnut::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void cSKill_Valtan_Doughnut::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT cSKill_Valtan_Doughnut::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = false;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				CDoughnutCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_DoughnutCollider"), TEXT("Com_ColliderSkillChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
		}
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(10.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

	CDoughnutCollider* pChildCollider = dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());
	pChildCollider->Set_Radius(5.f);
	pChildCollider->SetActive(false);
	pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

void cSKill_Valtan_Doughnut::Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius)
{
	if (OutsideRadius <= InsideRadius)
		return;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(OutsideRadius);

	dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child())->Set_Radius(InsideRadius);
}

HRESULT cSKill_Valtan_Doughnut::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

cSKill_Valtan_Doughnut* cSKill_Valtan_Doughnut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    cSKill_Valtan_Doughnut* pInstance = new cSKill_Valtan_Doughnut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : cSKill_Valtan_Doughnut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* cSKill_Valtan_Doughnut::Clone(void* pArg)
{
    cSKill_Valtan_Doughnut* pInstance = new cSKill_Valtan_Doughnut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : cSKill_Valtan_Doughnut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void cSKill_Valtan_Doughnut::Free()
{
	__super::Free();
}

