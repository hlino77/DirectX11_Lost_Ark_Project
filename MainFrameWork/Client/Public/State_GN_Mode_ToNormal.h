#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_GN_Mode_ToNormal final : public CState
{
public:
	CState_GN_Mode_ToNormal(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_Mode_ToNormal() = default;

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

	std::function<void(CState_GN_Mode_ToNormal&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iMode_ToNormal = 0;

	_int m_iMode_ToNormal_H = 0;
	_int m_iMode_ToNormal_S = 0;
	_int m_iMode_ToNormal_L = 0;

public:
	static CState_GN_Mode_ToNormal* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END