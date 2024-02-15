#include "..\Public\PipeLine.h"
#include "..\Public\Light_Manager.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eTransformState, Matrix& TransformMatrix)
{
	m_TransformMatrix[eTransformState] = TransformMatrix;
}

void CPipeLine::Set_MotionBlur(_bool bMotionBlur, _float fMotionBlurIntensity)
{
	m_IsMotionBlur = bMotionBlur;
	if (m_IsMotionBlur)
	{
		m_fMotionBlurIntensity = fMotionBlurIntensity;
	}
	else
	{
		m_fMotionBlurIntensity = 0.0f;
	}
}

void CPipeLine::Set_CascadeBoxes(BoundingOrientedBox* pBoxes)
{
	memcpy(m_tCascadeShadowBox, pBoxes, sizeof(BoundingOrientedBox) * 3);
}

void CPipeLine::Set_ShadowProj(Matrix* pMatrix)
{
	memcpy(m_matShadowProj, pMatrix, sizeof(Matrix) * 3);
}

void CPipeLine::Update()
{
	for (_uint i = 0; i < D3DTS_END; ++i)	
		m_TransformInverseMatrix[i] = m_TransformMatrix[i].Invert();

	m_vCamPosition = Vec4(m_TransformInverseMatrix[D3DTS_VIEW].m[3]);
	m_ViewProjMatrix = m_TransformMatrix[D3DTS_VIEW] * m_TransformMatrix[D3DTS_PROJ];

	CLight_Manager* pLightManager = GET_INSTANCE(CLight_Manager);
	m_LightViewProjMatrix = pLightManager->Get_DirectionLightMatrix() * m_TransformMatrix[D3DTS_PROJ];
	RELEASE_INSTANCE(CLight_Manager);
}

void CPipeLine::Free()
{

}
