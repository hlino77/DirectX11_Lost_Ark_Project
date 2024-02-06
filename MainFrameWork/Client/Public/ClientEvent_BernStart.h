#pragma once
#include "ClientEvent.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_Free;

class CClientEvent_BernStart : public CClientEvent
{
private:
	typedef struct CameraPos
	{
		_float fSpeed = 1.0f;
		Vec3 vPos;
		Vec3 vLook;
	};

public:
	CClientEvent_BernStart(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CClientEvent_BernStart() = default;

public:
	virtual HRESULT Initialize();

	virtual void Enter_Event();
	virtual void Exit_Event();

	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render();

private:
	void		Update_Camera(_float fTimeDelta);

	void		Load_CameraData();
private:
	EVENTSTATE eState;

	_float m_fCameraSpeed = 1.0f;
	_float m_fTimeRatio = 0.0f;

	vector<vector<CameraPos>> m_CameraPos;
	_uint m_iCurrPos = 0;
	_uint m_iCurrScene = 0;

public:
	virtual void Free();
};

END