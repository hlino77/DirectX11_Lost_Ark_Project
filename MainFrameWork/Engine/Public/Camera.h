#pragma once

/* 클라에서 정의할 카메라들의 부모. */
/* 모든 카메라가 가져야할 기본적인 기능을 보유한다. */
#include "GameObject.h"
#include "LockFree_Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		SimpleMath::Vector4		vEye, vAt;
		_float					fFovy, fAspect, fNear, fFar;
		_uint					iLayer;

		CTransform::TRANSFORMDESC		TransformDesc;
	}CAMERADESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera(const CCamera& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();




protected:
	CAMERADESC					m_CameraDesc;
	Matrix						m_matProj;


	BoundingFrustum				m_tCamFrustum;
protected:
	virtual HRESULT Ready_Components() override;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END