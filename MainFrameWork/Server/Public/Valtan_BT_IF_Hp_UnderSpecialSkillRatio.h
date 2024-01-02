#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CValtan_BT_IF_Hp_UnderSpecialSkillRatio :
    public CBT_Decorator
{
public:
	typedef struct tag_IFUnderSpecialSkillRatio_Desc : public DECORATOR_DESC
	{
		_float fSpecialSkillRatio;
	}IF_HPUNDERSPECIALSKILLRATIO_DESC;
private:
	CValtan_BT_IF_Hp_UnderSpecialSkillRatio() = default;
	CValtan_BT_IF_Hp_UnderSpecialSkillRatio(const CValtan_BT_IF_Hp_UnderSpecialSkillRatio& rhs) = delete;
	virtual ~CValtan_BT_IF_Hp_UnderSpecialSkillRatio() = default;

	virtual HRESULT Initialize(void* pArg) override
	{
		if (pArg == nullptr)
			return E_FAIL;
		if (FAILED(__super::Initialize(pArg)))
			return E_FAIL;
		IF_HPUNDERSPECIALSKILLRATIO_DESC* pDecoratorDesc = static_cast<IF_HPUNDERSPECIALSKILLRATIO_DESC*> (pArg);
		if (pDecoratorDesc == nullptr)
			return E_FAIL;
		m_fSpecialSkillPercentage = pDecoratorDesc->fSpecialSkillRatio;
		m_bIsPlayed = false;
		return S_OK;
	}

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
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Hp()/ m_pGameObject->Get_MaxHp()<= m_fSpecialSkillPercentage&& !m_bIsPlayed)
			return true;

		return false;
	}
private:

	_float m_fSpecialSkillPercentage = 0.f;
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