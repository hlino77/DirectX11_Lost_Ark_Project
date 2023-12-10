#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEvent : public CBase
{
protected:
	CEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEvent() = default;

public:
	virtual HRESULT Initialize() PURE;

	virtual void Enter_Event() PURE;
	virtual void Exit_Event() PURE;

	virtual void Tick(_float fTimeDelta) PURE;
	virtual void LateTick(_float fTimeDelta) PURE;
	virtual HRESULT Render() PURE;

	void		Set_State(_uint iState) { m_iState = iState; }

	
	_uint Get_ID() { return m_iEventID; }


protected:
	_int m_iEventID = -1;
	_uint m_iState = 0;

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	


public:
	virtual void Free();
};

END