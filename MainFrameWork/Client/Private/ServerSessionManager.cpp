#include "stdafx.h"
#include "ServerSessionManager.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "ServerSession.h"
#include "Struct.pb.h"
#include "ClientPacketHandler.h"


IMPLEMENT_SINGLETON(CServerSessionManager)

CServerSessionManager::CServerSessionManager()
	:m_fTimesync(3.0f)
{
	m_szNickName = L"";
}

void CServerSessionManager::Tick(_float fTimeDelta)
{
	m_tClientTime.fSecond += fTimeDelta;

	if (m_tClientTime.fSecond >= 60.0f)
	{
		m_tClientTime.fSecond -= 60.0f;
		m_tClientTime.iMinute += 1;
	}

	if (m_bConnected)
	{
		m_fTimesync -= fTimeDelta;
		if (m_fTimesync <= 0.0f)
		{
			m_fTimesync = 3.0f;
			Send_TimeSync();
		}
	}
}

void CServerSessionManager::Set_ServerSession(ServerSessionRef session)
{
	m_pServerSession = session;
	m_bConnected = true;
}

void CServerSessionManager::Send(SendBufferRef sendBuffer)
{
	m_pServerSession->Send(sendBuffer);
}

void CServerSessionManager::Send_TimeSync()
{
	Protocol::S_TIME pkt;
	LARGE_INTEGER iCurrTick;
	QueryPerformanceCounter(&iCurrTick);

	pkt.set_isendtick(iCurrTick.QuadPart);

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	
	Send(pSendBuffer);
}


void CServerSessionManager::Free()
{
	__super::Free();

	m_pServerSession = nullptr;
}

