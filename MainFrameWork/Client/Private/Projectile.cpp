#include "stdafx.h"
#include "GameInstance.h"
#include "Projectile.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "ColliderSphereGroup.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "Pool.h"

CProjectile::CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Projectile", OBJ_TYPE::PROJECTILE)
{
}

CProjectile::CProjectile(const CProjectile& rhs)
	: CGameObject(rhs)
{
}

HRESULT CProjectile::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CProjectile::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if(m_pRigidBody)
		m_pRigidBody->SetMass(2.0f);
	

	m_bActive = false;

    return S_OK;
}

void CProjectile::Tick(_float fTimeDelta)
{
	for (auto& Collider : m_AttackCollider)
	{
		if (Collider->IsActive())
		{
			Collider->Update_Collider();
		}
	}
}

void CProjectile::LateTick(_float fTimeDelta)
{
	m_fCurrTime += fTimeDelta;
	if (m_fCurrTime >= m_fActiveTime)
	{
		AttackEnd();
	}
}

HRESULT CProjectile::Render_Debug()
{
	for (auto& Collider : m_AttackCollider)
	{
		if (Collider->IsActive())
		{
			Collider->DebugRender();
		}
	}

	return S_OK;
}

void CProjectile::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CProjectile::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CProjectile::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}


void CProjectile::Shoot(_float fActiveTime, _bool bSphere, _bool bOBB)
{
	m_fActiveTime = fActiveTime;
	m_fCurrTime = 0.0f;
	
	if (bSphere)
	{
		m_AttackCollider[ATTACKCOLLIDER::SPHERE]->Update_Collider();
		m_AttackCollider[ATTACKCOLLIDER::SPHERE]->SetActive(true);
	}

	if (bOBB)
	{
		m_AttackCollider[ATTACKCOLLIDER::OBB]->Update_Collider();
		m_AttackCollider[ATTACKCOLLIDER::OBB]->SetActive(true);
	}

	Set_Active(true);
}

void CProjectile::AttackEnd()
{
	for (auto& Collider : m_AttackCollider)
	{
		if (Collider->IsActive())
		{
			Collider->SetActive(false);
		}
	}
	Set_Active(false);
	CPool<CProjectile>::Return_Obj(this);
}

HRESULT CProjectile::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;



	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_END;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSphere"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		if (pCollider)
			m_AttackCollider[ATTACKCOLLIDER::SPHERE] = pCollider;
	}



	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_END;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderOBBParent"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;


		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = false;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_END;
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderOBBChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_AttackCollider[ATTACKCOLLIDER::OBB] = pCollider;
		}
	}

	
	Safe_Release(pGameInstance);
    return S_OK;
}





void CProjectile::Free()
{
	__super::Free();



	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}

CProjectile* CProjectile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CProjectile* pInstance = new CProjectile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CProjectile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CProjectile::Clone(void* pArg)
{
	CProjectile* pInstance = new CProjectile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CProjectile");
		Safe_Release(pInstance);
	}

	return pInstance;
}
