#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderSphereGroup.h"
#include "DebugDraw.h"
#include "Model.h"
#include "ColliderOBB.h"

CSphereColliderGroup::CSphereColliderGroup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ColliderType::Group)
{
}

CSphereColliderGroup::CSphereColliderGroup(const CSphereColliderGroup& rhs)
	:Super(rhs)
{

}

HRESULT CSphereColliderGroup::Initialize_Prototype()
{
	Super::Initialize_Prototype();

	return S_OK;
}

HRESULT CSphereColliderGroup::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	ColliderInfo* tCollider = static_cast<ColliderInfo*>(pArg);

	m_iColLayer = tCollider->m_iLayer;
	m_bActive = tCollider->m_bActive;

	return S_OK;
}

void CSphereColliderGroup::Update_Collider()
{
	for (auto& Collider : m_Colliders)
	{
		Collider->Update_Collider();
	}
}


void CSphereColliderGroup::DebugRender()
{
#ifdef _DEBUG
	
	for (auto& Collider : m_Colliders)
	{
		Collider->DebugRender();
	}

#endif // DEBUG
}
_bool CSphereColliderGroup::Intersects(SimpleMath::Ray& ray, OUT _float& distance)
{
	return false;
}

_bool CSphereColliderGroup::Intersects(Super* other)
{
	_uint iIndex = 0;

	for (auto& Collider : m_Colliders)
	{
		if (Collider->Intersects(other))
		{
			m_iCollisionIndex = iIndex;
			return true;
		}
		++iIndex;
	}

	return false;
}

_bool CSphereColliderGroup::Intersects_Bounding(Super* other)
{
	return false;
}

_bool CSphereColliderGroup::Intersects_Box(const BoundingBox& Collider)
{
	return false;
}

void CSphereColliderGroup::Add_Collider(CCollider* pCollider)
{
	m_Colliders.push_back(pCollider);
}

CCollider* CSphereColliderGroup::Get_Collider(_uint iIndex)
{
	return m_Colliders[iIndex];
}




CSphereColliderGroup* CSphereColliderGroup::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphereColliderGroup* pInstance = new CSphereColliderGroup(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSphereColliderGroup");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CSphereColliderGroup::Clone(CGameObject* pGameObject, void* pArg)
{
	CSphereColliderGroup* pInstance = new CSphereColliderGroup(*this);
	pInstance->m_pOwner = pGameObject;
	//pInstance->m_pRigidBody = pInstance->m_pGameObject->GetRigidBody();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSphereColliderGroup");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSphereColliderGroup::Free()
{
	Super::Free();
}
