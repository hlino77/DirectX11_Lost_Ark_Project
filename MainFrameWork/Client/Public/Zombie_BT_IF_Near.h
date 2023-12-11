#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "Monster.h"
BEGIN(Client)
class CZombie_BT_IF_Near :
    public CBT_Decorator
{
private:
	CZombie_BT_IF_Near() = default;
	CZombie_BT_IF_Near(const CZombie_BT_IF_Near& rhs) = delete;
	virtual ~CZombie_BT_IF_Near() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
			m_bCondition = IsNear();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	IsNear()
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance()<1.f)
			return true;

		return false;
	}

public:
	static	CZombie_BT_IF_Near* Create(void* pArg)
	{
		CZombie_BT_IF_Near* pInstance = new CZombie_BT_IF_Near;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_IF_Near");
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