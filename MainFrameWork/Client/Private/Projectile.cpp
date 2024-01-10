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

	if (false == m_bReserveColli && true == m_IsSpawner)
	{
		m_fSpawnAcc += fTimeDelta;
		if (m_fSpawnTime <= m_fSpawnAcc)
		{
			m_fSpawnAcc = 0.f;
			m_iSpawnCnt++;
			Spawn_Projectile(fTimeDelta);
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
			Reset_Projectile();
			CPool<CProjectile>::Return_Obj(this);
		}
		return;
	}

	m_pRendererCom->Add_DebugObject(this);

	m_fCurrTime += fTimeDelta;
	if (m_fCurrTime >= m_fActiveTime)
	{
		AttackEnd();
		m_IsSpawner = false;
	}
	else if (true == m_IsSpawner && m_iSpawnMaxCnt <= m_iSpawnCnt)
	{
		if (-1 != m_iSpawnMaxCnt)
		{
			AttackEnd();
			m_IsSpawner = false;
		}
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

	if (false == pProjectileDesc->IsSpawner)
	{
		InitAsAttack(pProjectileDesc);
	}
	else
	{
		InitAsSpawner(pProjectileDesc);
	}

	return S_OK;
}

void CProjectile::InitAsAttack(PROJECTILE_DESC* pDesc)
{
	m_pAttackOwner = pDesc->pAttackOwner;

	if (Vec3() != pDesc->vAttackPos)
		Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vAttackPos);
	else
		Get_TransformCom()->Set_WorldMatrix(pDesc->AttackMatrix);

	CSphereCollider* pCollider = m_AttackCollider[pDesc->eUseCollider];
	pCollider->Set_ColLayer(pDesc->eLayer_Collider);
	pCollider->Set_Radius(pDesc->fRadius);
	pCollider->Set_Offset(pDesc->vOffset);
	pCollider->SetActive(true);

	if (ATTACKCOLLIDER::OBB == pDesc->eUseCollider)
	{
		COBBCollider* pChildCollider = static_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(pDesc->vChildScale);
		pChildCollider->Set_Offset(pDesc->vChildOffset);
		pChildCollider->SetActive(true);
	}

	if (true == pDesc->IsMove)
	{
		m_IsMove = true;
		m_fMoveSpeed = pDesc->fMoveSpeed;
	}

	m_ProjInfoDesc.iDamage = pDesc->iDamage;
	m_ProjInfoDesc.iStatusEffect = pDesc->iStatusEffect;
	m_ProjInfoDesc.fStatusDuration = pDesc->fStatusDuration;
	m_ProjInfoDesc.fRepulsion = pDesc->fRepulsion;
	m_ProjInfoDesc.bUseProjPos = pDesc->bUseProjPos;
	m_ProjInfoDesc.bUseFactor = pDesc->bUseFactor;
	m_ProjInfoDesc.IsCounter = pDesc->IsCounter;

	Shoot(pDesc->fAttackTime);
}

void CProjectile::InitAsSpawner(PROJECTILE_DESC* pDesc)
{
	m_IsSpawner = true;

	m_pAttackOwner = pDesc->pAttackOwner;

	if (Vec3() != pDesc->vAttackPos)
		Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vAttackPos);
	else
		Get_TransformCom()->Set_WorldMatrix(pDesc->AttackMatrix);

	CSphereCollider* pCollider = m_AttackCollider[SPHERE];
	pCollider->Set_ColLayer((_uint)LAYER_COLLIDER::LAYER_SPAWNER);
	pCollider->Set_Radius(pDesc->fRadius);
	pCollider->Set_Offset(pDesc->vOffset);
	pCollider->SetActive(true);

	if (true == pDesc->IsMove)
	{
		m_IsMove = true;
		m_fMoveSpeed = pDesc->fMoveSpeed;
	}

	if (true == pDesc->IsColliSpawn)
		m_bReserveColli = true;

	if (true == pDesc->bRansdomSpawn)
	{
		m_vRandSpawnFigure = pDesc->vRandSpawnFigure;
		m_IsRandomSpawn = true;
	}

	m_fSpawnAttackTime = pDesc->fSpawnAttackTime;
	

	m_iSpawnUseCol = pDesc->eUseCollider;
	m_iSpawnColLayer = pDesc->eLayer_Collider;
	m_iSpawnMaxCnt = pDesc->iSpawnAmount;
	m_fSpawnTime = pDesc->fSpawnTime;
	m_fSpawnAcc = m_fSpawnTime;

	m_fSpawnRadius = pDesc->fSpawnRadius;
	m_vSpawnOffset = pDesc->vSpawnOffset;

	m_vSpawnChildScale = pDesc->vChildScale;
	m_vSpawnChildOffset = pDesc->vChildOffset;

	m_SpawnProjDesc.iDamage = pDesc->iDamage;
	m_SpawnProjDesc.iStatusEffect = pDesc->iStatusEffect;
	m_SpawnProjDesc.fStatusDuration = pDesc->fStatusDuration;
	m_SpawnProjDesc.fRepulsion = pDesc->fRepulsion;
	m_SpawnProjDesc.bUseProjPos = pDesc->bUseProjPos;
	m_SpawnProjDesc.bUseFactor = pDesc->bUseFactor;
	m_SpawnProjDesc.IsCounter = pDesc->IsCounter;

	Shoot(pDesc->fAttackTime);
}

void CProjectile::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	m_pAttackOwner->OnCollisionEnter(iColLayer, pOther);

	if (true == m_bReserveColli)
	{
		if (OBJ_TYPE::BOSS == pOther->Get_Owner()->Get_ObjectType() || OBJ_TYPE::MONSTER == pOther->Get_Owner()->Get_ObjectType())
		{
			m_bReserveColli = false;
			m_IsMove = false;
		}
	}
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

void CProjectile::Spawn_Projectile(const _float& fTimeDelta)
{
	PROJECTILE_DESC pProjectileDesc;

	pProjectileDesc.pAttackOwner = m_pAttackOwner;

	Matrix matSpawn = m_pTransformCom->Get_WorldMatrix();
	Vec3 vPos = static_cast<CSphereCollider*>(m_AttackCollider[SPHERE])->Get_Center();
	matSpawn._41 = vPos.x; matSpawn._42 = vPos.y; matSpawn._43 = vPos.z;

	pProjectileDesc.AttackMatrix = matSpawn;

	pProjectileDesc.fAttackTime = m_fSpawnAttackTime;

	pProjectileDesc.eUseCollider = m_iSpawnUseCol;
	pProjectileDesc.eLayer_Collider = m_iSpawnColLayer;
	pProjectileDesc.fRadius = m_fSpawnRadius;

	if (true == m_IsRandomSpawn)
	{
		pProjectileDesc.vOffset.x = m_pGameInstance->Random_Float(m_vRandSpawnFigure.x, m_vRandSpawnFigure.y);
		pProjectileDesc.vOffset.y = 0.2f;
		pProjectileDesc.vOffset.z = m_pGameInstance->Random_Float(m_vRandSpawnFigure.z, m_vRandSpawnFigure.w);
	}
	else
		pProjectileDesc.vOffset = m_vSpawnOffset;


	if (ATTACKCOLLIDER::OBB == pProjectileDesc.eUseCollider)
	{
		pProjectileDesc.vChildScale = m_vSpawnChildScale;
		pProjectileDesc.vChildOffset = m_vSpawnChildOffset;
	}

	pProjectileDesc.iDamage = m_SpawnProjDesc.iDamage;
	pProjectileDesc.iStatusEffect = m_SpawnProjDesc.iStatusEffect;
	pProjectileDesc.fStatusDuration = m_SpawnProjDesc.fStatusDuration;
	pProjectileDesc.fRepulsion = m_SpawnProjDesc.fRepulsion;
	pProjectileDesc.bUseProjPos = m_SpawnProjDesc.bUseProjPos;
	pProjectileDesc.bUseFactor = m_SpawnProjDesc.bUseFactor;
	pProjectileDesc.IsCounter = m_SpawnProjDesc.IsCounter;

	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
	pAttack->InitProjectile(&pProjectileDesc);
}

void CProjectile::Reset_Projectile()
{
	m_IsMove = false;

	m_bReserveColli = false;
	m_IsSpawner = false;
	m_IsRandomSpawn = false;

	m_iSpawnCnt = 0;
	m_fSpawnAcc = 0.f;

	ZeroMemory(&m_ProjInfoDesc, sizeof(PROJINFO_DESC));
	ZeroMemory(&m_SpawnProjDesc, sizeof(PROJINFO_DESC));
	Set_Active(false);
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
