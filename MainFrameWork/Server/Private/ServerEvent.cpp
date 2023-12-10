#include "stdafx.h"
#include "ServerEvent.h"
#include "GameSessionManager.h"
#include "GameSession.h"


void CServerEvent::Send_State(EVENTSTATE eState)
{
	Protocol::S_EVENT pkt;

	pkt.set_ieventid(m_iEventID);
	pkt.set_istate((_uint)eState);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
}

void CServerEvent::Reset_ClientEventState()
{
	set<GameSessionRef>& Sessions = CGameSessionManager::GetInstance()->Get_Sessions();

	m_ClientEventState.clear();

	for (auto& Session : Sessions)
	{
		m_ClientEventState.emplace(Session->GetSessionID(), EVENTSTATE::TYPEEND);
	}
}

void CServerEvent::Set_ClientState(_uint iSessionID, EVENTSTATE eState)
{
	m_ClientEventState[iSessionID] = eState;
}

_bool CServerEvent::Check_ClientState(EVENTSTATE eState)
{
	for (auto& State : m_ClientEventState)
	{
		if (State.second != eState)
			return false;
	}
	return true;
}

void CServerEvent::Free()
{
	__super::Free();
}

CServerEvent::CServerEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEvent(iID, pDevice, pContext)
{
}
