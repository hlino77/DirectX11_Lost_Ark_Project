#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_WDR_Dash final : public CState
{
public:
	CState_WDR_Dash(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Dash() = default;

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

	std::function<void(CState_WDR_Dash&, _float)> m_TickFunc;

private:

	//Animation
	_int m_iDash = 0;

	// Sound
	_bool   m_EffectSound = false;
	_bool   m_PlayerSound = false;
	_float  m_EffectSoundAccTime = 0.f;
	_float	m_PlayerSoundAccTime = 0.f;


public:
	static CState_WDR_Dash* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END