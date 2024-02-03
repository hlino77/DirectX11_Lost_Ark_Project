#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Cut final : public CCamera
{
public:
	typedef struct tagCutFrame
	{
		Vec3	vOffset;
		_float  fTargetCameraLength = 1.f;
		_float	fZoomSpeed = 1.0f;

		Vec3	vTargetOffset;
	}CUTFRAME;

	typedef struct tagCutCameraDesc
	{
		class CGameObject* pCutTarget = nullptr;
		CCamera::CAMERADESC tCameraDesc;
		Vec3 vOffset = Vec3(-1.0f, 0.0f, -1.0f);
		_float fDefaultLength = 2.f;
	}CUTCAMERADESC;

public:
	enum class CAMERATYPE { OFFSET, LOOKTARGET, _END };

private:
	CCamera_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Cut(const CCamera_Cut& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera_Cut() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void		Cam_Shake(_float fFirstShake, _float fForce, _float fTime, _float fBreak);
	void		ZoomInOut(_float fCameraLength, _float fSpeed) { m_fTargetCameraLength = fCameraLength; m_fZoomSpeed = fSpeed; }
	void		Set_CameraLength(_float fCameraLength) { m_fCameraLength = m_fTargetCameraLength = fCameraLength; }
	void		Set_LengthToDefaultLength() { m_fTargetCameraLength = m_fDefaultLength; m_fZoomSpeed = 1.f; }

	void		Set_Offset(Vec3 vOffset) { m_vOffset = vOffset; m_vOffset.Normalize(); m_tLerpOffset.bActive = false; }
	void		Set_DefaultOffset(Vec3 vOffset) { m_vDefaultOffset = vOffset; m_vDefaultOffset.Normalize(); }
	void		Set_OffsetToDefaultOffset() { m_vOffset = m_vDefaultOffset; }
	void		Set_Lerp_Offset(_float fEndTime, Vec3 vTargetOffset, LERP_MODE eMode = LERP_MODE::DEFAULT) {
		m_vDefaultOffset = m_vOffset;
		m_tLerpOffset.Init_Lerp(fEndTime, m_vOffset, vTargetOffset, eMode);
	}

	// Target
	void		Set_Direct_TargetOffset(Vec3 vOffset) { m_vTargetOffset = vOffset; m_tLerpTarget.bActive = false; }
	void		Set_Lerp_TargetOffset(_float fEndTime, Vec3 vTargetOffset, LERP_MODE eMode = LERP_MODE::DEFAULT) {
		m_vDefaultTargetOffset = m_vTargetOffset;
		m_tLerpTarget.Init_Lerp(fEndTime, m_vTargetOffset, vTargetOffset, eMode);
	}
	void		Set_TargetOffsetToDefaultTargetOffset(Vec3 vOffset) { m_vTargetOffset = m_vDefaultTargetOffset; }

	// CutMode
	void		Set_CutMode(_uint iMode) { m_iState = iMode; }

	_float		Get_CameraLength() { return m_fCameraLength; }

private:
	virtual HRESULT Ready_Components() override;

private:
	void		Tick_Offset(_float fTimeDelta);
	void		Tick_LookTarget(_float fTimeDelta);

	void		Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta);

private:
	
	// Camera
	_uint m_iState = (_uint)CAMERATYPE::OFFSET;

	Vec3		m_vOffset;
	LERP_VEC3	m_tLerpOffset;
	Vec3		m_vDefaultOffset;

	_float m_fZoomSpeed = 1.0f;
	_float m_fDefaultLength;
	_float m_fCameraLength;
	_float m_fTargetCameraLength;

	//CamShake
	_float	m_fShakeTime = 0.0f;
	_float	m_fCurrShakeTime = 0.0f;
	_float	m_fShakeForce = 0.0f;
	_float	m_fBreak = 0.0f;
	_bool	m_bShake = false;

	Vec2	m_vShakeVelocity;
	Vec2	m_vShakeOffset;

	//TargetOffset
	class CGameObject* m_pTarget = nullptr;
	Vec3	m_vDefaultTargetOffset;
	Vec3	m_vTargetOffset;
	LERP_VEC3	m_tLerpTarget;
	

	vector<CUTFRAME> m_vecCutFrame;

public:
	static CCamera_Cut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END