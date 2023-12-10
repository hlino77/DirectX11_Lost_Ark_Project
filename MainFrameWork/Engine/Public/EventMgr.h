#pragma once
#include "Base.h"
#include "AsTypes.h"
#include "Lock.h"

BEGIN(Engine)

class CEvent;

class ENGINE_DLL CEventMgr : public CBase
{
	DECLARE_SINGLETON(CEventMgr);

private:
	explicit CEventMgr();
	virtual ~CEventMgr() = default;


	USE_LOCK
public:
	HRESULT		Reserve_EventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void		Tick(_float fTimeDelta);
	void		LateTick(_float fTimeDelta);
	HRESULT		Render();


	HRESULT		Add_Event(CEvent* pEvent);
	HRESULT		Start_Event(_uint iEventID);
	HRESULT		End_Event(_uint iEventID);



	CEvent*		Get_Event(_uint iEventID);


private:




public:


private:
	unordered_map<_uint, CEvent*> m_Events;
	CEvent* m_pCurrEvent = nullptr;

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
public:
	virtual void Free() override;
};

END