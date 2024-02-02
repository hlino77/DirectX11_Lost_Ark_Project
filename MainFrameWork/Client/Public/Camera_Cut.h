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
		_float m_fTargetCameraLength = 1.f;
		_float	fZoomSpeed = 1.0f;

		Vec3	vTargetOffset;
		_float  fReTargetLength = 1.f;
		_float	fTargetZoomSpeed = 1.f;
	}CUTFRAME;

	typedef struct tagCutCameraDesc
	{
		class CGameObject* pCutTarget = nullptr;
		CCamera::CAMERADESC tCameraDesc;
		Vec3 vOffset = Vec3(-1.0f, 0.0f, -1.0f);
		_float fDefaultLength = 2.f;
	}CUTCAMERADESC;

	enum class CameraEstherState { SA, WY, BT, _END };

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
	void		DefaultLength(_float fSpeed) { m_fTargetCameraLength = m_fDefaultLength; m_fZoomSpeed = fSpeed; }
	void		Set_CameraLength(_float fCameraLength) { m_fCameraLength = m_fTargetCameraLength = fCameraLength; }

	void		Set_Offset(Vec3 vOffset) { m_vOffset = vOffset; m_vOffset.Normalize(); }
	void		Set_DefaultOffset() { m_vOffset = m_vDefaultOffset; }
	void		Set_EstherMode(_uint iMode) { m_iState = iMode; }

	// Target
	void		Set_TargetOffset(Vec3 vOffset) { m_vTargetOffset = vOffset; }


	_float		Get_CameraLength() { return m_fCameraLength; }

protected:
	HRESULT		Load_CutFrame();

protected:
	virtual HRESULT Ready_Components() override;

private:
	void		Tick_SilianCamera(_float fTimeDelta);
	void		Tick_BahunturCamera(_float fTimeDelta);
	void		Tick_WayCamera(_float fTimeDelta);


	void		Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta);
private:
	Vec3 m_vOffset;
	Vec3 m_vDefaultOffset;

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
	Vec3	m_vTargetOffset;
	_float	m_fTargetZoomSpeed = 1.f;
	_float	m_fTargetLength = 1.f;
	_float  m_fReTargetLength = 1.f;

	class CGameObject* m_pTarget = nullptr;

	_uint m_iState;

	vector<CUTFRAME> m_vecCutFrame;

public:
	static CCamera_Cut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END