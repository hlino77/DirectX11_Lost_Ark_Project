#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CCommon_BT_IF_TwistLand_Server :
    public CBT_Decorator
{
private:
	CCommon_BT_IF_TwistLand_Server() = default;
	CCommon_BT_IF_TwistLand_Server(const CCommon_BT_IF_TwistLand_Server& rhs) = delete;
	virtual ~CCommon_BT_IF_TwistLand_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(_float fTimeDelta) override
	{
			m_bCondition = Is_Landing();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Landing()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Is_TwistLanding())
			return true;

		return false;
	}

public:
	static	CCommon_BT_IF_TwistLand_Server* Create(void* pArg)
	{
		CCommon_BT_IF_TwistLand_Server* pInstance = new CCommon_BT_IF_TwistLand_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CCommon_BT_IF_TwistLand_Server");
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