#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"
BEGIN(Client)
class CZombie_BT_WHILE_Within_Range :
    public CBT_Decorator
{
private:
	CZombie_BT_WHILE_Within_Range() = default;
	CZombie_BT_WHILE_Within_Range(const CZombie_BT_WHILE_Within_Range& rhs) = delete;
	virtual ~CZombie_BT_WHILE_Within_Range() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Within_Range();
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction(fTimeDelta);
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Within_Range()
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() < 10.f&& !static_cast<CMonster*>(m_pGameObject)->Is_Hit())
			return true;

		return false;
	}

public:
	static	CZombie_BT_WHILE_Within_Range* Create(void* pArg)
	{
		CZombie_BT_WHILE_Within_Range* pInstance = new CZombie_BT_WHILE_Within_Range;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_WHILE_Within_Range");
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