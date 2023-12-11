#pragma once
#include "Session.h"
#include "SendBuffer.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

//TODO
enum : uint16
{
	PKT_S_TIME = 1,
	PKT_S_LOGIN = 2,
	PKT_S_OPENLEVEL = 3,
	PKT_S_LEVELSTATE = 4,
	PKT_S_CREATEOBJECT = 5,
	PKT_S_ANIMATION = 6,
	PKT_S_OBJECTINFO = 7,
	PKT_S_STATE = 8,
	PKT_S_COLLIDERSTATE = 9,
	PKT_S_COLLISION = 10,
	PKT_S_USERINFO = 11,
	PKT_S_NEARTARGET = 12,
	PKT_S_SLOWMOTION = 13,
	PKT_S_EVENT = 14,
	PKT_S_HP = 15,
	PKT_S_CREATEPLAYER = 16,
};

// TODO
bool Handle_INVALID_Client(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handel_S_TIME_Client(PacketSessionRef& session, Protocol::S_TIME& pkt);
bool Handel_S_LOGIN_Client(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handel_S_OPENLEVEL_Client(PacketSessionRef& session, Protocol::S_OPEN_LEVEL& pkt);
bool Handel_S_LEVELSTATE_Client(PacketSessionRef& session, Protocol::S_LEVEL_STATE& pkt);
bool Handel_S_CREATEOBJECT_Client(PacketSessionRef& session, Protocol::S_CREATE_OBJCECT& pkt);
bool Handel_S_ANIMATION_Client(PacketSessionRef& session, Protocol::S_ANIMATION& pkt);
bool Handel_S_OBJECTINFO_Client(PacketSessionRef& session, Protocol::S_OBJECTINFO& pkt);
bool Handel_S_STATE_Client(PacketSessionRef& session, Protocol::S_STATE& pkt);
bool Handel_S_COLLIDERSTATE_Client(PacketSessionRef& session, Protocol::S_COLLIDERSTATE& pkt);
bool Handel_S_COLLISION_Client(PacketSessionRef& session, Protocol::S_COLLISION& pkt);
bool Handel_S_USERINFO_Client(PacketSessionRef& session, Protocol::S_USERINFO& pkt);
bool Handel_S_NEARTARGET_Client(PacketSessionRef& session, Protocol::S_NEARTARGET& pkt);
bool Handel_S_SLOWMOTION_Client(PacketSessionRef& session, Protocol::S_SLOWMOTION& pkt);
bool Handel_S_EVENT_Client(PacketSessionRef& session, Protocol::S_EVENT& pkt);
bool Handel_S_HP_Client(PacketSessionRef& session, Protocol::S_HP& pkt);
bool Handel_S_CREATEPLAYER_Client(PacketSessionRef& session, Protocol::S_CREATE_PLAYER& pkt);




class CClientPacketHandler
{
public:
	// TODO
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID_Client;

		GPacketHandler[PKT_S_TIME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TIME>(Handel_S_TIME_Client, session, buffer, len); };
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handel_S_LOGIN_Client, session, buffer, len); };
		GPacketHandler[PKT_S_OPENLEVEL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_OPEN_LEVEL>(Handel_S_OPENLEVEL_Client, session, buffer, len); };
		GPacketHandler[PKT_S_LEVELSTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEVEL_STATE>(Handel_S_LEVELSTATE_Client, session, buffer, len); };
		GPacketHandler[PKT_S_CREATEOBJECT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATE_OBJCECT>(Handel_S_CREATEOBJECT_Client, session, buffer, len); };
		GPacketHandler[PKT_S_ANIMATION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ANIMATION>(Handel_S_ANIMATION_Client, session, buffer, len); };
		GPacketHandler[PKT_S_OBJECTINFO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_OBJECTINFO>(Handel_S_OBJECTINFO_Client, session, buffer, len); };
		GPacketHandler[PKT_S_STATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_STATE>(Handel_S_STATE_Client, session, buffer, len); };
		GPacketHandler[PKT_S_COLLIDERSTATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_COLLIDERSTATE>(Handel_S_COLLIDERSTATE_Client, session, buffer, len); };
		GPacketHandler[PKT_S_COLLISION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_COLLISION>(Handel_S_COLLISION_Client, session, buffer, len); };
		GPacketHandler[PKT_S_USERINFO] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_USERINFO>(Handel_S_USERINFO_Client, session, buffer, len); };
		GPacketHandler[PKT_S_NEARTARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_NEARTARGET>(Handel_S_NEARTARGET_Client, session, buffer, len); };
		GPacketHandler[PKT_S_SLOWMOTION] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SLOWMOTION>(Handel_S_SLOWMOTION_Client, session, buffer, len); };
		GPacketHandler[PKT_S_EVENT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_EVENT>(Handel_S_EVENT_Client, session, buffer, len); };
		GPacketHandler[PKT_S_HP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_HP>(Handel_S_HP_Client, session, buffer, len); };
		GPacketHandler[PKT_S_CREATEPLAYER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CREATE_PLAYER>(Handel_S_CREATEPLAYER_Client, session, buffer, len); };

	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}


	// TODO
	static SendBufferRef MakeSendBuffer(Protocol::S_TIME& pkt) { return MakeSendBuffer(pkt, PKT_S_TIME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OPEN_LEVEL& pkt) { return MakeSendBuffer(pkt, PKT_S_OPENLEVEL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEVEL_STATE& pkt) { return MakeSendBuffer(pkt, PKT_S_LEVELSTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_OBJCECT& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATEOBJECT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ANIMATION& pkt) { return MakeSendBuffer(pkt, PKT_S_ANIMATION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_OBJECTINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OBJECTINFO); }
	static SendBufferRef MakeSendBuffer(Protocol::S_STATE& pkt) { return MakeSendBuffer(pkt, PKT_S_STATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_COLLIDERSTATE& pkt) { return MakeSendBuffer(pkt, PKT_S_COLLIDERSTATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_COLLISION& pkt) { return MakeSendBuffer(pkt, PKT_S_COLLISION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_USERINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_USERINFO); }
	static SendBufferRef MakeSendBuffer(Protocol::S_NEARTARGET& pkt) { return MakeSendBuffer(pkt, PKT_S_NEARTARGET); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SLOWMOTION& pkt) { return MakeSendBuffer(pkt, PKT_S_SLOWMOTION); }
	static SendBufferRef MakeSendBuffer(Protocol::S_EVENT& pkt) { return MakeSendBuffer(pkt, PKT_S_EVENT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_HP& pkt) { return MakeSendBuffer(pkt, PKT_S_HP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATEPLAYER); }


private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = SendBufferManager::GetInstance()->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};

