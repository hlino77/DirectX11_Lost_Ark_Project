#include "stdafx.h"
#include "SKill_King_Eruption.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"

CSKill_King_Eruption::CSKill_King_Eruption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_King_Eruption::CSKill_King_Eruption(const CSKill_King_Eruption& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_King_Eruption::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_King_Eruption::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 2.55f;
	m_iAtk = 5;
	m_fForce = 40.f;
	m_fBlinkTime = 2.5f;
    return S_OK;
}

void CSKill_King_Eruption::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f)
	{
		m_fBlinkTime =  5.f;
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	}
}

void CSKill_King_Eruption::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_King_Eruption::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSKill_King_Eruption::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_Eruption::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_Eruption::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_King_Eruption::Ready_Coliders()
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
				CFrustumCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_FrustumCollider"), TEXT("Com_ColliderSkillChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
		}
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(7.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.1f, 0.0f));

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Offset(Vec3(0.0f, 0.1f, 0.f));
	pChildCollider->Set_Far(7.5f);
	pChildCollider->Set_Near(0.f);
	pChildCollider->Set_Slopes(Vec4(0.05f, -0.05f, tanf(XMConvertToRadians(30.f)), tanf(XMConvertToRadians(-30.f))));
	pChildCollider->SetActive(true);
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

HRESULT CSKill_King_Eruption::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_King_Eruption* CSKill_King_Eruption::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_King_Eruption* pInstance = new CSKill_King_Eruption(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_King_Eruption");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_King_Eruption::Clone(void* pArg)
{
    CSKill_King_Eruption* pInstance = new CSKill_King_Eruption(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_King_Eruption");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_King_Eruption::Free()
{
	__super::Free();
}

