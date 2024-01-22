#include "stdafx.h"
#include "Skill_Golem_Swipe.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSkill_Golem_Swipe::CSkill_Golem_Swipe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Golem_Swipe::CSkill_Golem_Swipe(const CSkill_Golem_Swipe& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Golem_Swipe::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Golem_Swipe::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fMoveSpeed = 0.7f;
	m_fLastTime = 1.f;
	m_SkillDesc.iAtk = 15;
	m_SkillDesc.fForce = 11.f;
    return S_OK;
}

void CSkill_Golem_Swipe::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	m_pTransformCom->Go_Left(m_fMoveSpeed,fTimeDelta);
	CSphereCollider* pCollider = m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS];
	_float fRadius = pCollider->Get_Radius();
	fRadius += 0.7f * fTimeDelta;
	pCollider->Set_Radius(fRadius);

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
	Vec3 vScale = pChildCollider->Get_Scale();
	vScale.x += 0.7f * fTimeDelta;
	pChildCollider->Set_Scale(vScale);
}

void CSkill_Golem_Swipe::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Golem_Swipe::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_Golem_Swipe::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Swipe::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Swipe::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Golem_Swipe::Ready_Coliders()
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
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderSkillChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
		}
	}

	CSphereCollider* pCollider = m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS];
	pCollider->Set_Radius(1.f);
	pCollider->Set_Offset(Vec3(0.f, 0.f, 0.25f));
	pCollider->SetActive(true);

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
	pChildCollider->Set_Scale(Vec3(0.9f, 1.f, 1.f));
	pChildCollider->Set_Offset(Vec3(0.f, 0.f, 0.25f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
    return S_OK;
}

HRESULT CSkill_Golem_Swipe::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Golem_Swipe* CSkill_Golem_Swipe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Golem_Swipe* pInstance = new CSkill_Golem_Swipe(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Golem_Swipe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Golem_Swipe::Clone(void* pArg)
{
    CSkill_Golem_Swipe* pInstance = new CSkill_Golem_Swipe(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Golem_Swipe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Golem_Swipe::Free()
{
	__super::Free();
}

