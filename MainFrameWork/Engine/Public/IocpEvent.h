#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class Session;

enum class ENGINE_DLL EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send
};

class ENGINE_DLL IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();

public:
	EventType		eventType;
	IocpObjectRef	owner;
};

class ENGINE_DLL ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

class ENGINE_DLL DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};

class ENGINE_DLL AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	SessionRef	session = nullptr;
};

class ENGINE_DLL RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) { }
};

class ENGINE_DLL SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }

	vector<SendBufferRef> sendBuffers;
};

END