#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_IF_Hp_UnderSpecialSkillRatio :
    public CBT_Decorator
{

private:
	CValtan_BT_IF_Hp_UnderSpecialSkillRatio() = default;
	CValtan_BT_IF_Hp_UnderSpecialSkillRatio(const CValtan_BT_IF_Hp_UnderSpecialSkillRatio& rhs) = delete;
	virtual ~CValtan_BT_IF_Hp_UnderSpecialSkillRatio() = default;


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
		m_bIsPlayed = true;
	}

private:
	_bool	Is_UnderCertainHp()
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Hp()/ m_pGameObject->Get_MaxHp()<= m_fSpecialSkillRatio&& !m_bIsPlayed)
			return true;

		return false;
	}

private:

	_float m_fSpecialSkillRatio = 0.f;
	_bool m_bIsPlayed = false;

public:
	static	CValtan_BT_IF_Hp_UnderSpecialSkillRatio* Create(void* pArg)
	{
		CValtan_BT_IF_Hp_UnderSpecialSkillRatio* pInstance = new CValtan_BT_IF_Hp_UnderSpecialSkillRatio;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CValtan_BT_IF_Hp_UnderSpecialSkillRatio");
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