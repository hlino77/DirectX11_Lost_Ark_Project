#include "stdafx.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include "ClientPacketHandler.h"
#include "AsUtils.h"

CServerSession::CServerSession()
	: m_eLevelState(LEVELSTATE::STATEEND)
{
}

void CServerSession::OnConnected()
{
	CServerSessionManager::GetInstance()->Set_ServerSession(static_pointer_cast<CServerSession>(shared_from_this()));

	Protocol::S_NICKNAME pkt;

	pkt.set_strnickname(CAsUtils::ToString(CServerSessionManager::GetInstance()->Get_NickName()));

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = PacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO : packetId 대역 체크
	CClientPacketHandler::HandlePacket(session, buffer, len);
}

void CServerSession::OnSend(int32 len)
{
}

void CServerSession::OnDisconnected()
{
}

LEVELSTATE CServerSession::Get_LevelState()
{
	READ_LOCK
	return m_eLevelState;
}

void CServerSession::Set_LevelState(LEVELSTATE eState)
{
	WRITE_LOCK
	m_eLevelState = eState;
}
