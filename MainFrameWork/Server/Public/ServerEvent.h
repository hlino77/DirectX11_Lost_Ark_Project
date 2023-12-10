#pragma once
#include "Event.h"
#include "Server_Defines.h"

BEGIN(Server)

class CServerEvent : public CEvent
{
protected:
	CServerEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CServerEvent() = default;

public:
	virtual HRESULT Initialize() PURE;

	virtual void Enter_Event() PURE;
	virtual void Exit_Event() PURE;

	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;


	
	void		Send_State(EVENTSTATE eState);
	void		Reset_ClientEventState();
	void		Set_ClientState(_uint iSessionID, EVENTSTATE eState);

	_bool		Check_ClientState(EVENTSTATE eState);

protected:
	map<_uint, EVENTSTATE> m_ClientEventState;
public:
	virtual void Free();
};

END