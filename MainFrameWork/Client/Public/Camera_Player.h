#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)


class CCamera_Player final : public CCamera
{
public:
	typedef struct PlayerCameraDesc
	{
		class CPlayer* pPlayer = nullptr;
		CCamera::CAMERADESC tCameraDesc;
		Vec3 vOffset = Vec3(-1.0f, 0.0f, -1.0f);
	};

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

	void		Update_ShakeLook(Vec3& vLook, Vec3 vUp, Vec3 vRight);

protected:
	virtual HRESULT Ready_Components() override;


	
private:
	Vec3 m_vOffset;

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

	class CPlayer* m_pPlayer = nullptr;
public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END