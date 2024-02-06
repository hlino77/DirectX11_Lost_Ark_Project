#pragma once
#include "Base.h"

BEGIN(Server)

class CPlayer_Server;

class CParty_Server : public CBase
{
public:
	CParty_Server(vector<CPlayer_Server*>& Players);
	~CParty_Server();
public:
	void	Add_Player(CPlayer_Server* pPlayer);

	vector<CPlayer_Server*>& Get_Players() { return m_Players; }
private:
	vector<CPlayer_Server*> m_Players;

	USE_LOCK
};

END