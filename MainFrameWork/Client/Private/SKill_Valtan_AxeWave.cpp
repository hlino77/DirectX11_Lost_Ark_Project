#include "stdafx.h"
#include "Skill_Valtan_AxeWave.h"
#include "GameInstance.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderFrustum.h>

CSkill_Valtan_AxeWave::CSkill_Valtan_AxeWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_AxeWave::CSkill_Valtan_AxeWave(const CSkill_Valtan_AxeWave& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_AxeWave::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_AxeWave::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 2.f;
	m_SkillDesc.iAtk = 30;
	m_SkillDesc.fForce = 42.f;
    return S_OK;
}

void CSkill_Valtan_AxeWave::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Radius()+fTimeDelta*10.f);
	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());
	pChildCollider->Set_Far(pChildCollider->Get_Far() + fTimeDelta * 10.f);
	pChildCollider->Set_Near(pChildCollider->Get_Near() + fTimeDelta * 10.f);
}

void CSkill_Valtan_AxeWave::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_AxeWave::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_Valtan_AxeWave::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_AxeWave::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_AxeWave::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_AxeWave::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(2.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.1f, 0.0f));

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Offset(Vec3(0.0f, 0.1f, 0.f));
	pChildCollider->Set_Far(2.5f);
	pChildCollider->Set_Near(0.5f);
	pChildCollider->Set_Slopes(Vec4(1.f, -0.05f, tanf(XMConvertToRadians(30.f)), tanf(XMConvertToRadians(-30.f))));
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

HRESULT CSkill_Valtan_AxeWave::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_AxeWave* CSkill_Valtan_AxeWave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_AxeWave* pInstance = new CSkill_Valtan_AxeWave(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_AxeWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_AxeWave::Clone(void* pArg)
{
    CSkill_Valtan_AxeWave* pInstance = new CSkill_Valtan_AxeWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_AxeWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_AxeWave::Free()
{
	__super::Free();
}

