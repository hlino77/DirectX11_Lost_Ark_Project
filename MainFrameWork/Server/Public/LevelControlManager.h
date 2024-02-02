#pragma once
#include "Base.h"
#include "Lock.h"
#include "Server_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Server)
class CGameSession;

class CLevelControlManager : public CBase
{
	DECLARE_SINGLETON(CLevelControlManager);

private:
	typedef struct LEVELMOVEDESC
	{
		shared_ptr<CGameSession> pSession = nullptr;
		_uint iCurrLevel;
		_uint iNextLevel;
	};

public:
	explicit CLevelControlManager();
	virtual ~CLevelControlManager() = default;

public:
	HRESULT Login_Player(shared_ptr<CGameSession>& pLoginSession, Protocol::S_LOGIN& pkt);

	void	Update_LevelMove();
	HRESULT Reserve_LevelMove(shared_ptr<CGameSession>& pOwnerSession, _uint iCurrLevel, _uint iNextLevel);

	void Send_LevelState(shared_ptr<CGameSession>& pSession, _uint iLevelState);
public:


private:
	Vec3 Get_LevelSpawnPos(LEVELID eLevel);
	HRESULT Player_LevelMove(shared_ptr<CGameSession>& pOwnerSession, _uint iCurrLevel, _uint iNextLevel);

	void	Compute_DefaultEquipCodes(_uint iClass, vector<_int>& pItemCodes);
private:
	vector<LEVELMOVEDESC> m_LevelMoveList;


	USE_LOCK

public:
	virtual void Free() override;
};

END