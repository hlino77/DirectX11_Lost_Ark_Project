#include "stdafx.h"
#include "Skill_Valtan_DoughnutSecondHit.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"

CSkill_Valtan_DoughnutSecondHit::CSkill_Valtan_DoughnutSecondHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Valtan_DoughnutSecondHit::CSkill_Valtan_DoughnutSecondHit(const CSkill_Valtan_DoughnutSecondHit& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Valtan_DoughnutSecondHit::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Valtan_DoughnutSecondHit::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fLastTime = 3.2f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 45.f;
	m_fBlinkTime = 3.f;
    return S_OK;
}

void CSkill_Valtan_DoughnutSecondHit::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f&& m_fLastTime > 0.f)
	{
		if (m_strSoundTag.empty() == false && !m_bSoundOn)
		{
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 90.0f, 0.1f, 11.0f);
			CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", m_strSoundTag, 1.f);
			m_bSoundOn = true;
		}
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());
	}
}

void CSkill_Valtan_DoughnutSecondHit::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Valtan_DoughnutSecondHit::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSkill_Valtan_DoughnutSecondHit::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_DoughnutSecondHit::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Valtan_DoughnutSecondHit::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Valtan_DoughnutSecondHit::Ready_Coliders()
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
	pChildCollider->Set_Radius(6.5f);
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

void CSkill_Valtan_DoughnutSecondHit::Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius)
{
	if (OutsideRadius <= InsideRadius)
		return;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(OutsideRadius);

	dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child())->Set_Radius(InsideRadius);
}

HRESULT CSkill_Valtan_DoughnutSecondHit::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Valtan_DoughnutSecondHit* CSkill_Valtan_DoughnutSecondHit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Valtan_DoughnutSecondHit* pInstance = new CSkill_Valtan_DoughnutSecondHit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Valtan_DoughnutSecondHit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Valtan_DoughnutSecondHit::Clone(void* pArg)
{
    CSkill_Valtan_DoughnutSecondHit* pInstance = new CSkill_Valtan_DoughnutSecondHit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Valtan_DoughnutSecondHit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Valtan_DoughnutSecondHit::Free()
{
	__super::Free();
}

