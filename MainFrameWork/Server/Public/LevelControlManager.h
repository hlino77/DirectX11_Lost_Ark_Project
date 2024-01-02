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

	HRESULT Player_LevelMove(shared_ptr<class CGameSession>& pOwnerSession, _uint iCurrLevel, _uint iNextLevel);


	void Send_LevelState(shared_ptr<CGameSession>& pSession, _uint iLevelState);
public:


private:
	Vec3 Get_LevelSpawnPos(LEVELID eLevel);

	USE_LOCK

public:
	virtual void Free() override;
};

END