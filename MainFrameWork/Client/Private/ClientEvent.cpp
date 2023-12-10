#include "stdafx.h"
#include "ClientEvent.h"
#include "ServerSessionManager.h"


void CClientEvent::Send_State(EVENTSTATE eState)
{
	Protocol::S_EVENT pkt;

	pkt.set_ieventid(m_iEventID);
	pkt.set_istate((_uint)eState);

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);


}

void CClientEvent::Free()
{
	__super::Free();
}

CClientEvent::CClientEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEvent(iID, pDevice, pContext)
{
}
