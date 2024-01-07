#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_IF_Phase3_Server :
    public CBT_Decorator
{
private:
	CValtan_BT_IF_Phase3_Server() = default;
	CValtan_BT_IF_Phase3_Server(const CValtan_BT_IF_Phase3_Server& rhs) = delete;
	virtual ~CValtan_BT_IF_Phase3_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Phase3();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Phase3()
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase()==3 && !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
			return true;

		return false;
	}

public:
	static	CValtan_BT_IF_Phase3_Server* Create(void* pArg)
	{
		CValtan_BT_IF_Phase3_Server* pInstance = new CValtan_BT_IF_Phase3_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CValtan_BT_IF_Phase3_Server");
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