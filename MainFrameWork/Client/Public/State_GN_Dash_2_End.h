#pragma once
#include "C:\Project\3D Team Progect\MainFrameWork\Reference\Headers\State.h"
class CState_GN_Dahs_2_End :
    public CState
{
};

#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_GN_Dash_2_End final : public CState
{
public:
	CState_GN_Dash_2_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_Dash_2_End() = default;

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

	std::function<void(CState_GN_Dash_2_End&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iDash_End = 0;

public:
	static CState_GN_Dash_2_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END