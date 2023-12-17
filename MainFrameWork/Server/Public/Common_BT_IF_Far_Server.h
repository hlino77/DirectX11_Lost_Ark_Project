#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CCommon_BT_IF_Far_Server :
    public CBT_Decorator
{
private:
	CCommon_BT_IF_Far_Server() = default;
	CCommon_BT_IF_Far_Server(const CCommon_BT_IF_Far_Server& rhs) = delete;
	virtual ~CCommon_BT_IF_Far_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
			m_bCondition = IsFar();
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction( fTimeDelta);
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	IsFar()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance()>= static_cast<CMonster_Server*>(m_pGameObject)->Get_AttackRange())
			return true;

		return false;
	}

public:
	static	CCommon_BT_IF_Far_Server* Create(void* pArg)
	{
		CCommon_BT_IF_Far_Server* pInstance = new CCommon_BT_IF_Far_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CCommon_BT_IF_Far_Server");
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