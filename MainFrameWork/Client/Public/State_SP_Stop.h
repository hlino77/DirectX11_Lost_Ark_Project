#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_SP_Stop final : public CState
{
public:
	CState_SP_Stop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual ~CState_SP_Stop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Doaga* m_pPlayer = nullptr;

	std::function<void(CState_SP_Stop&, _float)> m_TickFunc;

private:
	//Animation

public:
	static CState_SP_Stop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Doaga* pOwner);
	virtual void Free() override;
};

END