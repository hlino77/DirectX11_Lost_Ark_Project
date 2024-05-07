#pragma once
#include "Engine_Defines.h"
#include "IocpCore.h"
#include "NetAddress.h"


BEGIN(Engine)
class AcceptEvent;
class ServerService;

class ENGINE_DLL Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	bool StartAccept(ServerServiceRef service);
	void CloseSocket();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
	ServerServiceRef _service;
};

END