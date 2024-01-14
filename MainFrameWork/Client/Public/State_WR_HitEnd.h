#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Controller_WR.h"

BEGIN(Client)

class CState_WR_HitEnd final : public CState
{
public:
	CState_WR_HitEnd(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_HitEnd() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_HitEnd&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iHitEnd = 0;

	_int m_iHitEnd_KnockDown = 0;
	_int m_iHitEnd_Bound = 0;
	_int m_iHitEnd_Twist = 0;

	_float m_fTimeAcc = 0.f;
	_float m_fDownTime = 3.f;

	CPlayer_Controller::HIT_TYPE m_eHitType;
	_float m_fForceDist;

	_bool	m_IsAnimEnd = { false };

public:
	static CState_WR_HitEnd* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END