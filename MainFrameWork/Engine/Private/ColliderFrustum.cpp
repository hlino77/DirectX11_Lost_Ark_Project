#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderFrustum.h"
#include "DebugDraw.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderSphereGroup.h"
#include "ColliderOBB.h"

CFrustumCollider::CFrustumCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ColliderType::Frustum)
{
}

CFrustumCollider::CFrustumCollider(const CFrustumCollider& rhs)
	:Super(rhs)
	, m_tBoundingFrustum(rhs.m_tBoundingFrustum)
{

}

HRESULT CFrustumCollider::Initialize_Prototype()
{
	Super::Initialize_Prototype();

	return S_OK;
}

HRESULT CFrustumCollider::Initialize(void* pArg)
{
	Set_BoundingFrustum();
	
	if (pArg == nullptr)
		return E_FAIL;

	ColliderInfo* tCollider = static_cast<ColliderInfo*>(pArg);

	m_iColLayer = tCollider->m_iLayer;
	m_bActive = tCollider->m_bActive;

	return S_OK;
}

void CFrustumCollider::Update_Collider()
{
	Set_BoundingFrustum();
}


void CFrustumCollider::DebugRender()
{
#ifdef _DEBUG
	Super::DebugRender();

	m_pBatch->Begin();

	DX::Draw(m_pBatch, m_tBoundingFrustum, Colors::Green);

	m_pBatch->End();
#endif // DEBUG
}
_bool CFrustumCollider::Intersects(SimpleMath::Ray& ray, OUT _float& distance)
{
	return m_tBoundingFrustum.Intersects(ray.position, ray.direction, OUT distance);
}

_bool CFrustumCollider::Intersects(Super* other)
{
	if (Intersects_Bounding(other) == false)
		return false;

	if (other->Get_Child())
	{
		if (Intersects_Bounding(other->Get_Child()) == false)
			return false;
	}

	return true;
}

_bool CFrustumCollider::Intersects_Bounding(Super* other)
{
	_bool bCollision = false;

	switch (other->GetColliderType())
	{
	case ColliderType::Sphere:
		bCollision = m_tBoundingFrustum.Intersects(static_cast<CSphereCollider*>(other)->GetBoundingSphere());
		break;
	case ColliderType::OBB:
		bCollision = m_tBoundingFrustum.Intersects(static_cast<COBBCollider*>(other)->GetBoundingBox());
		break;
	case ColliderType::AABB:
		bCollision = false;
		break;
	case ColliderType::Frustum:
		bCollision = m_tBoundingFrustum.Intersects(static_cast<CFrustumCollider*>(other)->GetBoundingFrustum());;
		break;
	case ColliderType::Group:
		bCollision = other->Intersects(this);
		break;
	}


	return bCollision;
}

_bool CFrustumCollider::Intersects_Box(const BoundingBox& Collier)
{
	return false;
}



void CFrustumCollider::Set_BoundingFrustum()
{
	CTransform* pTransform = m_pOwner->Get_TransformCom();

	Matrix	matWorld = pTransform->Get_WorldMatrix();

	Vec3 vPos, vScale;
	Quaternion vQuat;

	matWorld.Decompose(vScale, vQuat, vPos);

	Vec3 vOffset(0.0f, 0.0f, 0.0f);

	if (m_vOffset != Vec3(0.0f, 0.0f, 0.0f))
	{
		vOffset = XMVector3TransformNormal(m_vOffset, matWorld);
		vOffset.Normalize();
		vOffset *= m_vOffset.Length();
	}
	vQuat= Quaternion::Concatenate(vQuat, m_vOrientation);
	m_tBoundingFrustum.Near = m_fNear;
	m_tBoundingFrustum.Far = m_fFar;
	m_tBoundingFrustum.RightSlope = m_vSlopesTopBottomRightLeft.z;
	m_tBoundingFrustum.LeftSlope = m_vSlopesTopBottomRightLeft.w;
	m_tBoundingFrustum.TopSlope = m_vSlopesTopBottomRightLeft.x;
	m_tBoundingFrustum.BottomSlope = m_vSlopesTopBottomRightLeft.y;
	m_tBoundingFrustum.Origin = vPos + vOffset;
	m_tBoundingFrustum.Orientation = vQuat;
}

CFrustumCollider* CFrustumCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFrustumCollider* pInstance = new CFrustumCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COBBCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CFrustumCollider::Clone(CGameObject* pGameObject, void* pArg)
{
	CFrustumCollider* pInstance = new CFrustumCollider(*this);
	pInstance->m_pOwner = pGameObject;
	//pInstance->m_pRigidBody = pInstance->m_pGameObject->GetRigidBody();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : COBBCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFrustumCollider::Free()
{
	Super::Free();
}
