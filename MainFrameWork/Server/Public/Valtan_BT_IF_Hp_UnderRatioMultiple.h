#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_IF_Hp_UnderRatio :
    public CBT_Decorator
{

private:
	CValtan_BT_IF_Hp_UnderRatio() = default;
	CValtan_BT_IF_Hp_UnderRatio(const CValtan_BT_IF_Hp_UnderRatio& rhs) = delete;
	virtual ~CValtan_BT_IF_Hp_UnderRatio() = default;


public:
	void	Set_Ratio(_float fRatio) {	m_fSpecialSkillRatio = fRatio;	}

private:
	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_UnderCertainHp();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
		if (m_eReturn == BT_SUCCESS)
			m_bIsPlayed = true;
	}

private:
	_bool	Is_UnderCertainHp()
	{
		_float dPercent =	_float(m_pGameObject->Get_Hp()) / _float(m_pGameObject->Get_MaxHp());
		if (m_eReturn == BT_RUNNING ||dPercent <= m_fSpecialSkillRatio&& !m_bIsPlayed)
			return true;

		return false;
	}

private:

	_float m_fSpecialSkillRatio = 0.f;
	_bool m_bIsPlayed = false;

public:
	static	CValtan_BT_IF_Hp_UnderRatio* Create(void* pArg)
	{
		CValtan_BT_IF_Hp_UnderRatio* pInstance = new CValtan_BT_IF_Hp_UnderRatio;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CValtan_BT_IF_Hp_UnderRatio");
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