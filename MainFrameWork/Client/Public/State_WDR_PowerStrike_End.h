#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_PowerStrike_End final : public CState_Skill
{
public:
	CState_WDR_PowerStrike_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PowerStrike_End() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Shot();

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PowerStrike_End&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPowerStrike_End = 0;

public:
	static CState_WDR_PowerStrike_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END