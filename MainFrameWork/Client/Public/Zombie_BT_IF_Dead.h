#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"
BEGIN(Client)
class CZombie_BT_IF_Dead :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Dead() = default;
	CZombie_BT_IF_Dead(const CZombie_BT_IF_Dead& rhs) = delete;
	virtual ~CZombie_BT_IF_Dead() = default;

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
		if (static_cast<CMonster*>(m_pGameObject)->Get_Hp()<0)
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Dead* Create(void* pArg)
	{
		CZombie_BT_IF_Dead* pInstance = new CZombie_BT_IF_Dead;

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