#include "EventMgr.h"
#include "PipeLine.h"
#include "Transform.h"
#include "GameObject.h"
#include "Event.h"

IMPLEMENT_SINGLETON(CEventMgr)

CEventMgr::CEventMgr()
{
}

HRESULT CEventMgr::Reserve_EventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	return S_OK;
}

void CEventMgr::Tick(_float fTimeDelta)
{
	if (m_pCurrEvent)
		m_pCurrEvent->Tick(fTimeDelta);
}

void CEventMgr::LateTick(_float fTimeDelta)
{
	if (m_pCurrEvent)
		m_pCurrEvent->LateTick(fTimeDelta);
}

HRESULT CEventMgr::Render()
{
	if (m_pCurrEvent)
		return m_pCurrEvent->Render();
		
}

HRESULT CEventMgr::Add_Event(CEvent* pEvent)
{
	pEvent->Initialize();

	m_Events.emplace(pEvent->Get_ID(), pEvent);
	return S_OK;
}

HRESULT CEventMgr::Start_Event(_uint iEventID)
{
	if (iEventID >= m_Events.size())
		return E_FAIL;

	if (m_pCurrEvent)
	{
		m_pCurrEvent->Exit_Event();
		m_pCurrEvent = nullptr;
	}

	m_pCurrEvent = m_Events[iEventID];
	m_pCurrEvent->Enter_Event();

	return S_OK;
}

HRESULT CEventMgr::End_Event(_uint iEventID)
{
	if (m_pCurrEvent == nullptr || m_pCurrEvent->Get_ID() != iEventID)
		return E_FAIL;

	m_pCurrEvent->Exit_Event();

	m_pCurrEvent = nullptr;
	return S_OK;
}

CEvent* CEventMgr::Get_Event(_uint iEventID)
{
	return m_Events[iEventID];
}


void CEventMgr::Free()
{
}


