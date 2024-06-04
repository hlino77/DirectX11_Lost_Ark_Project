#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_IF_Armor_Server :
    public CBT_Decorator
{
private:
	CValtan_BT_IF_Armor_Server() = default;
	CValtan_BT_IF_Armor_Server(const CValtan_BT_IF_Armor_Server& rhs) = delete;
	virtual ~CValtan_BT_IF_Armor_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(_float fTimeDelta) override
	{
		m_bCondition = Is_Armor();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Armor()
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Armor()>0)
			return true;

		return false;
	}

public:
	static	CValtan_BT_IF_Armor_Server* Create(void* pArg)
	{
		CValtan_BT_IF_Armor_Server* pInstance = new CValtan_BT_IF_Armor_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CValtan_BT_IF_Armor_Server");
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