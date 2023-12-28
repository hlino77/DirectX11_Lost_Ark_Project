#include "stdafx.h"
#include "SKill_King_ChargeSwing.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSKill_King_ChargeSwing::CSKill_King_ChargeSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_King_ChargeSwing::CSKill_King_ChargeSwing(const CSKill_King_ChargeSwing& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_King_ChargeSwing::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_King_ChargeSwing::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_fMoveSpeed = 7.5f;
	m_fLastTime = 5.f;
    return S_OK;
}

void CSKill_King_ChargeSwing::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	m_pTransformCom->Go_Straight(m_fMoveSpeed, fTimeDelta);
	_float fRadius = m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Get_Radius();
	fRadius += 0.5f * fTimeDelta;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Radius(fRadius);

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Get_Child());
	Vec3 vScale = pChildCollider->Get_Scale();
	vScale.z += 0.4f*fTimeDelta;
	vScale.y += 1.f * fTimeDelta;

	pChildCollider->Set_Scale(Vec3(vScale));
}

void CSKill_King_ChargeSwing::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_King_ChargeSwing::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSKill_King_ChargeSwing::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_ChargeSwing::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_ChargeSwing::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_King_ChargeSwing::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS;
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

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, pCollider);
		}
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Radius(0.5f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Offset(Vec3(0.0f, 0.0f, 0.0f));

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Get_Child());
	pChildCollider->Set_Scale(Vec3(0.2f, 0.5f, 0.2f));
	pChildCollider->Set_Offset(Vec3(0.0f, 0.00f, 0.2f));
	pChildCollider->Set_Orientation(Quaternion::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-5.f)));
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

HRESULT CSKill_King_ChargeSwing::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_King_ChargeSwing* CSKill_King_ChargeSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_King_ChargeSwing* pInstance = new CSKill_King_ChargeSwing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_King_ChargeSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_King_ChargeSwing::Clone(void* pArg)
{
    CSKill_King_ChargeSwing* pInstance = new CSKill_King_ChargeSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_King_ChargeSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_King_ChargeSwing::Free()
{
	__super::Free();
}

