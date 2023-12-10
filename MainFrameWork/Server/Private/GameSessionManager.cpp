#include "stdafx.h"
#include "GameSessionManager.h"
#include "GameSession.h"


IMPLEMENT_SINGLETON(CGameSessionManager)

CGameSessionManager::CGameSessionManager()
{
}

void CGameSessionManager::Tick(_float fTimeDelta)
{
	m_tServerTime.fSecond += fTimeDelta;
	if (m_tServerTime.fSecond >= 60.0f)
	{
		m_tServerTime.fSecond -= 60.0f;
		m_tServerTime.iMinute += 1;
	}
}

void CGameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
	session->SetSessionID(m_iSessionID++);
}

void CGameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void CGameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}

void CGameSessionManager::Broadcast_Others(SendBufferRef sendBuffer, uint32 _ID)
{
	for (GameSessionRef session : _sessions)
	{
		if (session->GetSessionID() != _ID)
			session->Send(sendBuffer);
	}
}

uint32 CGameSessionManager::Get_SessionCount()
{
	READ_LOCK;
	return m_iSessionID;
}
