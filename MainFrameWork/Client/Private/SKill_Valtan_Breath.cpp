#include "stdafx.h"
#include "Skill_Valtan_Breath.h"
#include "GameInstance.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderFrustum.h>

CSkill_Valtan_Breath::CSkill_Valtan_Breath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_Breath::CSkill_Valtan_Breath(const CSkill_Valtan_Breath& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_Breath::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_Breath::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 0.8f;
	m_SkillDesc.iAtk = 100;
	m_SkillDesc.fForce = 49.9f;
	m_bSafeZonePierce = true;
    return S_OK;
}

void CSkill_Valtan_Breath::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CSkill_Valtan_Breath::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_Breath::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_Valtan_Breath::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_Breath::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_Breath::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_Breath::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.1f, 0.0f));

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Offset(Vec3(0.0f, 0.1f, 0.f));
	pChildCollider->Set_Far(7.5f);
	pChildCollider->Set_Near( 0.f);
	pChildCollider->Set_Slopes(Vec4(tanf(XMConvertToRadians(80.f)), tanf(XMConvertToRadians(-80.f)), tanf(XMConvertToRadians(60.f)), tanf(XMConvertToRadians(-60.f))));
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

HRESULT CSkill_Valtan_Breath::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_Breath* CSkill_Valtan_Breath::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_Breath* pInstance = new CSkill_Valtan_Breath(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_Breath");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_Breath::Clone(void* pArg)
{
    CSkill_Valtan_Breath* pInstance = new CSkill_Valtan_Breath(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_Breath");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_Breath::Free()
{
	__super::Free();
}

