#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderDoughnut.h"
#include "DebugDraw.h"
#include "Model.h"
#include "ColliderOBB.h"
#include "ColliderSphereGroup.h"
#include "ColliderFrustum.h"
#include <ColliderSphere.h>

CDoughnutCollider::CDoughnutCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ColliderType::Doughnut)
{
}

CDoughnutCollider::CDoughnutCollider(const CDoughnutCollider& rhs)
	:Super(rhs)
	, m_tBoundingSphere(rhs.m_tBoundingSphere)
{

}

HRESULT CDoughnutCollider::Initialize_Prototype()
{
	Super::Initialize_Prototype();

	return S_OK;
}

HRESULT CDoughnutCollider::Initialize(void* pArg)
{
	m_tBoundingSphere.Center = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
	m_tBoundingSphere.Radius = 1.0f;

	if (pArg == nullptr)
		return E_FAIL;

	ColliderInfo* tCollider = static_cast<ColliderInfo*>(pArg);

	m_iColLayer = tCollider->m_iLayer;
	m_bActive = tCollider->m_bActive;

	return S_OK;
}

void CDoughnutCollider::Update_Collider()
{
	Set_Center();
}


void CDoughnutCollider::DebugRender()
{
#ifdef _DEBUG
	Super::DebugRender();

	m_pBatch->Begin();

	DX::Draw(m_pBatch, m_tBoundingSphere, Colors::Green);

	m_pBatch->End();


	if (m_pChild)
		m_pChild->DebugRender();
#endif // DEBUG
}
_bool CDoughnutCollider::Intersects(SimpleMath::Ray& ray, OUT _float& distance)
{
	if (m_tBoundingSphere.Intersects(ray.position, ray.direction, OUT distance) == false)
		return false;

	return true;
}

_bool CDoughnutCollider::Intersects(Super* other)
{
	if (Intersects_Bounding(other) == true)
		return false;

	if(other->Get_Child())
	{
		if (Intersects_Bounding(other->Get_Child()) == true)
			return false;
	}



	return true;
}

_bool CDoughnutCollider::Intersects_Bounding(Super* other)
{
	_bool bCollision = false;

	switch (other->GetColliderType())
	{
	case ColliderType::Sphere:
		bCollision = m_tBoundingSphere.Intersects(static_cast<CSphereCollider*>(other)->GetBoundingSphere());
		break;
	case ColliderType::OBB:
		bCollision = m_tBoundingSphere.Intersects(static_cast<COBBCollider*>(other)->GetBoundingBox());
		break;
	case ColliderType::AABB:
		bCollision = false;
		break;
	case ColliderType::Frustum:
		bCollision = m_tBoundingSphere.Intersects(static_cast<CFrustumCollider*>(other)->GetBoundingFrustum());
		break;
	case ColliderType::Group:
		bCollision = other->Intersects(this);
		break;
	case ColliderType::Doughnut:
		bCollision = m_tBoundingSphere.Intersects(static_cast<CDoughnutCollider*>(other)->GetBoundingSphere());
		break;
	}

	return !bCollision;
}

_bool CDoughnutCollider::Intersects_Box(const BoundingBox& Collider)
{
	if (m_tBoundingSphere.Intersects(Collider))
		return false;


	return true;
}

void CDoughnutCollider::Set_Center()
{
	Matrix matObjectWorld = m_pOwner->Get_TransformCom()->Get_WorldMatrix();

	if (m_vOffset != Vec3(0.0f, 0.0f, 0.0f))
	{
		Vec3 vPos(0.0f, 0.0f, 0.0f);
		vPos = XMVector3TransformNormal(m_vOffset, matObjectWorld);
		vPos.Normalize();
		vPos *= m_vOffset.Length();
		vPos += Vec3(matObjectWorld.m[3]);
		m_tBoundingSphere.Center = vPos;
	}
	else
	{
		m_tBoundingSphere.Center = Vec3(matObjectWorld.m[3]);
	}
}

void CDoughnutCollider::Set_Center_ToBone()
{
	Matrix matOwnerWolrd = m_pOwner->Get_TransformCom()->Get_WorldMatrix();

	Matrix matBone = m_pOwner->Get_ModelCom()->Get_CurrBoneMatrix(m_iBoneIndex);

	Matrix matResult = matBone * matOwnerWolrd;
	if (m_vOffset != Vec3(0.0f, 0.0f, 0.0f))
	{
		Vec3 vPos(0.0f, 0.0f, 0.0f);
		vPos = XMVector3TransformNormal(m_vOffset, matResult);
		vPos.Normalize();
		vPos *= m_vOffset.Length();
		vPos += Vec3(matResult.m[3]);
		m_tBoundingSphere.Center = vPos;
	}
	else
	{
		m_tBoundingSphere.Center = Vec3(matResult.m[3]);
	}
}


CDoughnutCollider* CDoughnutCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDoughnutCollider* pInstance = new CDoughnutCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDoughnutCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CDoughnutCollider::Clone(CGameObject* pGameObject, void* pArg)
{
	CDoughnutCollider* pInstance = new CDoughnutCollider(*this);
	pInstance->m_pOwner = pGameObject;
	//pInstance->m_pRigidBody = pInstance->m_pGameObject->GetRigidBody();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDoughnutCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDoughnutCollider::Free()
{
	Super::Free();
}
