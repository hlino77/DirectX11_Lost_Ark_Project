#include "stdafx.h"
#include "Skill_Valtan_SeismicWave.h"
#include "GameInstance.h"
#include <ColliderSphereGroup.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>

CSkill_Valtan_SeismicWave::CSkill_Valtan_SeismicWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_SeismicWave::CSkill_Valtan_SeismicWave(const CSkill_Valtan_SeismicWave& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_SeismicWave::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_SeismicWave::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 1.5f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 42.f;

    return S_OK;
}

void CSkill_Valtan_SeismicWave::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Radius() + fTimeDelta * 10.f);
	CSphereColliderGroup* pChildCollider = dynamic_cast<CSphereColliderGroup*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());
	for (size_t i = 0; i < 4; i++)
	{
		CSphereCollider* pChildSphereCollider = dynamic_cast<CSphereCollider*>(pChildCollider->Get_Collider(i));
		switch (i)
		{
		case 0:
			pChildSphereCollider->Set_Offset(pChildSphereCollider->Get_Offset() + Vec3(0.f, 0.f, 8.f) * fTimeDelta);
			break;
		case 1:
			pChildSphereCollider->Set_Offset(pChildSphereCollider->Get_Offset() + Vec3(0.f, 0.f, -8.f) * fTimeDelta);
			break;
		case 2:
			pChildSphereCollider->Set_Offset(pChildSphereCollider->Get_Offset() + Vec3(8.f, 0.f, 0.f) * fTimeDelta);
			break;
		case 3:
			pChildSphereCollider->Set_Offset(pChildSphereCollider->Get_Offset() + Vec3(-8.f, 0.f, 0.f) * fTimeDelta);
			break;
		default:
			break;
		}
	}

}

void CSkill_Valtan_SeismicWave::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_SeismicWave::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSkill_Valtan_SeismicWave::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_SeismicWave::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_SeismicWave::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_SeismicWave::Ready_Coliders()
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
				CSphereColliderGroup* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColliderGroup"), TEXT("Com_ColliderSkillChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
				{
					CCollider::ColliderInfo tChildSphereColliderInfo;
					tChildSphereColliderInfo.m_bActive = false;
					tChildSphereColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
					CSphereCollider* pChildSphereCollider = nullptr;
					if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkillChild_0"), (CComponent**)&pChildSphereCollider, &tChildSphereColliderInfo)))
						return E_FAIL;
					pChildCollider->Add_Collider(pChildSphereCollider);
					pChildSphereCollider->Set_Radius(1.5f);
					pChildSphereCollider->SetActive(true);
					pChildSphereCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

				}
				{
					CCollider::ColliderInfo tChildSphereColliderInfo;
					tChildSphereColliderInfo.m_bActive = false;
					tChildSphereColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
					CSphereCollider* pChildSphereCollider = nullptr;
					if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkillChild_1"), (CComponent**)&pChildSphereCollider, &tChildSphereColliderInfo)))
						return E_FAIL;
					pChildCollider->Add_Collider(pChildSphereCollider);
					pChildSphereCollider->Set_Radius(1.5f);
					pChildSphereCollider->SetActive(true);
					pChildSphereCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
				}
				{
					CCollider::ColliderInfo tChildSphereColliderInfo;
					tChildSphereColliderInfo.m_bActive = false;
					tChildSphereColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
					CSphereCollider* pChildSphereCollider = nullptr;
					if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkillChild_2"), (CComponent**)&pChildSphereCollider, &tChildSphereColliderInfo)))
						return E_FAIL;
					pChildCollider->Add_Collider(pChildSphereCollider);
					pChildSphereCollider->Set_Radius(1.5f);
					pChildSphereCollider->SetActive(true);
					pChildSphereCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
				}
				{
					CCollider::ColliderInfo tChildSphereColliderInfo;
					tChildSphereColliderInfo.m_bActive = false;
					tChildSphereColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
					CSphereCollider* pChildSphereCollider = nullptr;
					if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkillChild_3"), (CComponent**)&pChildSphereCollider, &tChildSphereColliderInfo)))
						return E_FAIL;
					pChildCollider->Add_Collider(pChildSphereCollider);
					pChildSphereCollider->Set_Radius(1.5f);
					pChildSphereCollider->SetActive(true);
					pChildSphereCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
				}
			}
		}
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(1.5f);
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

HRESULT CSkill_Valtan_SeismicWave::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_SeismicWave* CSkill_Valtan_SeismicWave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_SeismicWave* pInstance = new CSkill_Valtan_SeismicWave(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_SeismicWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_SeismicWave::Clone(void* pArg)
{
    CSkill_Valtan_SeismicWave* pInstance = new CSkill_Valtan_SeismicWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_SeismicWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_SeismicWave::Free()
{
	__super::Free();
}

