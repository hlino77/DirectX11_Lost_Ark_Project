#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_Repeat_Server :
    public CBT_Decorator
{
private:
	CValtan_BT_Repeat_Server() = default;
	CValtan_BT_Repeat_Server(const CValtan_BT_Repeat_Server& rhs) = delete;
	virtual ~CValtan_BT_Repeat_Server() = default;

	virtual void OnStart() override
	{
		__super::OnStart();
	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition =true;
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		__super::OnEnd();
		Reset();
	}




public:
	static	CValtan_BT_Repeat_Server* Create(void* pArg)
	{
		CValtan_BT_Repeat_Server* pInstance = new CValtan_BT_Repeat_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CValtan_BT_Repeat_Server");
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