#include "stdafx.h"
#include "SKill_Valtan_DoughnutTerm2sec.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSKill_Valtan_DoughnutTerm2sec::CSKill_Valtan_DoughnutTerm2sec(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_Valtan_DoughnutTerm2sec::CSKill_Valtan_DoughnutTerm2sec(const CSKill_Valtan_DoughnutTerm2sec& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_Valtan_DoughnutTerm2sec::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_Valtan_DoughnutTerm2sec::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 2.2f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 45.f;
	m_fBlinkTime = 2.f;
    return S_OK;
}

void CSKill_Valtan_DoughnutTerm2sec::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f&& m_fLastTime > 0.f)
	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());
	}
}

void CSKill_Valtan_DoughnutTerm2sec::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_Valtan_DoughnutTerm2sec::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSKill_Valtan_DoughnutTerm2sec::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_DoughnutTerm2sec::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_DoughnutTerm2sec::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_Valtan_DoughnutTerm2sec::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(20.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

	CDoughnutCollider* pChildCollider = dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());
	pChildCollider->Set_Radius(5.3f);
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

void CSKill_Valtan_DoughnutTerm2sec::Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius)
{
	if (OutsideRadius <= InsideRadius)
		return;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(OutsideRadius);

	dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child())->Set_Radius(InsideRadius);
}

HRESULT CSKill_Valtan_DoughnutTerm2sec::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_Valtan_DoughnutTerm2sec* CSKill_Valtan_DoughnutTerm2sec::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_Valtan_DoughnutTerm2sec* pInstance = new CSKill_Valtan_DoughnutTerm2sec(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_Valtan_DoughnutTerm2sec");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_Valtan_DoughnutTerm2sec::Clone(void* pArg)
{
    CSKill_Valtan_DoughnutTerm2sec* pInstance = new CSKill_Valtan_DoughnutTerm2sec(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_Valtan_DoughnutTerm2sec");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_Valtan_DoughnutTerm2sec::Free()
{
	__super::Free();
}

