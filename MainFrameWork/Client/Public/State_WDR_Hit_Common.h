#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Controller_WDR.h"

BEGIN(Client)

class CState_WDR_Hit_Common final : public CState
{
public:
	CState_WDR_Hit_Common(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Hit_Common() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Hit_Dmg(_float fTimeDelta);

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_Hit_Common&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iHit_Dmg = 0;

	_int m_iHit_Dmg_1 = 0;
	_int m_iHit_Dmg_2 = 0;

	_float m_fTimeAcc = 0.f;
	_float m_fDmgTime = 1.f;

	CPlayer_Controller::HIT_TYPE m_eHitType;
	_float m_fForceDist;

public:
	static CState_WDR_Hit_Common* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END