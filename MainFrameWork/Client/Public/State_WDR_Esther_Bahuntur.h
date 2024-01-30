#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_WDR_Esther_Bahuntur final : public CState
{
public:
	CState_WDR_Esther_Bahuntur(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Esther_Bahuntur() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_Esther_Bahuntur&, _float)> m_TickFunc;

private:
	_int	m_iEsther = 0;

public:
	static CState_WDR_Esther_Bahuntur* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END