#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;

class CState_WDR_HeavyCrush final : public CState_Skill
{
public:
	CState_WDR_HeavyCrush(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_HeavyCrush() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Charge(_float fTimeDelta);
	void	Effect_Shot();

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_HeavyCrush&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iHeavyCrush = 0;


	_float m_fChageEffectDelay = 0.0;
	_float m_fChageEffectAcc = 0.0;

	vector<CEffect*> m_Effects;
public:
	static CState_WDR_HeavyCrush* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END