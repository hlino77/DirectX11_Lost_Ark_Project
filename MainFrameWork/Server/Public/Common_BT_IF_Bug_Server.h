#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CCommon_BT_IF_Bug_Server :
    public CBT_Decorator
{
private:
	CCommon_BT_IF_Bug_Server() = default;
	CCommon_BT_IF_Bug_Server(const CCommon_BT_IF_Bug_Server& rhs) = delete;
	virtual ~CCommon_BT_IF_Bug_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = IsBug();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	IsBug()
	{

		if (static_cast<CMonster_Server*>(m_pGameObject)->Get_StatusEffect(STATUSEFFECT::BUG) > 0 || m_eReturn == BT_RUNNING)
			return true;


		return false;
	}

public:
	static	CCommon_BT_IF_Bug_Server* Create(void* pArg)
	{
		CCommon_BT_IF_Bug_Server* pInstance = new CCommon_BT_IF_Bug_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CCommon_BT_IF_Bug_Server");
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