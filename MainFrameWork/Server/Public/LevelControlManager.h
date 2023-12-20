#pragma once
#include "Base.h"
#include "Lock.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Server)



class CLevelControlManager : public CBase
{
	DECLARE_SINGLETON(CLevelControlManager);

public:
	explicit CLevelControlManager();
	virtual ~CLevelControlManager() = default;

public:
	HRESULT Login_Player(shared_ptr<class CGameSession>& pLoginSession, Protocol::S_LOGIN& pkt);


public:


private:


	USE_LOCK

public:
	virtual void Free() override;
};

END