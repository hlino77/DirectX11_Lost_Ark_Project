#pragma once
#include "ClientEvent.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_Free;

class CClientEvent_BernStart : public CClientEvent
{
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

protected:
	EVENTSTATE eState;

	CCamera_Free* m_pCamera = nullptr;

	_bool m_bEnd = false;
	_float m_fDelayTime = 0.0f;

private:
	_float m_fCameraTime = 0.0f;

public:
	virtual void Free();
};

END