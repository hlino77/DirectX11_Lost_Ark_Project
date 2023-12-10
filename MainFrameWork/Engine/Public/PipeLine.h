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
	void Set_Transform(TRANSFORMSTATE eTransformState, Matrix TransformMatrix);

	Matrix Get_TransformMatrix(TRANSFORMSTATE eTransformState) const {
		return m_TransformMatrix[eTransformState];
	}


	Matrix Get_TransformFloat4x4_TP(TRANSFORMSTATE eTransformState) const {
		return Get_TransformMatrix(eTransformState).Transpose();
	}

	Matrix Get_TransformMatrixInverse(TRANSFORMSTATE eTransformState) const {
		return m_TransformInverseMatrix[eTransformState];
	}

	Vec4 Get_CamPosition() const {
		return m_vCamPosition;
	}

	const BoundingFrustum& Get_CamFrustum() { return m_tCamFrustum; }
	void	Set_Frustum(const BoundingFrustum& tBoundingFrustum) { m_tCamFrustum = tBoundingFrustum; }
public:
	void Update();
	
private:
	Matrix				m_TransformMatrix[D3DTS_END];
	Matrix				m_TransformInverseMatrix[D3DTS_END];
	Vec4					m_vCamPosition;

	BoundingFrustum		m_tCamFrustum;
public:
	virtual void Free() override;
};

END