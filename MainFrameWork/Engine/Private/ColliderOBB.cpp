#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderOBB.h"
#include "DebugDraw.h"
#include "Model.h"
#include "ColliderSphere.h"

COBBCollider::COBBCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ColliderType::OBB)
	, m_vScale(Vec3(1.0f, 1.0f, 1.0f))
{
}

COBBCollider::COBBCollider(const COBBCollider& rhs)
	:Super(rhs)
	, m_tBoundingBox(rhs.m_tBoundingBox)
	, m_vScale(rhs.m_vScale)
{

}

HRESULT COBBCollider::Initialize_Prototype()
{
	Super::Initialize_Prototype();

	return S_OK;
}

HRESULT COBBCollider::Initialize(void* pArg)
{
	Set_BoundingBox();
	
	if (pArg == nullptr)
		return E_FAIL;

	ColliderInfo* tCollider = static_cast<ColliderInfo*>(pArg);

	m_iColLayer = tCollider->m_iLayer;
	m_bActive = tCollider->m_bActive;

	return S_OK;
}

void COBBCollider::Tick(const _float& fTimeDelta)
{
	Set_BoundingBox();
}

void COBBCollider::LateTick(const _float& fTimeDelta)
{
}

void COBBCollider::DebugRender()
{
#ifdef _DEBUG
	Super::DebugRender();

	m_pBatch->Begin();

	DX::Draw(m_pBatch, m_tBoundingBox, Colors::Green);

	m_pBatch->End();
#endif // DEBUG
}
_bool COBBCollider::Intersects(SimpleMath::Ray& ray, OUT _float& distance)
{
	return m_tBoundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

_bool COBBCollider::Intersects(Super* other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return m_tBoundingBox.Intersects(static_cast<CSphereCollider*>(other)->GetBoundingSphere());
	//case ColliderType::AABB:
	//	return m_tBoundingSphere.Intersects(static_cast<CAABBCollider*>(other)->GetBoundingBox());
	case ColliderType::OBB:
		return m_tBoundingBox.Intersects(static_cast<COBBCollider*>(other)->GetBoundingBox());
	//case ColliderType::Cylinder:
	//	return m_tBoundingSphere.Intersects(static_cast<CCylinderCollider*>(other)->GetBoundingCylinder());
	}

	return false;
}

_bool COBBCollider::Intersects_Box(const BoundingBox& Collier)
{
	return m_tBoundingBox.Intersects(Collier);
}



void COBBCollider::Set_StaticBoundingBox()
{
	Vec3 vPos = dynamic_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);

	m_tBoundingBox.Center = vPos + m_vOffset;
	m_tBoundingBox.Extents = m_vScale;
	m_tBoundingBox.Orientation = m_vOrientation;
}

void COBBCollider::Set_BoundingBox()
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
		

	m_tBoundingBox.Center = vPos + vOffset;
	m_tBoundingBox.Extents = m_vScale;
	m_tBoundingBox.Orientation = vQuat;
}

COBBCollider* COBBCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COBBCollider* pInstance = new COBBCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COBBCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* COBBCollider::Clone(CGameObject* pGameObject, void* pArg)
{
	COBBCollider* pInstance = new COBBCollider(*this);
	pInstance->m_pOwner = pGameObject;
	//pInstance->m_pRigidBody = pInstance->m_pGameObject->GetRigidBody();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : COBBCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COBBCollider::Free()
{
	Super::Free();
}
