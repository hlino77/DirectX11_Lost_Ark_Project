#include "stdafx.h"
#include "Skill_King_ChargeSwing.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSkill_King_ChargeSwing::CSkill_King_ChargeSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_King_ChargeSwing::CSkill_King_ChargeSwing(const CSkill_King_ChargeSwing& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_King_ChargeSwing::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_King_ChargeSwing::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fMoveSpeed = 6.f;
	m_fLastTime = 5.f;
	m_SkillDesc.iAtk = 30;
	m_SkillDesc.fForce = 30.5f;
    return S_OK;
}

void CSkill_King_ChargeSwing::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	m_pTransformCom->Go_Straight(m_fMoveSpeed, fTimeDelta);
}

void CSkill_King_ChargeSwing::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_King_ChargeSwing::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_King_ChargeSwing::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_King_ChargeSwing::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_King_ChargeSwing::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_King_ChargeSwing::Ready_Coliders()
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
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(3.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.5f, 0.0f));

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());
	pChildCollider->Set_Scale(Vec3(1.5f, 0.2f, 1.f));
	pChildCollider->Set_Offset(Vec3(0.0f, 0.5f, 0.f));
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

HRESULT CSkill_King_ChargeSwing::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_King_ChargeSwing* CSkill_King_ChargeSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_King_ChargeSwing* pInstance = new CSkill_King_ChargeSwing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_King_ChargeSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_King_ChargeSwing::Clone(void* pArg)
{
    CSkill_King_ChargeSwing* pInstance = new CSkill_King_ChargeSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_King_ChargeSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_King_ChargeSwing::Free()
{
	__super::Free();
}

