#include "..\Public\Camera.h"
#include "PipeLine.h"
#include "Light_Manager.h"

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

	Matrix matView = matWorld.Invert();
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, matView);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, m_matProj);

	Vec3 vPos, vScale;
	Quaternion vQuat;

	matWorld.Decompose(vScale, vQuat, vPos);

	m_tCamFrustum.Origin = vPos;
	m_tCamFrustum.Orientation = vQuat;

	pPipeLine->Set_Frustum(m_tCamFrustum);
	Update_Cascade();
}

void CCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Update_Cascade()
{
	// 카메라의 역행렬과 시야각,화면비, 가까운 평면 Z,먼 평면 Z
	Matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	Matrix matView = matWorld.Invert();


	//시야각을 이용하여 수직 시야각을 구함
	float tanHalfVFov = tanf(m_CameraDesc.fFovy / 2.0f);
	// 수직 시야각을 이용하여 수평 시야각을 구함
	float tanHalfHFov = tanHalfVFov * m_CameraDesc.fAspect;

	// 절두체를 나누기위한 각 부분 절두체의 끝 지점 선언
	m_fCascadeEnd[0] = m_CameraDesc.fNear;
	m_fCascadeEnd[1] = 15.f;
	m_fCascadeEnd[2] = 30.f;
	m_fCascadeEnd[3] = 60.f;

	// 3개의 절두체로 나누기위해 3번 반복함
	for (uint32_t i = 0; i < 3; ++i)
	{

		//+X,+Y 좌표에 수평,수직 시야각을 이용하여 구함. 각 부분 절두체의 가까운,먼
		//평면의 값을 곱하여 4개의 점을 구함
		float xn = m_fCascadeEnd[i] * tanHalfHFov;
		float xf = m_fCascadeEnd[i + 1] * tanHalfHFov;
		float yn = m_fCascadeEnd[i] * tanHalfVFov;
		float yf = m_fCascadeEnd[i + 1] * tanHalfVFov;


		//+좌표값을 구하면 -좌표값을 구하여 각각의 절두체 평면을 구할수있음.
		//각 절두체의 Z값을 저장하여 i 가 낮은 순서로 가까운 평면 먼평면을 구성함
		Vec4 frustumCorners[8] =
		{
			//near Face
			{xn,yn,m_fCascadeEnd[i],1.0f},
			{-xn,yn,m_fCascadeEnd[i],1.0f},
			{xn,-yn,m_fCascadeEnd[i],1.0f},
			{-xn,-yn,m_fCascadeEnd[i],1.0f},
			//far Face
			{xf,yf,m_fCascadeEnd[i + 1],1.0f},
			{-xf,yf,m_fCascadeEnd[i + 1],1.0f},
			{xf,-yf,m_fCascadeEnd[i + 1],1.0f},
			{-xf,-yf,m_fCascadeEnd[i + 1],1.0f}
		};

		Vec4 centerPos;
		for (uint32_t j = 0; j < 8; ++j)
		{
			frustumCorners[j] = XMVector4Transform(frustumCorners[j], matWorld);
			centerPos += frustumCorners[j];
		}

		centerPos /= 8.0f;
		float radius = 0.0f;

		for (uint32_t j = 0; j < 8; ++j)
		{
			float distance = (frustumCorners[j] - centerPos).Length();
			radius = max(radius, distance);
		}

		radius = ceil(radius * 16.0f) / 16.0f;

		// using radius ,  we made aabb box
		Vec3 maxExtents(radius, radius, radius);
		Vec3 minExtents = -maxExtents;

		Vec3 vLightDir = CLight_Manager::GetInstance()->Get_LightDir();
		Vec3 shadowCamPos = Vec3(centerPos) + (vLightDir * minExtents.z);
		Matrix lightMatrix = Matrix::CreateWorld(shadowCamPos, -vLightDir, Vec3(0.0f, 1.0f, 0.0f));
		Matrix lightInv = lightMatrix.Invert();

		Vec3 cascadeExtents = maxExtents - minExtents;

		Matrix matProj = XMMatrixOrthographicLH(cascadeExtents.x, cascadeExtents.y, 0.0f, cascadeExtents.z);
		m_shadowOrthoProj[i] = lightInv * matProj;

		Vec3 vPos, vScale;
		Quaternion vQuat;
		lightMatrix.Decompose(vScale, vQuat, vPos);
		m_tCascadeShadowBox[i].Orientation = vQuat;
		m_tCascadeShadowBox[i].Center = Vec3(centerPos);
		m_tCascadeShadowBox[i].Extents = maxExtents;
	}

	CPipeLine::GetInstance()->Set_ShadowProj(m_shadowOrthoProj);
	CPipeLine::GetInstance()->Set_CascadeBoxes(m_tCascadeShadowBox);
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
