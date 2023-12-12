#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CZombie_BT_IF_Dead_Server :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Dead_Server() = default;
	CZombie_BT_IF_Dead_Server(const CZombie_BT_IF_Dead_Server& rhs) = delete;
	virtual ~CZombie_BT_IF_Dead_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float& fTimeDelta) override
	{

		m_bCondition = IsZeroHP();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	IsZeroHP()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Get_Hp()<1)
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Dead_Server* Create(void* pArg)
	{
		CZombie_BT_IF_Dead_Server* pInstance = new CZombie_BT_IF_Dead_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CMoloch_BT_IF_Dead");
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