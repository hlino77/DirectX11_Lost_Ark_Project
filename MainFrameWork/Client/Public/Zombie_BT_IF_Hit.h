#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"

BEGIN(Client)

class CZombie_BT_IF_Hit :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Hit() = default;
	CZombie_BT_IF_Hit(const CZombie_BT_IF_Hit& rhs) = delete;
	virtual ~CZombie_BT_IF_Hit() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Hit();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Hit()
	{
		if (static_cast<CMonster*>(m_pGameObject)->Is_Hit())
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Hit* Create(void* pArg)
	{
		CZombie_BT_IF_Hit* pInstance = new CZombie_BT_IF_Hit;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_IF_Hit");
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