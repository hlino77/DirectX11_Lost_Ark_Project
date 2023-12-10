#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CGameObject(pDevice, pContext, strObjTag, -1)
{
}

CCamera::CCamera(const CCamera & rhs, CTransform::TRANSFORMDESC* pArg)
	: CGameObject(rhs)
{
	m_pTransformCom = (CTransform*)rhs.m_pTransformCom->Clone(this, pArg);
}

HRESULT CCamera::Initialize_Prototype()
{
	m_pTransformCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));

	m_iLayer = m_CameraDesc.iLayer;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));


	m_matProj = XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar);
	m_tCamFrustum = BoundingFrustum(m_matProj);

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	CPipeLine* pPipeLine = CPipeLine::GetInstance();

	Matrix matWorld = m_pTransformCom->Get_WorldMatrix();


	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, matWorld.Invert());
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, m_matProj);


	Vec3 vPos, vScale;
	Quaternion vQuat;

	matWorld.Decompose(vScale, vQuat, vPos);

	m_tCamFrustum.Origin = vPos;
	m_tCamFrustum.Orientation = vQuat;

	pPipeLine->Set_Frustum(m_tCamFrustum);
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Ready_Components()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
