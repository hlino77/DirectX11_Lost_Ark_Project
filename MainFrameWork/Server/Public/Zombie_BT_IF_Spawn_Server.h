#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CZombie_BT_IF_Spawn_Server :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Spawn_Server() = default;
	CZombie_BT_IF_Spawn_Server(const CZombie_BT_IF_Spawn_Server& rhs) = delete;
	virtual ~CZombie_BT_IF_Spawn_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Spawn();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Spawn()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Spawn())
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Spawn_Server* Create(void* pArg)
	{
		CZombie_BT_IF_Spawn_Server* pInstance = new CZombie_BT_IF_Spawn_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_IF_Spawn_Server");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free() override
	{
		__super::Free();
	}
};

END