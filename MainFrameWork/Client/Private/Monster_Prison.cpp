#include "stdafx.h"
#include "Monster_Prison.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"



CMonster_Prison::CMonster_Prison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CMonster_Prison::CMonster_Prison(const CMonster& rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Prison::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Prison::Initialize(void* pArg)
{
	m_iMaxHp = 10;
	m_iHp = m_iMaxHp;
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_szModelName = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_bInstance = Desc->bInstance;
	m_iCurrLevel = Desc->iLevel;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);

	m_fMoveSpeed = 1.5f;

	if (FAILED(Ready_HpUI()))
		return E_FAIL;


	m_iAtk = 0;
	m_vecAttackRanges.clear();
	m_vecAttackRanges.push_back(0.f);
	m_vecAttackRanges.push_back(0.f);
	m_fAttackRange = m_vecAttackRanges[0];
	m_fNoticeRange = 0.f;

	return S_OK;
}

void CMonster_Prison::Tick(_float fTimeDelta)
{
}

void CMonster_Prison::LateTick(_float fTimeDelta)
{
	Set_Colliders(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;
	m_pRendererCom->Add_DebugObject(this);
}

HRESULT CMonster_Prison::Render()
{

	return S_OK;
}

void CMonster_Prison::Set_SlowMotion(_bool bSlow)
{
}

void CMonster_Prison::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			Send_Collision(1, Vec3(), STATUSEFFECT::EFFECTEND, 0.f, 0.f, 0);
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			Send_Collision(1, Vec3(), STATUSEFFECT::EFFECTEND, 0.f, 0.f, 0);
		}
	}
}

void CMonster_Prison::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Prison::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Prison::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
	m_iHp -= iDamage;
	if (m_iHp < 1)
		Set_Die();
}

HRESULT CMonster_Prison::Ready_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Radius(0.6f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER]->Set_Offset(Vec3(0.0f, 0.5f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;

}

HRESULT CMonster_Prison::Ready_BehaviourTree()
{
	return S_OK;
}

CMonster_Prison* CMonster_Prison::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Prison* pInstance = new CMonster_Prison(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_Prison");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Prison::Clone(void* pArg)
{
	CMonster_Prison* pInstance = new CMonster_Prison(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CMonster_Prison");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMonster_Prison::Free()
{
	__super::Free();
	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);
}
