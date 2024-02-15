#pragma once

#include "Base.h"
#include "AsTypes.h"

BEGIN(Engine)

class CPipeLine	final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void	Set_Transform(TRANSFORMSTATE eTransformState, Matrix& TransformMatrix);

	const Matrix&	Get_TransformMatrix(TRANSFORMSTATE eTransformState) const			{ return m_TransformMatrix[eTransformState]; }
	const Matrix&	Get_ViewProjMatrix() const											{ return m_ViewProjMatrix; }
	const Matrix&	Get_LightViewProjMatrix() const										{ return m_LightViewProjMatrix; }
	Matrix	Get_TransformFloat4x4_TP(TRANSFORMSTATE eTransformState) const				{ return Get_TransformMatrix(eTransformState).Transpose(); }
	const Matrix&	Get_TransformMatrixInverse(TRANSFORMSTATE eTransformState) const	{ return m_TransformInverseMatrix[eTransformState]; }
	const Vec4&	Get_CamPosition() const													{ return m_vCamPosition; }
	
	_bool	Is_MotionBlur()														{ return m_IsMotionBlur; }
	void	Set_MotionBlur(_bool bMotionBlur, _float fMotionBlurIntensity = 0.0f);

	_float	Get_MotionBlurIntensity() { return m_fMotionBlurIntensity; }

	const BoundingFrustum& Get_CamFrustum()										{ return m_tCamFrustum; }
	void Set_Frustum(const BoundingFrustum& tBoundingFrustum)					{ m_tCamFrustum = tBoundingFrustum; }


	void	Set_CascadeBoxes(BoundingOrientedBox* pBoxes);
	void	Set_ShadowProj(Matrix* pMatrix);

	BoundingOrientedBox* Get_CascadeBoxes() { return m_tCascadeShadowBox; }
	Matrix* Get_ShadowProj() { return m_matShadowProj; }
public:
	void Update();
	
private:
	Matrix				m_TransformMatrix[D3DTS_END];
	Matrix				m_ViewProjMatrix = Matrix::Identity;
	Matrix				m_LightViewProjMatrix = Matrix::Identity;
	Matrix				m_TransformInverseMatrix[D3DTS_END];
	Vec4				m_vCamPosition;

	BoundingFrustum		m_tCamFrustum;
	_float				m_fMotionBlurIntensity = 0.0f;

	_bool				m_IsMotionBlur = false;

	BoundingOrientedBox	m_tCascadeShadowBox[3];
	Matrix m_matShadowProj[3];
public:
	virtual void Free() override;
};

END