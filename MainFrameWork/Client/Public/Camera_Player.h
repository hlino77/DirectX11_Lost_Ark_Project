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
	void		Cam_Shake(_float fForce, _float fTime);



protected:
	virtual HRESULT Ready_Components() override;


	
private:
	_bool m_bMouseMove = true;
	Vec3 m_vOffset;
	_float m_fCameraLength;

	//CamShake
	_float	m_fShakeTime = 0.0f;
	_float	m_fCurrShakeTime = 0.0f;
	_float	m_fShakeForce = 0.0f;
	_bool	m_bShake = false;
	Vec3	m_vOriginLook;

	class CPlayer* m_pPlayer = nullptr;
public:
	static CCamera_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END