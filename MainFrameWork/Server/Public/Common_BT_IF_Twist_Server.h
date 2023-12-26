#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"

BEGIN(Server)

class CCommon_BT_IF_Twist_Server :
    public CBT_Decorator
{
private:
	CCommon_BT_IF_Twist_Server() = default;
	CCommon_BT_IF_Twist_Server(const CCommon_BT_IF_Twist_Server& rhs) = delete;
	virtual ~CCommon_BT_IF_Twist_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Twist();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Twist()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Twist())
			return true;

		return false;
	}

public:
	static	CCommon_BT_IF_Twist_Server* Create(void* pArg)
	{
		CCommon_BT_IF_Twist_Server* pInstance = new CCommon_BT_IF_Twist_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CCommon_BT_IF_Twist_Server");
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