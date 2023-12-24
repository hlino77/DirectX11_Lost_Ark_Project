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
	if (true == m_IsMove)
		m_pTransformCom->Go_Straight(m_fMoveSpeed, fTimeDelta);

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
	if (m_bEnd)
	{
		m_fCurrTime -= fTimeDelta;
		if (m_fCurrTime < 0.0f)
		{
			for (_uint i = 0; i < COLEND; ++i)
			{
				if (m_bColliderOut[i])
					CCollisionManager::GetInstance()->Out_Colider(m_AttackCollider[i]);
			}
			if (true == m_IsMove) 
				m_IsMove = false;

			ZeroMemory(&m_ProjInfoDesc, sizeof(PROJINFO_DESC));
			Set_Active(false);
			CPool<CProjectile>::Return_Obj(this);
		}
		return;
	}

	m_pRendererCom->Add_DebugObject(this);

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

HRESULT CProjectile::InitProjectile(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PROJECTILE_DESC* pProjectileDesc = (PROJECTILE_DESC*)pArg;
	m_pAttackOwner = pProjectileDesc->pAttackOwner;

	if (Vec3() != pProjectileDesc->vAttackPos)
		Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pProjectileDesc->vAttackPos);
	else
		Get_TransformCom()->Set_WorldMatrix(m_pAttackOwner->Get_TransformCom()->Get_WorldMatrix());

	CSphereCollider* pCollider = m_AttackCollider[pProjectileDesc->eUseCollider];
	pCollider->Set_ColLayer(pProjectileDesc->eLayer_Collider);
	pCollider->Set_Radius(pProjectileDesc->fRadius);
	pCollider->Set_Offset(pProjectileDesc->vOffset);
	pCollider->SetActive(true);

	if (ATTACKCOLLIDER::OBB == pProjectileDesc->eUseCollider)
	{
		COBBCollider* pChildCollider = static_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(pProjectileDesc->vChildScale);
		pChildCollider->Set_Offset(pProjectileDesc->vChildOffset);
		pChildCollider->SetActive(true);
	}

	if (true == pProjectileDesc->IsMove)
	{
		m_IsMove = true;
		m_fMoveSpeed = pProjectileDesc->fMoveSpeed;
	}
	
	m_ProjInfoDesc.iDamage = pProjectileDesc->iDamage;
	m_ProjInfoDesc.iStatusEffect = pProjectileDesc->iStatusEffect;
	m_ProjInfoDesc.fStatusDuration = pProjectileDesc->fStatusDuration;
	m_ProjInfoDesc.fRepulsion = pProjectileDesc->fRepulsion;
	m_ProjInfoDesc.bUseProjPos = pProjectileDesc->bUseProjPos;
	m_ProjInfoDesc.bUseFactor = pProjectileDesc->bUseFactor;

	Shoot(pProjectileDesc->fAttackTime);

	return S_OK;
}

void CProjectile::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	m_pAttackOwner->OnCollisionEnter(iColLayer, pOther);
}

void CProjectile::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
	m_pAttackOwner->OnCollisionStay(iColLayer, pOther);
}

void CProjectile::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	m_pAttackOwner->OnCollisionExit(iColLayer, pOther);
}

void CProjectile::Shoot(_float fActiveTime)
{
	m_bEnd = false;
	m_fActiveTime = fActiveTime;
	m_fCurrTime = 0.0f;

	for (_uint i = 0; i < COLEND; ++i)
	{
		m_bColliderOut[i] = false;
		if (m_AttackCollider[i]->IsActive())
		{
			m_AttackCollider[i]->Update_Collider();
			CCollisionManager::GetInstance()->Add_Colider(m_AttackCollider[i]);
			m_bColliderOut[i] = true;
		}
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
	m_bEnd = true;
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
