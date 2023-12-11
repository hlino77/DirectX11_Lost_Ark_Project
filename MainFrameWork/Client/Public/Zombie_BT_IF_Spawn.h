#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"
BEGIN(Client)
class CZombie_BT_IF_Spawn :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Spawn() = default;
	CZombie_BT_IF_Spawn(const CZombie_BT_IF_Spawn& rhs) = delete;
	virtual ~CZombie_BT_IF_Spawn() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
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
		if (static_cast<CMonster*>(m_pGameObject)->Is_Spawn())
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Spawn* Create(void* pArg)
	{
		CZombie_BT_IF_Spawn* pInstance = new CZombie_BT_IF_Spawn;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_IF_Spawn");
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