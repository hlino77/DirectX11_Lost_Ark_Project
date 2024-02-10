#include "stdafx.h"
#include "Skill_Valtan_PizzaTerm.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSkill_Valtan_PizzaTerm::CSkill_Valtan_PizzaTerm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_PizzaTerm::CSkill_Valtan_PizzaTerm(const CSkill_Valtan_PizzaTerm& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_PizzaTerm::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_PizzaTerm::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 2.2f;
	m_fBlinkTime = 2.f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 45.f;
    return S_OK;
}

void CSkill_Valtan_PizzaTerm::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f&& m_fLastTime > 0.f)
	{
		if (m_strSoundTag.empty() == false && !m_bSoundOn)
		{
			CGameInstance::GetInstance()->PlaySoundFile(m_strSoundTag, CHANNEL_EFFECT);
			m_bSoundOn = true;
		}
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());
	}
}

void CSkill_Valtan_PizzaTerm::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_PizzaTerm::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSkill_Valtan_PizzaTerm::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_PizzaTerm::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_PizzaTerm::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_PizzaTerm::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(20.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.1f, 0.0f));

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Offset(Vec3(0.0f, 0.1f, 0.f));
	pChildCollider->Set_Far(20.f);
	pChildCollider->Set_Near(0.f);
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


HRESULT CSkill_Valtan_PizzaTerm::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_PizzaTerm* CSkill_Valtan_PizzaTerm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_PizzaTerm* pInstance = new CSkill_Valtan_PizzaTerm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_PizzaTerm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_PizzaTerm::Clone(void* pArg)
{
    CSkill_Valtan_PizzaTerm* pInstance = new CSkill_Valtan_PizzaTerm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_PizzaTerm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_PizzaTerm::Free()
{
	__super::Free();
}

