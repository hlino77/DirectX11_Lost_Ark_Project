#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"

BEGIN(Client)

class CZombie_BT_IF_Hit_Left :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Hit_Left() = default;
	CZombie_BT_IF_Hit_Left(const CZombie_BT_IF_Hit_Left& rhs) = delete;
	virtual ~CZombie_BT_IF_Hit_Left() = default;

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
		if (static_cast<CMonster*>(m_pGameObject)->Is_Hit()&& static_cast<CMonster*>(m_pGameObject)->Is_Left())
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Hit_Left* Create(void* pArg)
	{
		CZombie_BT_IF_Hit_Left* pInstance = new CZombie_BT_IF_Hit_Left;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_IF_Hit_Left");
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