#pragma once
#include "Event.h"
#include "Client_Defines.h"

BEGIN(Client)

class CClientEvent : public CEvent
{
protected:
	CClientEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CClientEvent() = default;

public:
	virtual HRESULT Initialize() PURE;

	virtual void Enter_Event() PURE;
	virtual void Exit_Event() PURE;

	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;



	void		Send_State(EVENTSTATE eState);

protected:


public:
	virtual void Free();
};

END