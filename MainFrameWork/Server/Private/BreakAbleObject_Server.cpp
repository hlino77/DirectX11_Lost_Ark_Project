#include "stdafx.h"
#include "BreakAbleObject_Server.h"
#include "GameInstance.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "GameSession.h"

#include "NavigationMgr.h"

CBreakAbleObject_Server::CBreakAbleObject_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"BreakAbleObject", OBJ_TYPE::PROP)
{
}

CBreakAbleObject_Server::CBreakAbleObject_Server(const CBreakAbleObject_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBreakAbleObject_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBreakAbleObject_Server::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iCurrLevel = Desc->iCurrentLevel;


	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CBreakAbleObject_Server::Tick(_float fTimeDelta)
{


}

void CBreakAbleObject_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CBreakAbleObject_Server::Render()
{
	return E_NOTIMPL;
}

void CBreakAbleObject_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL && pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)
	{
		Set_Dead(true);


		for (auto& CellIndex : m_NaviCellIndex)
		{
			CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, true);
		}
	}

}

void CBreakAbleObject_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CBreakAbleObject_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CBreakAbleObject_Server::Add_Collider()
{
	CSphereCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CSphereCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<CSphereCollider*>(pComponent);
	if (nullptr == pCollider)
		return;

	m_StaticColliders.push_back(pCollider);

	Safe_Release(pGameInstance);

}

void CBreakAbleObject_Server::Add_ChildCollider(_uint iIndex)
{
	COBBCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<COBBCollider*>(pComponent);
	if (nullptr == pCollider)
		return;


	m_StaticColliders[iIndex]->Set_Child(pCollider);

	Safe_Release(pGameInstance);
}

void CBreakAbleObject_Server::Add_CollidersToManager()
{
	for (auto& Collider : m_StaticColliders)
	{
		if (Collider)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider);
		}
	}
}

HRESULT CBreakAbleObject_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

CBreakAbleObject_Server* CBreakAbleObject_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBreakAbleObject_Server* pInstance = new CBreakAbleObject_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBreakAbleObject_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBreakAbleObject_Server::Clone(void* pArg)
{
	CBreakAbleObject_Server* pInstance = new CBreakAbleObject_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBreakAbleObject_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBreakAbleObject_Server::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
