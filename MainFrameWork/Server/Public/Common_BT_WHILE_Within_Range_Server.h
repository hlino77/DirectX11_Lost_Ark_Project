#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CCommon_BT_WHILE_Within_Range_Server :
    public CBT_Decorator
{
private:
	CCommon_BT_WHILE_Within_Range_Server() = default;
	CCommon_BT_WHILE_Within_Range_Server(const CCommon_BT_WHILE_Within_Range_Server& rhs) = delete;
	virtual ~CCommon_BT_WHILE_Within_Range_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(_float fTimeDelta) override
	{
		m_bCondition = Is_Within_Range();
	
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Within_Range()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < static_cast<CMonster_Server*>(m_pGameObject)->Get_NoticeRange() && !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
			return true;

		return false;
	}

public:
	static	CCommon_BT_WHILE_Within_Range_Server* Create(void* pArg)
	{
		CCommon_BT_WHILE_Within_Range_Server* pInstance = new CCommon_BT_WHILE_Within_Range_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CCommon_BT_WHILE_Within_Range_Server");
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