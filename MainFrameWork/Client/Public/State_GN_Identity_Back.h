#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_GN_Identity_Back final : public CState
{
public:
	CState_GN_Identity_Back(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_Identity_Back() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_Identity_Back&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iIdentity = 0;
	_int m_iIdentity_HandtoShot = 0;
	_int m_iIdentity_ShottoLong = 0;
	_int m_iIdentity_LongtoHand = 0;

public:
	static CState_GN_Identity_Back* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END