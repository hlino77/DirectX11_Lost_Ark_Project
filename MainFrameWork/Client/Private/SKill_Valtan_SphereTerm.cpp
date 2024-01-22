#include "stdafx.h"
#include "Skill_Valtan_SphereTerm.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSkill_Valtan_SphereTerm::CSkill_Valtan_SphereTerm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_SphereTerm::CSkill_Valtan_SphereTerm(const CSkill_Valtan_SphereTerm& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_SphereTerm::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_SphereTerm::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 2.2f;
	m_fBlinkTime = 2.f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 12.f;

    return S_OK;
}

void CSkill_Valtan_SphereTerm::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f && m_fLastTime > 0.f)
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());

}

void CSkill_Valtan_SphereTerm::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_SphereTerm::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSkill_Valtan_SphereTerm::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_SphereTerm::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_SphereTerm::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_SphereTerm::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(1.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
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

HRESULT CSkill_Valtan_SphereTerm::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_SphereTerm* CSkill_Valtan_SphereTerm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_SphereTerm* pInstance = new CSkill_Valtan_SphereTerm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_SphereTerm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_SphereTerm::Clone(void* pArg)
{
    CSkill_Valtan_SphereTerm* pInstance = new CSkill_Valtan_SphereTerm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_SphereTerm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_SphereTerm::Free()
{
	__super::Free();
}

