#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CCamera_Player final : public CCamera
{
public:
	struct PlayerCameraDesc
	{
		class CPlayer* pPlayer = nullptr;
		CCamera::CAMERADESC tCameraDesc;
		Vec3 vOffset = Vec3(-1.0f, 0.0f, -1.0f);
		_float fDefaultLength = 7.5f;
	};

	enum class CameraState { FREE, DEFAULT, RESET, STATEEND };

private:
	CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Player(const CCamera_Player& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera_Player() = default;

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
	Vec3		Get_Offset() { return m_vOffset; }
	void		Set_DefaultOffset() { m_vOffset = m_vDefaultOffset; }
	void		Set_Mode(CameraState eMode) { m_eState = eMode; }
	CameraState Get_Mode() { return m_eState; }

	_float		Get_CameraLength() { return m_fCameraLength; }

	void		Set_ResetSpeed(_float fSpeed) { m_fResetSpeed = fSpeed; }

	void		Set_MotionBlur(_float fTime, _float fIntensity = 0.0f) 
	{ 
		m_bMotionBlur = true;
		m_fMotionBlurAcc = fTime;
		m_fMotionBlurIntensity = fIntensity; 
	}

	void		Set_RadialBlur(_float fTime, Vec3& vPos, _float fDamping,_float fIntensity = 0.0f)
	{
		m_bRadialBlur = true;
		m_fRadialBlurAcc = fTime;
		m_fRadialBlurIntensity = fIntensity;
		m_fRadialBlurDamping = fDamping;
		m_vRadialPos = vPos;
	}

	void		Set_Chromatic(_float fTime, Vec3& vPos, _float fDamping, _float fIntensity = 0.0f)
	{
		m_bChromatic = true;
		m_fChromaticAcc = fTime;
		m_fChromaticIntensity = fIntensity;
		m_fChromaticDamping = fDamping;
		m_vChromaticPos = vPos;
	}

	void		Set_FadeInOut(_float fSpeed, _bool bInOut, Vec4 vColor = Vec4());
	void		Set_FadeIntensity(_float fIntensity) { m_fFadeIntensity = fIntensity; }

	void	Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }

protected:
	virtual HRESULT Ready_Components() override;

private:
	void		Tick_FreeCamera(_float fTimeDelta);
	void		Tick_DefaultCamera(_float fTimeDelta);
	void		Tick_ResetCamera(_float fTimeDelta);
	void		Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight, _float fTimeDelta);
	HRESULT		Render_FadeInOut();

	void		Ready_FadeInOut();
	void		Update_FadeInOut(_float fTimeDelta);
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

	CGameObject* m_pTarget = nullptr;

	CameraState m_eState;

	// MotionBlur
	_bool	m_bMotionBlur = false;
	_float	m_fMotionBlurAcc = 0.0f;
	_float	m_fMotionBlurIntensity = 0.0f;

	// RadialBlur
	_bool	m_bRadialBlur = false;
	_float	m_fRadialBlurAcc = 0.0f;
	_float	m_fRadialBlurIntensity = 0.0f;
	_float	m_fRadialBlurDamping = 0.0f;
	Vec3	m_vRadialPos;

	// ChromaticAberration
	_bool	m_bChromatic = false;
	_float	m_fChromaticAcc = 0.0f;
	_float	m_fChromaticIntensity = 0.f;
	_float	m_fChromaticDamping = 0.0f;
	Vec3	m_vChromaticPos;

	_float	m_fResetSpeed = 0.0f;

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureFade = { nullptr };

	_bool m_bFade = false;
	_float m_fFadeIntensity = 0.0f;
	_float m_fFadeSpeed = 0.0f;
	Vec4 m_vFadeColor;
	_bool m_bFadeInOut = false;

	Matrix m_FadeWorld, m_FadeVeiw, m_FadeProj;
public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END