#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Controller_MG.h"

BEGIN(Client)

class CState_MG_Hit final : public CState
{
public:
	CState_MG_Hit(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_Hit() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

	void	Hit_Down(_float fTimeDelta);

private:
	class CPlayer_Bard* m_pPlayer = nullptr;

	std::function<void(CState_MG_Hit&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iHit = 0;

	_int m_iHit_Down = 0;
	_int m_iHit_KnockDown = 0;
	_int m_iHit_Bound = 0;
	_int m_iHit_Twist = 0;

	_float m_fTimeAcc = 0.f;
	_float m_fDownTime = 2.f;

	CPlayer_Controller::HIT_TYPE m_eHitType;
	_float m_fForceDist;
	_float m_fHitCheck;
	Vec3   m_vHitCenter;

	_bool	m_IsAnimEnd = { false };

	_bool	m_IsFall = { false };
	_uint	m_iFallFrame = { 0 };

public:
	static CState_MG_Hit* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END